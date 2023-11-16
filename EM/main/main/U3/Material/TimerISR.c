#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>


ISR(TIMER0_OVF_vect){
	static uint16_t counter = 0;
	TCNT0 = 6;
	counter++;
	if (counter==1000){
		counter = 0;
		PORTB ^= (1<<PORTB1);
	}
}


int main(void)
{
    DDRB |= (1<<PORTB1)|(1<<PORTB0);
	TCNT0 = 6;
	TCCR0B &= ~((1<<CS02)|(1<<CS01)|(1<<CS00));
	TCCR0B |= (1<<CS01)|(1<<CS00);
	TIMSK0 |= (1<<TOIE0);
	sei();
    while (1) 
    {
		_delay_ms(200);
		PORTB ^= (1<<PORTB0);
    }
}
