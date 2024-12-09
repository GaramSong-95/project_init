#ifndef COUNTER_H_
#define COUNTER_H_

#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>

enum {mode1,mode2,modify_min,modify_hour};


#include "../../periph/gpio/gpio.h"
#include "../../periph/tcnt/tcnt.h"
#include "../../driver/fnd/fnd.h"
#include "../../driver/button/button.h"


void counter_init();
void tic_tok();
void counter_run();
void counter_check();
void counter_execute();


#endif /* COUNTER_H_ */