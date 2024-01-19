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

uint8_t state = 1;
uint16_t adc;
uint16_t icrr;
uint16_t icrf;
char sreg;
uint32_t output;
uint16_t dutyCycle;
uint32_t icrd;
uint32_t icrc;
uint16_t icrro = 0;
uint8_t duty_percentage;
char c[80];
char l[80];

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
		//state = 0;
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
	sprintf(c, "%d", duty_percentage);
	for(int f=0; c[f] != '\0'; f++)
		USART_Transmit(c[f]);
	USART_Transmit('%');
}

ISR(TIMER1_CAPT_vect){ 
	if (TCCR1B & (1<<ICES1)){
		icrr = ICR1;
	} else {
		icrf = ICR1;
		if (icrr < icrf){
			icrd = icrf - icrr;
			} else {
			icrd = (0xFFFF - icrr) + icrf;
		}
		if (icrro < icrr){
			icrc = icrr - icrro;
			} else {
			icrc = (0xFFFF - icrro) + icrr;
		}
		if (icrc == 500){
			icrc = 250;
		}
		duty_percentage = 100 - ((icrd * 100) / icrc);
		icrro = icrr;
	}
	TCCR1B ^= (1<<ICES1);
}

int main(void)
{
	for(int i = 0; i < 80; i++){
		l[i] = ' ';
	}
	TCCR0A |= (1<<WGM00)|(1<<WGM01)|(1<<COM0B1)|(1<<COM0B0); 
	TCCR0B |= (1<<WGM02)|(1<<CS01)|(1<<CS00);
	OCR0A = 249; 
	TCCR1B |= (1<<CS11)|(1<<CS10);
	TCCR1B &= ~(1<<ICES1);
	TIMSK1 |= (1<<ICIE1); 
	sei();
	PRR &= ~(1<<PRADC);
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1)| (1<<ADPS0);
	ADMUX |= (1<<REFS1)|(1<<REFS0)|(1<<MUX3);
	DDRD = (1<<PORTD5);
	USART_Init();
	const char start[] = "Wir haben diese Werte:\n";
	const char thermo[] = "Thermometer: ";
	const char pote[] = "Potentiometer: ";
	const char duty_text[] = "Wir haben einen Duty Cycle von: ";
	while(1)
	{		
		setCursorPosition(4,32);
		USART_Transmit(' ');
		USART_Transmit(' ');
		USART_Transmit(' ');
		USART_Transmit(' ');
		USART_Transmit(' ');
		USART_Transmit(' ');
		USART_Transmit(' ');
		USART_Transmit(' ');
		USART_Transmit(' ');
		USART_Transmit(' ');
		led_control(); 
		setCursorPosition(1, 1);		
		for(int f=0; start[f] != '\0'; f++)
			USART_Transmit(start[f]);
		for(int f=0; thermo[f] != '\0'; f++)
			USART_Transmit(thermo[f]);		
		change_state();		
		for(int f=0; pote[f] != '\0'; f++)
			USART_Transmit(pote[f]);		
		change_state();		
		for(int f=0; duty_text[f] != '\0'; f++)
			USART_Transmit(duty_text[f]);		
		duty();
		for(int f=0; l[f] != '\0'; f++)
			USART_Transmit(l[f]);
	}

}
