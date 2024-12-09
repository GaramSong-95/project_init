#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "ap/counter/counter.h"
//#include "driver/fnd/fnd.h"

ISR(TIMER0_OVF_vect)
{
	fnd_run();
}
ISR(TIMER2_COMP_vect)
{
	tic_tok();
}

int main(void)
{
    
	counter_init();
	
	sei();
	
	
	
    while (1) 
    {
		counter_run();
    }
}

