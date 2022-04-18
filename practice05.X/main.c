////+++++++++++++++++++++++++++++++++++++| LIBRARIES / HEADERS |+++++++++++++++++++++++++++++++++++++
#include "device_config.h"
#include <math.h>
//+++++++++++++++++++++++++++++++++++++| DIRECTIVES |+++++++++++++++++++++++++++++++++++++
#define _XTAL_FREQ 1000000
#define DELAY 100 //delay establecido para la "accion especial"

//+++++++++++++++++++++++++++++++++++++| DATA TYPES |+++++++++++++++++++++++++++++++++++++
enum por_dir{ output, input };              // output = 0, input = 1
enum por_ACDC { digital, analog };          // digital = 0, analog = 1
enum resistor_state { set_ON, res_ON };     // set_ON = 0, res_ON = 1
enum led_state { led_OFF, led_ON };         // led_OFF = 0, led_ON = 1
enum butto_state { pushed, no_pushed };     // pushed = 0, no_pushed = 1

////+++++++++++++++++++++++++++++++++++++| ISRs |+++++++++++++++++++++++++++++++++++++
//// ISR for high priority
//void __interrupt ( high_priority ) high_isr( void );
//// ISR for low priority
//void __interrupt ( low_priority ) low_isr( void ); 

//+++++++++++++++++++++++++++++++++++++| FUNCTION DECLARATIONS |+++++++++++++++++++++++++++++++++++++
void portsInit( void );

//+++++++++++++++++++++++++++++++++++++| MAIN |+++++++++++++++++++++++++++++++++++++
void main( void ){
    portsInit(); //Se hace la inicializacion de los puertos de la funcion
    char molePosition, buttonStatus = 0; //status del boton iniciado en cero
    while(1){
        unsigned char num = rand() % 8 ; //se imprime un numero al azar entre 0 y 7
        switch(num) { //funcion usada para intercambiar entre los LEDs de la tarjeta
            case 0:
                molePosition = 0x01; //Posicion definida en hexadecimal para el caso 0
                break;
            case 1:
                molePosition = 0x02;//Posicion definida en hexadecimal para el caso 1
                break;
            case 2:
                molePosition = 0x04;//Posicion definida en hexadecimal para el caso 2
                break;
            case 3:
                molePosition = 0x08;//Posicion definida en hexadecimal para el caso 3
                break;
            case 4:
                molePosition = 0x10;//Posicion definida en hexadecimal para el caso 4
                break;
            case 5:
                molePosition = 0x20;//Posicion definida en hexadecimal para el caso 5
                break;
            case 6:
                molePosition = 0x40;//Posicion definida en hexadecimal para el caso 6
                break;
            default:
                molePosition = 0x80;//Posicion definida en hexadecimal para el caso 7
                break;
        }
        LATB = molePosition; //Se escribe el valor de molePosition en el pin de PORTB
        __delay_ms(1000);
        buttonStatus = ~PORTD; //se nega el valor del puerto D debido a que el boton manda 1 al no estar presionado
        if (molePosition == buttonStatus) //ciclo para ejecutar la accion especial en caso de que el valor logico del boton presionado
            //coincida con el del LED encendido
        {
            for (unsigned char i = 0; i < 7; i++){ //ciclo para hacer el barrido desde el LED 0 hasta el 7
                LATB = (unsigned char) pow(2, i);
                __delay_ms(DELAY);
            }
            for (unsigned char i = 7; i > 0; i--){ //ciclo para hacer el barrido desde el LED 7 hasta el 0
                LATB = (unsigned char) pow(2, i);
                __delay_ms(DELAY);
            }   
            LATB = 0x01; //asignacion del valor de 1 a los puertos de B
            __delay_ms(DELAY);
        }
    }
}

//+++++++++++++++++++++++++++++++++++++| FUNCTIONS |+++++++++++++++++++++++++++++++++++++
void portsInit( void ){
    ANSELB = digital;                       // Set port B as Digital
    TRISB  = 0x00;                          // RB as Output for LEDs
    ANSELD = digital;                       // Set port D as Digital
    TRISD  = 0xFF;                          // RD as Input for push buttons

}