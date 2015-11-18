/* 
 * Author: Anna Platash
 *
 */

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>


int main(void){
   
    DDRB = 0x00;
    PORTB = 0b00000001;
    DDRA = 0xFF;

    while(1){
        int x = 1000;
        for (int i=10; i<1000; i++){
                _delay_us(x);
                PORTA = 0xFF;
                _delay_us(x - i);
                PORTA = 0x00;

        }
    }
        
}

int snake(void){
    while(1) {
        if(PINB & 0x01){
                PORTA = 0x00;
        } else {
            for(int i = 0; i < 8; i++) {
                PORTA = 1 << i;
                _delay_ms(50);
            }
        }

    }
}

int button(void) {
    while(1){
        if(PINB & 0x01) {
            PORTA = 0b00000000;

            } else {
            PORTA = 0b00001111;
        }
    }
} 
