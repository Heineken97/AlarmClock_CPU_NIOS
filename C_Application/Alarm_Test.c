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
int sonido() {
    printf("Sonido\n");
    return 0;
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
int main() {
    //PONER EXACTOS RESPECTO A LA HORA REAL
    int hora_inicio = 00, minuto_inicio = 46, segundo_inicio = 1;
    int hora_fin = 00, minuto_fin = 47, segundo_fin = 1;
    alarma(hora_inicio, minuto_inicio, segundo_inicio, hora_fin, minuto_fin, segundo_fin);
    printf("Alarma configurada: Inicio %02d:%02d:%02d, Fin %02d:%02d:%02d\n",
           alarma_inicio.hora, alarma_inicio.minuto, alarma_inicio.segundo,
           alarma_fin.hora, alarma_fin.minuto, alarma_fin.segundo);
    while (1) {
        // Obtiene la hora actual del sistema
        time_t tiempo_actual = time(NULL);
        struct tm* tiempo_local = localtime(&tiempo_actual);
        // Simulación del encendido de la alarma
        if (alarma_activada == 1) {
            if (tiempo_local->tm_hour == alarma_inicio.hora &&
                tiempo_local->tm_min == alarma_inicio.minuto &&
                tiempo_local->tm_sec == alarma_inicio.segundo) {
                printf("Iniciando cuenta regresiva...\n");
            }
            if (tiempo_local->tm_hour == alarma_fin.hora &&
                tiempo_local->tm_min == alarma_fin.minuto &&
                tiempo_local->tm_sec == alarma_fin.segundo) {
                printf("Alarma activada, sonando...\n");
                sonido();
                alarma_activada = 0;
            }
        }
        printf("Hora actual: %02d:%02d:%02d\n", tiempo_local->tm_hour, tiempo_local->tm_min, tiempo_local->tm_sec);
        sleep(1); // Espera de 1 segundo para simular el paso del tiempo real
    }
    return 1;
}
