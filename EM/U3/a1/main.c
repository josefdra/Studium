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

ISR(TIMER0_OVF_vect){
	static uint16_t counter = 0;
	TCNT0 = 6;	
	counter++;
	if (counter==1000){
		counter = 0;
		setEvent(VAR_CHANGED);
	}
	if ((counter%50)==0){
		if(~PINC & 0x01){
			setEvent(RESET);
		}
		if(~PIND & 0x04){
			setEvent(START);
		}
		if(var == 8){
			setEvent(OVERFLOW);
		}
	}
}

int main(void)
{
	PORTD |= (1<<PORTD0)|(1<<PORTD1)|(1<<PORTD2)|(1<<PORTD5);
	PORTC |= 0x03;
	helper = PORTB;
	PORTB |= 0x0F;
	DDRC = 0x02;	
	DDRB = 0x0F;	
	TCNT0 = 6;
	TCCR0B &= ~((1<<CS02)|(1<<CS01)|(1<<CS00));
	TCCR0B |= (1<<CS01)|(1<<CS00);
	TIMSK0 |= (1<<TOIE0);
	sei();
	safe = (PIND & 0x03);
	safe |= ((PIND & 0x20) >> 3);
	var = safe;
	
	while(1)
	{
		if(eventIsSet(START))
		{		
			clearEvent(START);
			while(!eventIsSet(RESET)){				
				if(eventIsSet(VAR_CHANGED)){
					clearEvent(VAR_CHANGED);
					helper &= 0xF8;
					helper |= var;
					PORTB = helper;
					helper = PORTB;
					var++;
				}
				if (eventIsSet(OVERFLOW)){
					clearEvent(OVERFLOW);
					var = safe;
				}
			}
			clearEvent(RESET);
			safe = (PIND & 0x03);
			safe |= ((PIND & 0x20) >> 3);
			var = safe;
			PORTB = 0x07;
		}
	}
}