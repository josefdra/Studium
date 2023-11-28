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
   
   Koffer nicht benutzen: 7, 17, 13
   Koffer geht: 19
*/

volatile uint8_t safe;
volatile uint8_t var;
volatile uint8_t helper;
volatile uint8_t var_old;


void change_var(){
	var++;
	startTimer(0);
}

void check_buttons_and_overflow(){	
	if(~PINC & 0x01){
		cli();
		cancelTimer(0);
		safe = (PIND & 0x03);
		safe |= ((PIND & 0x20) >> 3);
		var = safe;
		PORTB = 0x07;
		sei();
	}
	if(~PIND & 0x04){
		startTimer(0);		
	}
	if (timerRunning(0) && var != var_old){		
		helper &= 0xF8;
		helper |= var;
		PORTB = helper;
		helper = PORTB;
		var_old = var;		
	}
	if(var == 8){
		var, var_old = safe;
	}
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
	TCNT1 = 65285;
	TCCR1B &= ~((1<<CS12)|(1<<CS11)|(1<<CS10));
	TCCR1B |= (1<<CS11)|(1<<CS10);
	TIMSK1 |= (1<<TOIE1);
	sei();
	safe = (PIND & 0x03);
	safe |= ((PIND & 0x20) >> 3);
	var, var_old = safe;
	declareTimer(change_var, 1000, 0);
	declareTimer(check_buttons_and_overflow, 500, 1);
	startTimer(1);
	
	while(1)
	{
		
	}
}