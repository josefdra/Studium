/*
 * GccApplication1.c
 *
 * Created: 13.10.2023 08:38:26
 *  Author: drj45115
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

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

ISR(PCINT1_vect){
	if(~PINC & 0x01){
		PORTB |= 0x07;
		start_pressed = 0;
	}
	if(~PINC & 0x02){		
		var = safe;
		PORTC |= 0x02;
	}
}

ISR(INT0_vect){
	safe = (PIND & 0x03);
	safe |= ((PIND & 0x20) >> 3);
	var = safe;
	start_pressed = 1;
}

int main(void)
{
	PORTD |= (1<<PORTD0)|(1<<PORTD1)|(1<<PORTD2)|(1<<PORTD5);
	PORTC |= 0x03;
	DDRC = 0x02;
	helper = PORTB;
	DDRB = 0x0F;
	PORTB |= 0x0F;
	EICRA &= ~((1<<ISC01)|(1<<ISC00));
	EICRA |= (1<<ISC01);
	EIMSK |=(1<<INT0);
	PCMSK1 |= 0x03;
	PCICR |= 0x02;
	sei();
	safe = (PIND & 0x03);
	safe |= ((PIND & 0x20) >> 3);
	var = safe;
	
	while(1)
	{
		if(start_pressed)
		{
			while(var < 8)
			{
				if(!start_pressed)
					break;
				helper &= 0xF8;
				helper |= var;
				PORTB = helper;
				helper = PORTB;
				_delay_ms(1000);
				var++;
			}
			PORTC &= ~(1<<PORTC1);
		}
	}
}