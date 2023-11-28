/*
 * test.c
 *
 * Created: 28.11.2023 12:11:10
 *  Author: drj45115
 */ 

#define F_CPU 16000000UL

#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>



int main(void)
{
	DDRB = 0x0F;
	PORTB ^= 0x07;
    while(1)
    {
        for(uint16_t i = 0; i<65000; i++){
			for(uint16_t j = 0; j<65000; j++){
				PORTB ^= 0x07;
			}
		}
    }
}