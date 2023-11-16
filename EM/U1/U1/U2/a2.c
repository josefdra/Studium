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
   PORTD4 = A4
*/

uint8_t safe;
uint8_t var;
uint8_t helper;
volatile uint8_t start_pressed = 0;

ISR(PCINT1_vect){
	PORTB |= 0x07;
	start_pressed = 0;
}

ISR(INT0_vect){
	start_pressed = 1;
}

//Add Outport
ISR(){
	var = 0;
}

int main(void)
{
	PORTD |= 0x1F;
	PORTC |= 0x01;
	helper = PORTB;
	DDRB = 0x07;
	PORTB |= 0x07;
	EICRA &= ~((1<<ISC01)|(1<<ISC00));
	EICRA |= (1<<ISC01);
	EIMSK |=(1<<INT0);
	PCMSK1 |= 0x01;
	PCICR |= 0x02;
	sei();
	
	while(1)
	{		
		safe = (PIND & 0x03);
		safe |= ((PIND & 0x40) >> 3);
		if(start_pressed)
		{		
			var = 0;
			while(1)
			{			
				if(var = safe + 1)
					//Set Interrupt Port to 0 --> activate ISR --> var = 0
				if(!start_pressed)
					break;
				helper &= 0xF8;
				helper |= var;
				PORTB = helper;
				helper = PORTB;
				_delay_ms(1000);	
				var++;		
			}		
		}	
	}
}
