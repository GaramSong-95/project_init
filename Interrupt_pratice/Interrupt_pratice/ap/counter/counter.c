#include "counter.h"




uint8_t sec,min,hour,modestate;
uint16_t milisec;


btn btn_mode,btn_modify,btn_up,btn_switch;


void counter_init()
{
	sec=0;
	min=0;
	hour=0;
	milisec=0;
	tcnt2_CTC_init();
	tcnt0_OVR_init();
	fnd_init();
	btn_init(&btn_mode,&DDRA,&PINA,0);
	btn_init(&btn_modify,&DDRA,&PINA,1);
	btn_init(&btn_up,&DDRA,&PINA,2);
	btn_init(&btn_switch,&DDRA,&PINA,3);
	modestate = mode1;
}
void tic_tok()
{
	if(milisec==500) fnd_dotflag();
	
	if((modestate==modify_min)||(modestate==modify_hour)) return;
	
	milisec = (milisec+1)%1000;
	if(milisec) return;
	
	sec = (sec+1)%60;
	if(sec) return;
	
	min = (min+1)%60;
	if(min) return;
	
	hour = (hour+1)%24;
}
void counter_run()
{
	counter_check();
	counter_execute();
}

void counter_check()
{
	switch(modestate)
	{
		case mode1:
		if(btn_check(&btn_mode)==RELEASED)
		{
			modestate = mode2;
		}
		else if(btn_check(&btn_modify)==RELEASED)
		{
			modestate = modify_min;
		}
		break;
		case mode2:
		if(btn_check(&btn_mode)==RELEASED)
		{
			modestate = mode1;
		}
		else if(btn_check(&btn_modify)==RELEASED)
		{
			modestate = modify_min;
		}
		break;
		case modify_min:
		if(btn_check(&btn_modify)==RELEASED)
		{
			modestate = mode2;
		}
		else if(btn_check(&btn_switch)==RELEASED)
		{
			modestate = modify_hour;
		}
		break;
		case modify_hour:
		if(btn_check(&btn_modify)==RELEASED)
		{
			modestate = mode2;
		}
		else if(btn_check(&btn_switch)==RELEASED)
		{
			modestate = modify_min;
		}
		break;
		
	}
}
void counter_execute()
{
	uint16_t counterdata;
	
	switch(modestate)
	{
		case mode1:
		counterdata = (sec*100 + (milisec/100%10)*10 + (milisec/10%10));
		break;
		case mode2:
		counterdata = (hour*100+min);
		break;
		case modify_min:
		counterdata = (hour*100+min);
		if(btn_check(&btn_up)==RELEASED)
		{
			min = (min+1)%60;
		}
		else if(btn_check(&btn_mode)==RELEASED)
		{
			counter_init();
		}
		break;
		case modify_hour:
		counterdata = (hour*100+min);
		if(btn_check(&btn_up)==RELEASED)
		{
			hour = (hour+1)%24;
		}
		else if(btn_check(&btn_mode)==RELEASED)
		{
			counter_init();
		}
		break;
	}
	
	fnd_setdata(counterdata);
}