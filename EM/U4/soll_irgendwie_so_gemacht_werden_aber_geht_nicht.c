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
#include "util/delay.h"


/* TTL RX - Arduino TX
   TTL TX - Arduino RX
*/

char ring_buffer[BUFFER_SIZE];
uint8_t isEmpty = 1;
uint8_t rxHead = 0;
uint8_t rxTail = 0;
uint8_t receivedChars = 0;
uint8_t flow_control_flag = 0;
void USART_Init(){
	UBRR0H = (BAUD_CONST >> 8);
	UBRR0L = BAUD_CONST;
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0) | (1<<RXCIE0);
}

void USART_Transmit(unsigned char data){
	while(!(UCSR0A & (1<<UDRE0)));
	if(receivedChars ==  BUFFER_SIZE && !flow_control_flag){
		flow_control_flag = 1;
		UDR0 = 17; //XON
		receivedChars = 0;
	}
	else if (receivedChars == (BUFFER_SIZE - 8) && flow_control_flag){
		flow_control_flag = 0;
		UDR0 = 19; //XOFF
	}
	UDR0 = data;	
}

unsigned char USART_Receive(){
	while(!(UCSR0A & (1<<RXC0)));
	if (rxTail != rxHead && flow_control_flag){
		UDR0 = ring_buffer[rxTail];
		rxTail = (rxTail + 1) % BUFFER_SIZE;
		
	}
	return UDR0;
}

ISR(USART_RX_vect){
	char receivedChar = UDR0;
	uint8_t nextHead = (rxHead + 1) % BUFFER_SIZE;
	
	if (nextHead != rxTail){
		ring_buffer[rxHead] = receivedChar;
		rxHead = nextHead;
		receivedChars++;
	}
	
}

int main(void)
{
	sei();
	USART_Init();
	
	
	while(1)
	{
		char d = USART_Receive();
		USART_Transmit(d);
		_delay_ms(200);
	}
}