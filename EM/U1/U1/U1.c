/*
 * GccApplication1.c
 *
 * Created: 13.10.2023 08:38:26
 *  Author: kon33382
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

/* PORTB2 = A1
   PORTB1 = A4
   PORTB0 = A7
*/

int main(void)
{

	DDRB |= (1<<DDB1);
	PORTB |= (1<<PORTB1)|(1<<PORTB0);
	uint8_t pressed = 0;
    while(1 && !pressed)
    {
		if (PINB & (1<<PINB0))
		{
			PORTB |= (1<<PORTB1);
			pressed = 1;
		}
		else
			PORTB &= ~(1<<PORTB1);
		_delay_ms(1000);

		pressed = 0;

        //TODO:: Please write your application code
    }
}
