#ifndef TCNT_H_
#define TCNT_H_

#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>


void tcnt2_CTC_init();
void tcnt0_OVR_init();


#endif /* TCNT_H_ */