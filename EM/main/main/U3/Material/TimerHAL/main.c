/*
 * TimerHAL.c
 *
 * Created: 03.03.2020 09:03:58
 * Author : mea39511
 */ 

#define F_CPU 16000000UL

#define EVENT_TIMER1SEC 0x01

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "events.h"
#include "TimerHAL.h"


void toggleLED() {
	PORTB ^= (1<<PORTB1);
}

int main(void)
{
	DDRB |= (1<<PORTB1)|(1<<PORTB0);
	TimerHAL_register_callback(toggleLED);
	TimerHAL_init();
	sei();
	while (1)
	{
		_delay_ms(200);
		PORTB ^= (1<<PORTB0);
	}
}

