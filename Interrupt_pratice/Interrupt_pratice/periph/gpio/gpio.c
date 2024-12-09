#include "gpio.h"


void gpio_initAll(volatile uint8_t *DDR,uint8_t Direction)
{
	if(Direction==INPUT)
	{
		*DDR = 0x00;
	}
	else if(Direction==OUTPUT)
	{
		*DDR = 0xff;
	}
}
void gpio_initInd(volatile uint8_t *DDR,uint8_t Direction,uint8_t pinNum)
{
	if(Direction==OUTPUT)
	{
		*DDR |= (1<<pinNum);
	}
	else if(Direction==INPUT)
	{
		*DDR &= ~(1<<pinNum);
	}
}
void gpio_portAll(volatile uint8_t *PORT,uint8_t data)
{
	*PORT = data;
}
void gpio_portInd(volatile uint8_t *PORT,uint8_t pinNum)
{
	*PORT = (1<<pinNum);
}
uint8_t gpio_pinAll(volatile uint8_t *PIN)
{
	return *PIN;
}
uint8_t gpio_pinInd(volatile uint8_t *PIN,uint8_t pinNum)
{
	return ((*PIN & (1<<pinNum))!=0);
}