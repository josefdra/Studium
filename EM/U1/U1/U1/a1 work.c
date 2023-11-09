/*
 * GccApplication1.c
 *
 * Created: 13.10.2023 08:38:26
 *  Author: drj45115
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

	int var = 0x08;
	int DDRB = 0xFF;
	int helper = PORTB;

    while(1)
    {
		if (var != 0)
		{
			var -= 1;
		}
		else
		{
			var = 0x07;
		}

		helper &= 0xF8;
		helper |= var;
		PORTB = helper;

		_delay_ms(1000);
    }
}
