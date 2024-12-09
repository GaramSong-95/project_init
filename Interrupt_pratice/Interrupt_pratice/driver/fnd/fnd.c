#include "fnd.h"


	
uint16_t countdata;
uint8_t dotdata;


void fnd_init()
{
	
	DDRE = 0xff;
	DDRF = 0xff;
	countdata=0;
	dotdata=0;
}



void display(uint16_t data)
{
	uint8_t fndFont[11]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x67, 0x80};
	
	static uint8_t fndDigitState=0;
	fndDigitState=(fndDigitState+1)%5;
	
	PORTE |= (1<<7)|(1<<6)|(1<<5)|(1<<4); // 먼저 다 꺼서 잔상효과를 없앤다.
	
	switch(fndDigitState)
	{
		case 0:
		PORTF=fndFont[data/1000%10];        // 원하는 자리의 숫자를 뽑아낼 수 있음. 천의 자리
		PORTE &= ~(1<<4);
		break;
		case 1:
		PORTF=fndFont[data/100%10];
		PORTE &= ~(1<<5);
		break;
		case 2:
		PORTF=fndFont[data/10%10];
		PORTE &= ~(1<<6);
		break;
		case 3:
		PORTF=fndFont[data%10];
		PORTE &= ~(1<<7);
		break;
		case 4:                                      // DP 점찍는 코드
		if(dotdata)
		PORTF=fndFont[10];
		else PORTF=0x00;
		PORTE &= ~(1<<5);
		break;
	}
}

void fnd_dotflag()
{
	dotdata = ~dotdata;
}
void fnd_setdata(uint16_t num)
{
	countdata=num;
}
void fnd_run()
{
	display(countdata);
}