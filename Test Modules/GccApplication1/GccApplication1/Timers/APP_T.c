/* Timer
 * main.c
 * 
 */ 


#include "APP_T.h"




void Timer_Test_Init()
{
	DDRA|=1;                //pin 0 of port a as output
	Timer0_Init(NORMAL);
}

void Timer_Test()
{
	PORTA^=1;            // toggle pin 0
	timer_delay_us(1000000); // 1sec
	
}
