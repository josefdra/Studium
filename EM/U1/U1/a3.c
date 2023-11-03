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

	uint8_t var = ~0x07;
	DDRB = 0xFF;
	uint8_t helper;
    while(1)
    {
		helper = PORTB;
		var = ~var;
		if (var != 0)
		{
			var -= 1;
		}
		else
		{
			var = 0x07;
		}
		var = ~var;

		helper = helper >> 3;
		helper = helper << 3;
		helper ^= var;
		PORTB = helper;

		_delay_ms(1000);

        //TODO:: Please write your application code
    }
}
