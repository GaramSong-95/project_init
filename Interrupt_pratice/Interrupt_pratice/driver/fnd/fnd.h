#ifndef FND_H_
#define FND_H_

#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>

#define fnd_DDDR   DDRE
#define fnd_ADDDR  DDRF

#define fnd_DPORT  PORTE
#define fnd_ADPORT PORTF

#define fnd_dotdata 10


void fnd_init();
void display(uint16_t data);
void fnd_dotflag();
void fnd_setdata(uint16_t data);
void fnd_run();


#endif /* FND_H_ */