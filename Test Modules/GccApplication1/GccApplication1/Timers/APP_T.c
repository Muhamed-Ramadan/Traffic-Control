/* bitMath
 * main.c
 * 
 */ 


#include "APP_T.h"




void Timer_Test_Init()
{
	DDRA|=1;
	Timer0_Init(NORMAL);
}

void Timer_Test()
{
	PORTA^=1;
	timer_delay_us(1000000);
	
}
