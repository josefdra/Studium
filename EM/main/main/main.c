/*
 * GccApplication1.c
 *
 * Created: 13.10.2023 08:38:26
 *  Author: drj45115
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "event.h"
#include "timer.h"

/* PORTB2 = A1
   PORTB1 = A4
   PORTB0 = A7

   PORTD0 = A5
   PORTD1 = A6
   PORTD5 = A7
   Analog A0 = A3
   PORTD2 = A4
*/

volatile uint8_t safe;
volatile uint8_t var;
volatile uint8_t helper;
volatile uint8_t start_pressed = 0;
volatile uint8_t var_old;

void change_var(){
	var++;
	startTimer(0);
}

void check_buttons_and_overflow(){	
	if(~PINC & 0x01){
		safe = (PIND & 0x03);
		safe |= ((PIND & 0x20) >> 3);
		var = safe;
		PORTB = 0x07;
	}
	if(~PIND & 0x04){
		if(var != var_old){
			helper &= 0xF8;
			helper |= var;
			PORTB = helper;
			helper = PORTB;
			var_old = var;
		}		
	}
	if(var == 8){
		var = safe;
	}
	var_old = var;
	startTimer(1);
}

int main(void)
{
	PORTD |= (1<<PORTD0)|(1<<PORTD1)|(1<<PORTD2)|(1<<PORTD5);
	PORTC |= 0x03;
	helper = PORTB;
	PORTB |= 0x0F;
	DDRC = 0x02;	
	DDRB = 0x0F;	
	TCCR1B &= ~((1<<CS02)|(1<<CS01)|(1<<CS00));
	TCCR1B |= (1<<CS11);
	TIMSK1 |= (1<<TOIE0);
	TCNT1 = 254;
	sei();
	safe = (PIND & 0x03);
	safe |= ((PIND & 0x20) >> 3);
	var = safe;
	declareTimer_milli(change_var, 1000, 0);
	declareTimer_milli(check_buttons_and_overflow, 5, 1);
	startTimer(0);
	startTimer(1);
	
	while(1)
	{
		
	}
}