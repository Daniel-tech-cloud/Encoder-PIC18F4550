#include <xc.h>

// Definir pines del encoder
#define EncoderA PORTBbits.RB0
#define EncoderB PORTBbits.RB1

// Variables para guardar el estado anterior de las señales
int EncoderA_prev = 0;
int EncoderB_prev = 0;

// Contadores de pulsos
int PulsosA = 0;
int PulsosB = 0;

// Prototipo de función
void inicializar();

// Interrupción
void __interrupt() ISR(void);

void main() {
    inicializar();
    
    while(1) {
       
        // Verificar el estado de la interrupción de EncoderA
        if (EncoderA_prev == 0 && EncoderA == 1) {
            // Activar el puerto RA0 para EncoderA
            LATAbits.LATA0 = 1;
        } else {
            // Desactivar el puerto RA0 para EncoderA
            LATAbits.LATA0 = 0;
          }
            
        // Verificar el estado de la interrupción de EncoderB
        if (EncoderB_prev == 0 && EncoderB == 1) {
           // Activar el puerto RA1 para EncoderB
           LATAbits.LATA1 = 1;
        } else {
           // Desactivar el puerto RA1 para EncoderB
           LATAbits.LATA1 = 0;
        }

        // Limpiar la bandera de interrupción
        INTCONbits.RBIF = 0;
        
    }
}

void inicializar(){
    // Configurar puertos como entradas
    TRISBbits.TRISB0 = 1; // EncoderA
    TRISBbits.TRISB1 = 1; // EncoderB
    
    // Configurar interrupción por cambio de estado en PORTB
    INTCONbits.RBIE = 1; // Habilitar la interrupción por cambio de estado en PORTB
    INTCONbits.RBIF = 0; // Limpiar la bandera de interrupción inicialmente
    INTCON2bits.RBPU = 0; // Habilitar las resistencias de pull-up en PORTB
    
    // Habilitar interrupciones globales
    INTCONbits.GIE = 1;

    // Configurar puertos A como salidas
    TRISAbits.TRISA0 = 0; // RA0 para EncoderA
    TRISAbits.TRISA1 = 0; // RA1 para EncoderB
}

void __interrupt() ISR(void) {
    if (INTCONbits.RBIF) {
        // Guardar el estado actual de las señales
        int currentEncoderA = EncoderA;
        int currentEncoderB = EncoderB;
        
        // Verificar el estado anterior y actual para determinar la dirección del giro
        if (EncoderA_prev == 0 && EncoderB_prev == 0) {
            if (currentEncoderA == 1 && currentEncoderB == 0) {
                // Giro a la derecha
                PulsosA++;
            } else if (currentEncoderA == 0 && currentEncoderB == 1) {
                // Giro a la izquierda
                PulsosB++;
            }
        }
        
        // Actualizar el estado anterior
        EncoderA_prev = currentEncoderA;
        EncoderB_prev = currentEncoderB;
        
        // Limpiar la bandera de interrupción
        INTCONbits.RBIF = 0;
    }
}

