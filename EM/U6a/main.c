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
#include <stdio.h>
#include "uart.h"

uint8_t state = 0;

void clearTerminal() {
	USART_Transmit('\033');
	USART_Transmit('[');
	USART_Transmit('2');
	USART_Transmit('J');
}

void setCursorPosition(int row, int col) {
	USART_Transmit('\033');
	USART_Transmit('[');
	USART_Transmit(row / 10 + '0');
	USART_Transmit(row % 10 + '0');
	USART_Transmit(';');
	USART_Transmit(col / 10 + '0');
	USART_Transmit(col % 10 + '0');
	USART_Transmit('H');
}

void temp(uint16_t var){
	char c[8];
	var -= 289;
	sprintf(c, "%d", var);
	for(int f=0; c[f] != '\0'; f++)
		USART_Transmit(c[f]);
	USART_Transmit('C');
}

void pot(uint16_t var){
	var *= 5;
	var /= 1000;
	char c[8];
	sprintf(c, "%d", var);
	for(int f=0; c[f] != '\0'; f++)
		USART_Transmit(c[f]);
	USART_Transmit('m');
	USART_Transmit('V');
}

void change_state(){
	if(state == 0){
		state = 1;
		ADMUX &= ~(1<<MUX0);
		ADMUX |= (1<<REFS1)|(1<<MUX3);
		_delay_ms(5);
		ADCSRA |= (1 << ADSC);
		while(ADCSRA & 0x40);
		uint16_t adc = ADC;
		temp(adc);
	} else{
		state = 0;
		ADMUX &= ~((1<<REFS1)|(1<<MUX3));
		ADMUX |= (1<<MUX0);
		_delay_ms(5);
		ADCSRA |= (1 << ADSC);
		while(ADCSRA & 0x40);
		uint16_t adc = ADC;
		pot(adc);
	}
	USART_Transmit('\n');
}

int main(void)
{
	sei();
	
	//ADC aktivieren
	PRR &= ~(1<<PRADC);
	// Prescaling
	//ADPS 1 1 1 == 125kHz (16M / 128)
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1)| (1<<ADPS0);
	// Referenzspannung
	ADMUX |= (1<<REFS1)|(1<<REFS0)|(1<<MUX3);	
	USART_Init();
	const char start[] = "Wir haben diese Werte:\n";
	const char thermo[] = "Thermometer: ";
	const char pote[] = "Potentiometer: ";
	while(1)
	{	
		setCursorPosition(1, 1);
		for(int f=0; start[f] != '\0'; f++)
			USART_Transmit(start[f]);
		for(int f=0; thermo[f] != '\0'; f++)
			USART_Transmit(thermo[f]);
		change_state();		
		for(int f=0; pote[f] != '\0'; f++)
			USART_Transmit(pote[f]);
		change_state();
		_delay_ms(1000);
		clearTerminal();
	}
}