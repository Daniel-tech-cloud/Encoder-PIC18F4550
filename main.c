#include <xc.h>
#include <stdint.h>

// Definiciones de pines
#define ENCODER_A_PIN RB0
#define ENCODER_B_PIN RB1

// Variables globales
volatile int16_t contador_total = 0;
volatile uint16_t giros_derecha = 0;
volatile uint16_t giros_izquierda = 0;
volatile int16_t angulo_volante = 0;

// Límites del volante
// #define ANGULO_MAXIMO 180
// #define ANGULO_MINIMO -180

// Prototipos de funciones
void inicializar();
void  SPI_Init(); 

// Interrupción
void __interrupt() ISR();


void main() {
    inicializar(); // Iniciaiza configuración entradas y salidas de PIC 
    SPI_Init(); // Inicializa la interfaz SPI

    while(1) {
        PORTA = contador_total;

    }
}

void inicializar() {
    // Configurar puertos como entradas
    TRISB = 0b00000011; // RB0 y RB1 como entradas

    // Configurar puerto A como salidas
    TRISA = 0x00;

    // Configurar interrupciones
    INTCONbits.GIE = 1;   // Habilitar interrupciones globales
    INTCONbits.INT0IE = 1; // Habilitar interrupción INT0 (RB0)
    INTCON2bits.INTEDG0 = 0; // Interrupción en flanco de bajada para INT0
    INTCON3bits.INT1IE = 1; // Habilitar interrupción INT1 (RB1)
    INTCON2bits.INTEDG1 = 0; // Interrupción en flanco de bajada para INT1
}


void __interrupt() ISR() {
    if(INTCONbits.INT0IF) { // Verificar si la interrupción es de RB0
        if(ENCODER_B_PIN == 1) {
            contador_total++; // Sentido horario
            giros_derecha++;
        } else {
            contador_total--; // Sentido antihorario
            giros_izquierda++;
        }
        INTCONbits.INT0IF = 0; // Limpiar bandera de interrupción INT0
    }

    if(INTCON3bits.INT1IF) { // Verificar si la interrupción es de RB1
        if(ENCODER_A_PIN == 0) {
            contador_total++; // Sentido horario
            giros_derecha++;
        } else {
            contador_total--; // Sentido antihorario
            giros_izquierda++;
        }
        INTCON3bits.INT1IF = 0; // Limpiar bandera de interrupción INT1
    }

    // // Limitar el rango de ángulo del volante
    // angulo_volante = contador_total;

    // if (angulo_volante > ANGULO_MAXIMO) {
    //     angulo_volante = ANGULO_MAXIMO;
    //     contador_total = ANGULO_MAXIMO;
    // } else if (angulo_volante < ANGULO_MINIMO) {
    //     angulo_volante = ANGULO_MINIMO;
    //     contador_total = ANGULO_MINIMO;
    // }
}