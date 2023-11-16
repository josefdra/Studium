/*
 * TimerHAL.h
 *
 * Created: 03.03.2020 10:04:40
 *  Author: mea39511
 */ 

#ifndef TIMERHAL_H_
#define TIMERHAL_H_

#include "TimerHAL_cfg.h"
#include "events.h"
#include <avr/io.h>
#include <stdlib.h>


#ifdef TIMER_IS_USED

#ifdef TIMER_USE_CALLBACK

void (*callback)()=NULL;

void TimerHAL_register_callback(void (*cb)()){
	callback = cb;
}

#endif // TIMER_USE_CALLBACK

#ifdef TIMER_USE_EVENT

event_type timerEvent = 0;

void TimerHAL_register_event(event_type ev){
	timerEvent = ev;
}

#endif // TIMER_USE_EVENT

void TimerHAL_init() {
	TCCR0B &= ~((1<<CS02)|(1<<CS01)|(1<<CS00));
	TCCR0B |= PRESCALER_BITMASK; //((1<<CS01)|(1<<CS00));
	TCNT0 = TIMER_COUNTER_START_VALUE;
	TIMSK0 |= (1<<TOIE0);
}


ISR(TIMER0_OVF_vect){
#ifdef TIMER_RESOLUTION_MS
	static uint16_t counter = 0;
#endif // TIMER_RESOLUTION_MS
    TCNT0 = TIMER_COUNTER_START_VALUE;
#ifdef TIMER_RESOLUTION_MS
    counter++;
	if (counter == TIMER_LIMIT){
		counter = 0;
#ifdef TIMER_USE_CALLBACK
	    sei();
		callback();
#endif // TIMER_USE_CALLBACK
#ifdef TIMER_USE_EVENT
		setEvent(timerEvent);
#endif // TIMER_USE_EVENT
	}
#endif // TIMER_RESOLUTION_MS	
}


#endif // TIMER_IS_USED

#endif /* TIMERHAL_H_ */