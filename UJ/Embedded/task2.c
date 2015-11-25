/**
 * Autor Anna Platash
 */

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

ISR(INT0_vect) {
    int licznik = 0;
    licznik++;
	if(licznik >= 0b10000000) {
		licznik = 0;
	}
    PORTB = licznik;
}


void blink() {
    while(1){
        PORTB^=0b10000000;
        _delay_ms(500);
    }
}

int main(void) {
    MCUCR = (1 << ISC01);
    GICR = (1 << INT0);
    sei();
    DDRB = 0xFF;
    DDRD = 0x00;       
    PORTD = 0b00000100;
    blink();
}
