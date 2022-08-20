

 
#include "Timers.h"
#include "ISR_Interrupt.h"

volatile static  u32 overflow=0;


ISR(TIMER0_OVF_vect)
{
	overflow++;	
}





void Force_Stop_Timer0()
{
	overflow=~0;	
}
void Timer0_Init(timer_modes Tmode)
{
	switch(Tmode)
	{
		case NORMAL:
		CLRBit(TCCR0,WGM01);
		CLRBit(TCCR0,WGM00);
		
		Timer0_Stop();
		
		SETBit(TIMSK,TOIE0);
		sei();
		break;
		case CTC:
		
		break;	
		case FPWM:
		SETBit(TCCR0,WGM01);
		SETBit(TCCR0,WGM00);
		
		SETBit(TCCR0,COM01);
		CLRBit(TCCR0,COM00);
		
		SETBit(TCCR0,CS00);
		CLRBit(TCCR0,CS01);
		SETBit(TCCR0,CS02);
		break;
	}
}



void Timer0_Start(void)
{
	CLRBit(TCCR0,CS00);
	SETBit(TCCR0,CS01);
	CLRBit(TCCR0,CS02);
}
void Timer0_Stop(void)
{
	CLRBit(TCCR0,CS00);
	CLRBit(TCCR0,CS01);
	CLRBit(TCCR0,CS02);
}

void ResetTimer(void)
{
	TCNT0=0x00;
	overflow=0;
}
void timer_delay_us(u32 delay)
{
	ResetTimer();
	Timer0_Start();
	while(delay > ((TCNT0+1+overflow*256UL)) ); //   256UL/2UL	
	Timer0_Stop();
}
