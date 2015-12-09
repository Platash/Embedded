/*
 * Author : Anna Platash
 */ 

#define F_CPU 16000000UL  // 16 MHz
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>


#define KEYBOARD_PORT DDRA
#define KEYBOARD_DDR DDRA
#define KEYBOARD_PIN PINA
#define LED_PORT PORTD
#define LED_DDR DDRD
#define LED_AUX_PORT PORTC
#define LED_AUX_DDR DDRC
#define TASK_SIZE 2
#define INVALID_VALUE 0xFFFFFFFF

typedef struct {
	void (*foo)();
	uint32_t timeout;
	uint32_t timetogo;
	uint32_t ready;
} TASK;

TASK TASK_TABLE[TASK_SIZE];
uint8_t NUMBERS[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x5E, 0b01111100, 0b01001110, 0x77};
volatile NUMBERS_TO_SHOW [] = {0x5E, 0b01111100, 0b01001110, 0x77};
	
ISR(TIMER0_COMP_vect) {
	int i;
	for (i = 0; i < TASK_SIZE; i++)
	{
		if (0 == TASK_TABLE[i].timetogo)
		{
			if (TASK_TABLE[i].timeout != INVALID_VALUE)
			{
				TASK_TABLE[i].ready++;
				TASK_TABLE[i].timetogo = TASK_TABLE[i].timeout;
			}
		}
		else
		{
			TASK_TABLE[i].timetogo--;
		}

	}
}

int schedule() {
	while (1)
	{
		uint8_t i;
		for(i = 0; i < TASK_SIZE; ++i) {
			if(TASK_TABLE[i].ready != 0x00) {
				TASK_TABLE[i].foo();
				TASK_TABLE[i].ready--;
				break;
			}
		}
	}
	return 1234;
}

void addTask(void (*f)(),uint32_t timeout, uint32_t isPeriodic, uint8_t priority)
{
	if(priority<TASK_SIZE){
		TASK_TABLE[priority].foo=f;
		TASK_TABLE[priority].timetogo=timeout;
		TASK_TABLE[priority].ready=0;
		TASK_TABLE[priority].timeout=(isPeriodic)?timeout:INVALID_VALUE;
	}
}

void light_led() {
	uint8_t i;
	for(i=0; i<3; i++) {
		LED_AUX_PORT = (1<<i); 
		LED_PORT = NUMBERS_TO_SHOW[i];
		_delay_ms(5);
		PORTB=(0<<i); 
	}
}

void check_keyboard() {
	 uint8_t row, col;
	 KEYBOARD_PORT|= 0X0F;
	 for(col = 0; col < 3; col++) {
		 KEYBOARD_DDR &= ~(0X7F);
		 _delay_ms(1);
		 KEYBOARD_DDR |= (0X40 >> col);
		 _delay_ms(1);
		 for(row=0; row<4; row++) {
			 if(!(KEYBOARD_PIN & (0X08 >> row))) {
				 uint8_t number = row * 3 + col;
				 NUMBERS_TO_SHOW[0] = NUMBERS[number - 1];
				 NUMBERS_TO_SHOW[0] = 0;
				 NUMBERS_TO_SHOW[0] = 0;
				 NUMBERS_TO_SHOW[0] = 0;
				 //return number;
			 }
		 }
	 }
	 //return 0XFF;
}

int main(void) {
	MCUCR |= (1 << ISC01) | (0 << ISC00);
	GICR |= (1 << INT0);
	TCCR0 |=(1 << WGM01);
	TCCR0 |=((1 << CS02)|(1 << CS00));
	OCR0 = 100;
	TIMSK |=(1 << OCIE0); 
	sei();
	
	KEYBOARD_DDR = 0xF0;
	_delay_ms(1);
	KEYBOARD_PORT = 0x0F;
	_delay_ms(1);
	LED_DDR = 0xFF;
	LED_AUX_DDR = 0xFF;
	LED_AUX_PORT = 0x00;
	addTask(&check_keyboard, 20, 1, 1);
	schedule();
	while (1)
	{
		light_led();
	}
}
