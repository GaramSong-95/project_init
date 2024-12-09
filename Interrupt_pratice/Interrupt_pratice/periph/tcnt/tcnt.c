#include "tcnt.h"

void tcnt2_CTC_init()
{
	TCCR2 |= (0<<CS22)|(1<<CS21)|(1<<CS20);
	
	TCCR2 |= (1<<WGM21)|(0<<WGM20);
	
	TIMSK |= (1<<OCIE2);
	
	OCR2 = 250-1;
}

void tcnt0_OVR_init()
{
	TCCR0 |= (1<<CS02)|(0<<CS01)|(1<<CS00);
	
	TIMSK |= (1<<TOIE0);
	
	TCNT0 = 0;
}