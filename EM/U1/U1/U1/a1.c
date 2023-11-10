/*
 * GccApplication1.c
 *
 * Created: 13.10.2023 08:38:26
 *  Author: drj45115
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

/* PORTB2 = A1
   PORTB1 = A4
   PORTB0 = A7
*/

int main(void){

	uint8_t var = 0x00;
	DDRB = 0x07;
	uint8_t helper = PORTB;

    while(1)
    {
		for(var = 0; var < 8; var++){
			helper &= 0xF8;
			helper |= var;
			PORTB = helper;

			_delay_ms(1000);
		}
    }
}
