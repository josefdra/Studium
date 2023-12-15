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
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include "uart.h"
#include "event.h"
#include "timer.h"

#pragma GCC optimize("O0")

/* PORTB2 = A1
   PORTB1 = A4
   PORTB0 = A7

   PORTD0 = A5
   PORTD1 = A6
   PORTD5 = A7
   Analog A0 = A3
   PORTD2 = A4
   
   Koffer nicht benutzen: 7, 17, 13
   Koffer geht: 19
*/

uint8_t safe EEMEM = 3;
volatile uint8_t var;
volatile uint8_t helper;
const char startzeit_einstellen[] PROGMEM = "Startzeit einstellen (1) <1 Startzeit>, Timer starten (2), Timer stoppen (3)\n"; //in flash
const char startzeit_ausgeben[] PROGMEM = "eingestellte Startzeit: "; //in flash
const char gestoppte_zeit[] PROGMEM = "gestoppte Zeit: "; //in flash


void change_var(){
	var++;
	startTimer(0);
}

void check_buttons_and_overflow(){	
	if(var == 8){
		var = eeprom_read_byte(&safe);
	}
	if (timerRunning(0)){		
		helper &= 0xF8;
		helper |= var;
		PORTB = helper;
		helper = PORTB;
	}
	startTimer(1);
}

int main(void)
{
	
	PORTD |= (1<<PORTD0)|(1<<PORTD1)|(1<<PORTD2)|(1<<PORTD5);
	PORTC |= 0x03;
	helper = PORTB;
	PORTB |= 0x0F;
	DDRC = 0x02;
	DDRB = 0x0F;
	TCNT1 = 65285;
	TCCR1B &= ~((1<<CS12)|(1<<CS11)|(1<<CS10));
	TCCR1B |= (1<<CS11)|(1<<CS10);
	TIMSK1 |= (1<<TOIE1);
	sei();
	declareTimer(change_var, 1000, 0);
	declareTimer(check_buttons_and_overflow, 500, 1);
	startTimer(1);
	
	USART_Init();
	char c;
	char d[4];
	uint8_t while_count = 0;
	
	// Ctrl + L = 12
	while(1)
	{
		for(int f=0; pgm_read_byte(&startzeit_einstellen[f]) != '\0'; f++)
			USART_Transmit(pgm_read_byte(&startzeit_einstellen[f]));
		while (d[3] != 13 && d[1] != 13){
			c = USART_Receive();
			if (c == '\0'){
				continue;
			}
			else{
				d[while_count] = c;
				USART_Transmit(c);
				while_count++;
			}
		}
		if(d[0] == '1'){
			eeprom_write_byte(&safe, 7 - (d[2] - '0'));
			for(int f=0; pgm_read_byte(&startzeit_ausgeben[f]) != '\0'; f++)
				USART_Transmit(pgm_read_byte(&startzeit_ausgeben[f]));
			USART_Transmit(7 + '0' - eeprom_read_byte(&safe));
			USART_Transmit('\n');
		} else if(d[0] == '2'){
			var = eeprom_read_byte(&safe);
			startTimer(0);			
		} else if(d[0] == '3'){
			cancelTimer(0);
			PORTB = 0x07;
			for(int f=0; pgm_read_byte(&gestoppte_zeit[f]) != '\0'; f++)
				USART_Transmit(pgm_read_byte(&gestoppte_zeit[f]));
			USART_Transmit(7 + '0' - var);
			USART_Transmit('\n');
		}
		for(int i = 0; i<4; i++)
			d[i] = '\0';
		while_count = 0;
	}
}