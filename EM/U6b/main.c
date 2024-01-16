 * Created: 13.10.2023 08:38:26
 *  Author: drj45115
 */

/*
PD5 - OC0B
PD6 - OC0A
*/

#define F_CPU 16000000UL
#define BAUDRATE 9600
#define BAUD_CONST (((F_CPU/(BAUDRATE*16UL)))-1)
#define BUFFER_SIZE 32
#include <stdint.h>
#include <avr/io.h>

int main(void)
{
	TCCR0A |= (1<<COM0A1)|(1<<COM0A0)|(1<<WGM00)|(1<<WGM01);
	TCCR0B |= (1<<WGM02)|(1<<CS01)|(1<<CS00);
	OCR0A = 249;
	DDRB = 0x01;
	DDRD = 0x40;
	PORTB &= 0x0;
	PORTB |= 0x01;
	uint32_t c = 0;
	while(1)
	{	
		if(TIFR0 & ~(1<<OCF0B)){
			c += 1;
			TIFR0 |= (1<<OCF0B);
		}
		if(c == 800000){
			c = 0;
			PORTB &= ~0x01;
		}
	}
}