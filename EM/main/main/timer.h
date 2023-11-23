/*
 * timer.h
 *
 * Created: 17.11.2023 12:47:54
 *  Author: drj45115
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "event.h"

uint16_t counter_intern = 0;

static uint16_t timer_event[] = {
	0x0010,
	0x0020,
	0x0040,
	0x0080,
	0x0100
};

struct s_timer
{
	uint16_t duration;
	void (*callback)();
	char multiply;
	uint16_t counter;
	char running;
};

volatile struct s_timer timers[5];

void declareTimer(void (*cb)(), uint16_t d, uint8_t t_n){
	timers[t_n].duration = d;
	timers[t_n].callback = cb;
	timers[t_n].counter = 0;
	timers[t_n].running = 0;
}

void declareTimer_milli(void (*cb)(), uint16_t d, uint8_t t_n){
	declareTimer(cb, d, t_n);
	timers[t_n].multiply = 1;
}

void declareTimer_mikro(void (*cb)(), uint16_t d, uint8_t t_n){
	declareTimer(cb, d, t_n);
	timers[t_n].multiply = 0;
}

void startTimer(uint8_t t_n){
	timers[t_n].running = 1;
}

void cancelTimer(uint8_t t_n){
	timers[t_n].running = 0;
}

ISR(TIMER1_OVF_vect){
	for(int i = 0; i < 5; i++){
		if(timers[i].running == 1){
			if(timers[i].multiply == 1){
				counter_intern++;
				if(counter_intern == 1000){
					timers[i].counter++;
					if(timers[i].counter == timers[i].duration){
						cli();
						timers[i].counter = 0;
						timers[i].running = 0;
						setEvent(timer_event[i]);
						sei();
					}
					counter_intern = 0;
				}
			} else {
				timers[i].counter++;
				if(timers[i].counter == timers[i].duration){
					cli();
					timers[i].counter = 0;
					timers[i].running = 0;
					setEvent(timer_event[i]);
					sei();
				}
			}
		}
	}
}

#endif /* TIMER_H_ */