
#ifndef BUTTON_H_
#define BUTTON_H_

#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>

#include "../../periph/gpio/gpio.h"

typedef struct btn_{
	volatile uint8_t *DDR;
	volatile uint8_t *PIN;
	uint8_t pinNum;
	uint8_t prevstate;
}btn;

enum {RELEASED,PUSHED,NONE};

void btn_init(btn *btn_st,volatile uint8_t *DDR,volatile uint8_t *PIN,uint8_t pinNum);
uint8_t btn_check(btn *btn_st);



#endif /* BUTTON_H_ */