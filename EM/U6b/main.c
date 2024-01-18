/*
 * GccApplication1.c
 *
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
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include "uart.h"

uint8_t state = 0;
uint16_t adc;
char sreg;
uint32_t output;
uint16_t dutyCycle;
uint16_t icri;
uint8_t dutyPercentage;
char c[8];

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
	var -= 289;
	sprintf(c, "%d", var);
	for(int f=0; c[f] != '\0'; f++)
		USART_Transmit(c[f]);
	USART_Transmit('C');
}

void pot(uint16_t var){
	var *= 5;
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
		adc = ADC;
		temp(adc);
		} else{
		state = 0;
		ADMUX &= ~((1<<REFS1)|(1<<MUX3));
		ADMUX |= (1<<MUX0);
		_delay_ms(5);
		ADCSRA |= (1 << ADSC);
		while(ADCSRA & 0x40);
		adc = ADC;
		pot(adc);
	}
	USART_Transmit('\n');
}

void led_control(){	
	output = ADC;
	dutyCycle = (((output * 1000) / 1023) * 255) / 1000;
	OCR0B = dutyCycle;
}

void duty(){
	icri = 1000 - icri; // Umkehrung weil wir wissen wollen wie lange die LED leuchtet
	dutyPercentage = icri / 10; // icri kann maximal den Wert 1000 annehmen was 100% entspräche
	sprintf(c, "%d", dutyPercentage);
	for(int f=0; c[f] != '\0'; f++)
		USART_Transmit(c[f]);
	USART_Transmit('%');
}

ISR(TIMER1_OVF_vect){ // hinzufügen
	sreg = SREG;
	cli();
	icri = ICRIL | (ICRIH<<8);
	sei();
	SREG = sreg;
	TCNT1 = 0;
}

int main(void)
{
	TCCR0A |= (1<<WGM00)|(1<<WGM01)|(1<<COM0B1)|(1<<COM0B0);
	TCCR0B |= (1<<WGM02)|(1<<CS01)|(1<<CS00);
	OCR0A = 249;	
	TCCR1B |= (1<<CS11)|(1<<CS10); // Prescaler von Timer 1 auch auf 64
	TIMSK1 |= (1<<ICIE1); // Interrupt für Veränderung an ICP1 aktivieren
	sei();
	PRR &= ~(1<<PRADC);
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1)| (1<<ADPS0);
	ADMUX |= (1<<REFS1)|(1<<REFS0)|(1<<MUX3);
	DDRD = (1<<PORTD5);
	USART_Init();
	const char start[] = "Wir haben diese Werte:\n";
	const char thermo[] = "Thermometer: ";
	const char pote[] = "Potentiometer: ";
	const char duty[] = "Wir haben einen Duty Cycle von: ";
	while(1)
	{		
		led_control(); // Doppelt um den Delay anstatt von 10ms auf 5ms zu reduzieren. Notwendig?
		setCursorPosition(1, 1);
		for(int f=0; start[f] != '\0'; f++)
			USART_Transmit(start[f]);
		for(int f=0; thermo[f] != '\0'; f++)
			USART_Transmit(thermo[f]);
		change_state();
		led_control(); // Doppelt um den Delay anstatt von 10ms auf 5ms zu reduzieren. Notwendig?
		for(int f=0; pote[f] != '\0'; f++)
			USART_Transmit(pote[f]);
		change_state();
		for(int f=0; duty[f] != '\0'; f++)
			USART_Transmit(duty[f]);
		duty();
		clearTerminal();
	}

}
