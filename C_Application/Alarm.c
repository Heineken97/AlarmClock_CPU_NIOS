#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "io.h"
//#define CLOCKS_PER_SEC 1000000
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
Tiempo hora_actual = {0, 0, 0}; // Nueva estructura para la hora actual

int alarma_activada = 0;
int minutos_restantes = 0;
int sonido() {
    // Comparación de la hora actual con la hora de la alarma
    int minutos_actuales = hora_actual.hora * 60 + hora_actual.minuto;
    int minutos_inicio = alarma_inicio.hora * 60 + alarma_inicio.minuto;
    int minutos_fin = alarma_fin.hora * 60 + alarma_fin.minuto;

    if (minutos_actuales >= minutos_inicio && minutos_actuales < minutos_fin) {
        IOWR(BUZZER_BASE, 0, 1); // Activar el buzzer
        printf("Alarma sonando...\n");
    } else {
        IOWR(BUZZER_BASE, 0, 0); // Desactivar el buzzer
    }

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
void incrementar_hora_actual() {
    hora_actual.segundo++;
    if (hora_actual.segundo >= 60) {
        hora_actual.segundo = 0;
        hora_actual.minuto++;
        if (hora_actual.minuto >= 60) {
            hora_actual.minuto = 0;
            hora_actual.hora++;
            if (hora_actual.hora >= 24) {
                hora_actual.hora = 0;
            }
        }
    }
}

void configurar_hora_actual() {
    int i = 0;
    int hora_temp = 0, minuto_temp = 0, segundo_temp = 0;
    unsigned char boton_numero, boton_confirmar;

    while (i <= 2) {
        boton_numero = IORD(BUTTON_NUMERO_BASE, 0);
        boton_confirmar = IORD(BUTTON_CONFIRMAR_BASE, 0);
        if (boton_confirmar == 1) {
            i++;
        }
        if (i == 0) {
            hora_temp = boton_numero;
            if (hora_temp >= 24) {
                hora_temp = 0;
            }
        } else if (i == 1) {
            minuto_temp = boton_numero;
            if (minuto_temp >= 60) {
                minuto_temp = 0;
            }
        } else if (i == 2) {
            segundo_temp = boton_numero;
            if (segundo_temp >= 60) {
                segundo_temp = 0;
            }
        }
    }
    hora_actual.hora = hora_temp;
    hora_actual.minuto = minuto_temp;
    hora_actual.segundo = segundo_temp;

    printf("Hora actual configurada: %02d:%02d:%02d\n", hora_actual.hora, hora_actual.minuto, hora_actual.segundo);
}
int main() {
    int hora_inicio, minuto_inicio, segundo_inicio;
    int hora_fin, minuto_fin, segundo_fin;
    unsigned char switch_alarma;

    configurar_hora_actual(); // Configuración inicial de la hora

    while (1) {
        switch_alarma = IORD(SWITCH_ALARMA_BASE, 0);

        if (switch_alarma == 1) {
            // Configurar la alarma de manera similar a la configuración de la hora actual
            alarma(hora_inicio, minuto_inicio, segundo_inicio, hora_fin, minuto_fin, segundo_fin);
            printf("Alarma configurada: Inicio %02d:%02d:%02d, Fin %02d:%02d:%02d\n",
                   alarma_inicio.hora, alarma_inicio.minuto, alarma_inicio.segundo,
                   alarma_fin.hora, alarma_fin.minuto, alarma_fin.segundo);
        }

        incrementar_hora_actual(); // Incrementar la hora actual

        if (alarma_activada == 1) {
            sonido(); // Verificar si debe sonar la alarma
        }

        printf("Hora actual: %02d:%02d:%02d\n", hora_actual.hora, hora_actual.minuto, hora_actual.segundo);

        IOWR(DISPLAY_1_BASE, 0, hora_actual.hora / 10); // Primer dígito de la hora
        IOWR(DISPLAY_2_BASE, 0, hora_actual.hora % 10); // Segundo dígito de la hora
        IOWR(DISPLAY_3_BASE, 0, hora_actual.minuto / 10); // Primer dígito de los minutos
        IOWR(DISPLAY_4_BASE, 0, hora_actual.minuto % 10); // Segundo dígito de los minutos
        IOWR(DISPLAY_5_BASE, 0, hora_actual.segundo / 10); // Primer dígito de los segundos
        IOWR(DISPLAY_6_BASE, 0, hora_actual.segundo % 10); // Segundo dígito de los segundos

        // Espera de un segundo simulado
        for (volatile int i = 0; i < 1000000; i++);
    }

    return 1;
}
