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

void startTimer(uint8_t t_n){
	timers[t_n].running = 1;
}

void cancelTimer(uint8_t t_n){
	timers[t_n].running = 0;
}

int timerRunning(uint8_t t_n){
	return timers[t_n].running;
}

ISR(TIMER1_OVF_vect){
	TCNT1 = 65285;
	for(int i = 0; i < 5; i++){
		if(timers[i].running == 1){
			timers[i].counter++;
			if(timers[i].counter == timers[i].duration){
				cli();
				timers[i].counter = 0;
				timers[i].running = 0;
				if (timers[i].callback!=NULL){
					sei();
					timers[i].callback();
				}
			} 
		}
	}
}

#endif /* TIMER_H_ */