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

char ring_buffer[BUFFER_SIZE] = {0};
uint8_t rxHead = 0;
uint8_t rxTail = 0;
uint8_t isEmpty = 1;
uint8_t received_chars = 0;

void USART_Init(){
	UBRR0H = (BAUD_CONST >> 8);
	UBRR0L = BAUD_CONST;
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0) | (1<<RXCIE0);
}

void USART_Transmit(unsigned char data){
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

unsigned char USART_Receive(){
	cli();
	char c;
	
	if(received_chars == 8)
		USART_Transmit(17); //XON
	c = ring_buffer[rxTail];
	if (c != '\0' ){
		ring_buffer[rxTail] = '\0';
		rxTail = (rxTail + 1) % BUFFER_SIZE;
		received_chars--;
	}
	sei();
	return c;
}

ISR(USART_RX_vect){
	char receivedChar = UDR0;
	uint8_t nextHead = (rxHead + 1) % BUFFER_SIZE;
	ring_buffer[rxHead] = receivedChar;
	rxHead = nextHead;
	received_chars++;
	if(received_chars == (BUFFER_SIZE - 8))
		USART_Transmit(19); //XOFF
}

int main(void)
{
	sei();
	USART_Init();
	char d;
	
	while(1)
	{
		d = USART_Receive();
		if (d == '\0'){
			continue;
		}
		else{
			USART_Transmit(d);
		}
		_delay_ms(200);
	}
}