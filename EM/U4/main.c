/*
 * GccApplication1.c
 *
 * Created: 13.10.2023 08:38:26
 *  Author: drj45115
 */

#define F_CPU 16000000UL
#define BAUDRATE 9600
#define BAUD_CONST (((F_CPU/(BAUDRATE*16UL)))-1)
#define BUFFER_SIZE 32
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>


/* TTL RX - Arduino TX
   TTL TX - Arduino RX
*/

char ring_buffer[BUFFER_SIZE];
uint8_t isEmpty = 1;
uint8_t isr_iterator = 0;
uint8_t transmit_iterator = 0;
uint8_t flow_control_flag = 0;
void USART_Init(){
	UBRR0H = (BAUD_CONST >> 8);
	UBRR0L = BAUD_CONST;
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0) | (1<<RXCIE0);
}

void USART_Transmit(){
	while(!(UCSR0A & (1<<UDRE0)));
	if (transmit_iterator < 32 && flow_control_flag/*&& !isEmpty*/){
		UDR0 = ring_buffer[transmit_iterator];
		transmit_iterator++;
		if (transmit_iterator==31) {
			transmit_iterator= 0;
			//isEmpty = 1;
			UDR0 = 17; //XON
			flow_control_flag = 0;
		}
	}
	
}

unsigned char USART_Receive(){
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

ISR(USART_RX_vect){
	char receivedChar = UDR0;
	dfgdg
	if (isr_iterator < 32 /*&& isEmpty*/){
	ring_buffer[isr_iterator] = receivedChar;
	isr_iterator++;
	}
	
	if (isr_iterator == 31){
		//isEmpty = 0;
		isr_iterator = 0;
		UDR0 = 19; //XOFF
		flow_control_flag = 1;
	}

}

int main(void)
{
	sei();
	USART_Init();
	
	
	while(1)
	{
		USART_Transmit();
	}
}