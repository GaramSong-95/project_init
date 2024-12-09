#ifndef GPIO_H_
#define GPIO_H_

#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>


enum  {INPUT,OUTPUT};
	
	
void gpio_initAll(volatile uint8_t *DDR,uint8_t Direction);
void gpio_initInd(volatile uint8_t *DDR,uint8_t Direction,uint8_t pinNum);
void gpio_portAll(volatile uint8_t *PORT,uint8_t data);
void gpio_portInd(volatile uint8_t *PORT,uint8_t pinNum);
uint8_t gpio_pinAll(volatile uint8_t *PIN);
uint8_t gpio_pinInd(volatile uint8_t *PIN,uint8_t pinNum);

#endif /* GPIO_H_ */