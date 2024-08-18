#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define CLOCKS_PER_SEC 1000000
#define SWITCH_ALARMA_BASE 0x00011000 // Dirección base del switch (ajustar real)
#define BUTTON_NUMERO_BASE 0x00011010 // Dirección base del botón (ajustar real)
#define BUTTON_CONFIRMAR_BASE 0x00011020 // Dirección base del botón (ajustar real)
#define DISPLAY_1_BASE 0x00011030 // Dirección base del display 1
#define DISPLAY_2_BASE 0x00011040 // Dirección base del display 2
#define DISPLAY_3_BASE 0x00011050 // Dirección base del display 3
#define DISPLAY_4_BASE 0x00011060 // Dirección base del display 4
#define DISPLAY_5_BASE 0x00011070 // Dirección base del display 5
#define DISPLAY_6_BASE 0x00011080 // Dirección base del display 6
#define DISPLAY_7_BASE 0x00011090 // Dirección base del display 7
#define DISPLAY_8_BASE 0x000110A0 // Dirección base del display 8
#define BUZZER_BASE 0x000110B0 // Dirección base del buzzer (ajustar real)
typedef struct {
	int hora;
    int minuto;
    int segundo;
} Tiempo;
Tiempo alarma_inicio = {0, 0, 0};
Tiempo alarma_fin = {0, 0, 0};
int alarma_activada = 0;
int minutos_restantes = 0;
int sonido(){
	clock_t start_time = clock();
    while (clock() < start_time + 30 * CLOCKS_PER_SEC) {
        IOWR(BUZZER_BASE, 0, 1);
    }
    IOWR(BUZZER_BASE, 0, 0); 
}
int calcular_diferencia_minutos(Tiempo inicio, Tiempo fin) {
    int minutos_inicio = inicio.hora * 60 + inicio.minuto;
    int minutos_fin = fin.hora * 60 + fin.minuto;
    return minutos_fin - minutos_inicio;
}
int alarma(int hora_inicio, int minuto_inicio, int segundo_inicio, int hora_fin, int minuto_fin, int segundo_fin) {
	alarma_inicio.hora = hora_inicio;
    alarma_inicio.minuto = minuto_inicio;
    alarma_inicio.segundo = segundo_inicio;
    alarma_fin.hora = hora_fin;
    alarma_fin.minuto = minuto_fin;
    alarma_fin.segundo = segundo_fin;
	minutos_restantes = calcular_diferencia_minutos(alarma_inicio, alarma_fin);
	alarma_activada = 1;
	return 0;
}
int main()
{
	int hora_inicio, minuto_inicio, segundo_inicio;
    int hora_fin, minuto_fin, segundo_fin;
	unsigned char switch_alarma, boton_numero , boton_confirmar;
	time_t tiempo_actual;
	struct tm* tiempo_local;
	while(1)
	{
		switch_alarma = IORD(SWITCH_ALARMA_BASE, 0);
        boton_numero = IORD(BUTTON_NUMERO_BASE, 0);
		boton_confirmar = IORD(BUTTON_CONFIRMAR_BASE, 0);
		tiempo_actual = time(NULL);
		tiempo_local = localtime(&tiempo_actual);
		if(switch_alarma == 1){
			hora_inicio = 0, minuto_inicio = 0, segundo_inicio = 0;
   			hora_fin = 0, minuto_fin = 0, segundo_fin = 0;
			int i = 0;
			while(i<=5){
				if(boton_confirmar == 1){
					i++;
				}
				if(i == 0){
					if(hora_inicio == 24){
						hora_inicio = 0;
					}
				}else if(i == 1){
					if(minuto_inicio == 60){
						minuto_inicio = 0;
					}
				}else if(i == 2){
					if(segundo_inicio == 60){
						segundo_inicio = 0;
					}
				}else if(i == 3){
					if(hora_fin == 24){
						hora_fin = 0;
					}
				}else if(i == 4){
					if(minuto_fin == 60){
						minuto_fin = 0;
					}
				}else if(i == 5){
					if(segundo_fin == 60){
						segundo_fin = 0;
					}
				}else{
					alarma(hora_inicio, minuto_inicio, segundo_inicio, hora_fin, minuto_fin, segundo_fin);
					break;
				}
			}
		}
		if (alarma_activada == 1) {
            if (tiempo_local->tm_hour == alarma_inicio.hora && tiempo_local->tm_min == alarma_inicio.minuto && tiempo_local->tm_sec == alarma_inicio.segundo) {
                printf("Iniciando cuenta regresiva...\n");
            }
            if (tiempo_local->tm_hour == alarma_fin.hora && tiempo_local->tm_min == alarma_fin.minuto && tiempo_local->tm_sec == alarma_fin.segundo) {
                printf("Alarma activada, sonando...\n");
                sonido();
                alarma_activada = 0;
            }
        }
		printf("Hora actual: %02d:%02d:%02d\n", tiempo_local->tm_hour, tiempo_local->tm_min, tiempo_local->tm_sec);
		IOWR(DISPLAY_1_BASE, 0, tiempo_local->tm_hour / 10); // Primer dígito de la hora
        IOWR(DISPLAY_2_BASE, 0, tiempo_local->tm_hour % 10); // Segundo dígito de la hora
        IOWR(DISPLAY_3_BASE, 0, tiempo_local->tm_min / 10); // Primer dígito de los minutos
        IOWR(DISPLAY_4_BASE, 0, tiempo_local->tm_min % 10); // Segundo dígito de los minutos
        IOWR(DISPLAY_5_BASE, 0, tiempo_local->tm_sec / 10); // Primer dígito de los segundos
        IOWR(DISPLAY_6_BASE, 0, tiempo_local->tm_sec % 10); // Segundo dígito de los segundos
		clock_t start_time = clock();
        while (clock() < start_time + CLOCKS_PER_SEC);
	}
	return 1;
}
