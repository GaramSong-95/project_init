#include "button.h"




void btn_init(btn *btn_st,volatile uint8_t *DDR,volatile uint8_t *PIN,uint8_t pinNum)
{
	btn_st->DDR=DDR;
	btn_st->PIN=PIN;
	btn_st->pinNum=pinNum;
	btn_st->prevstate=RELEASED;
	gpio_initInd(btn_st->DDR,INPUT,btn_st->pinNum);
}

uint8_t btn_check(btn *btn_st)
{
	uint8_t curstate = gpio_pinInd(btn_st->PIN,btn_st->pinNum);
	if((curstate==RELEASED)&&(btn_st->prevstate==RELEASED))
	{
		_delay_ms(5);
		btn_st->prevstate=PUSHED;
		return PUSHED;
	}
	else if((curstate!=RELEASED)&&(btn_st->prevstate==PUSHED))
	{
		_delay_ms(5);
		btn_st->prevstate=RELEASED;
		return RELEASED;
	}
	return NONE;
}