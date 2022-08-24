

 
#include "Timers.h"

volatile static  u32 overflow=0; // holder number of overflows done by counter register
static Prescaler_EN prescaler;
static double Tick_Time;
static double Overflow_Time;
static u32 Maximum_Overflow_Times;


ISR(TIMER0_OVF_vect)    //this isr is called when counter register does overflow
{
	overflow++;	
}






void Timer0_Init(timer_modes_EN Tmode,Prescaler_EN pre)
{
	u16 Array_Of_Prescalers[6]={0,1,8,64,256,1024}; // arr[_8_PRESC]=arr[2]=8
	prescaler=pre;
	Tick_Time=Array_Of_Prescalers[prescaler] / F_CPU;  // the time taken by 1 tick 
	Overflow_Time=256UL*Tick_Time;                     // the timer taken by 1 overflow (256 ticks)
	
	switch(Tmode)
	{
		case NORMAL:          // selecting NORMAL mode by clearing bit 3 and bit 6 in TCCR0 register
		CLRBit(TCCR0,WGM01);
		CLRBit(TCCR0,WGM00);
		
		Timer0_Stop();
		
		SETBit(TIMSK,TOIE0); // enable the timer0 interrupt in register TIMSK
		sei();               // enable the global interrupt
		break;
		
		case CTC:
		break;
		
		case FPWM:
		/*
		SETBit(TCCR0,WGM01);
		SETBit(TCCR0,WGM00);
		
		SETBit(TCCR0,COM01);
		CLRBit(TCCR0,COM00);
		
		SETBit(TCCR0,CS00);
		CLRBit(TCCR0,CS01);
		SETBit(TCCR0,CS02);
		*/
		break;
	}
}



void Timer0_Start(void) // the counter register TCNT0 start counting 
{                       // by writing the prescaler value {NO_PRESC,_8_PRESC,_16_PRESC,_64_PRESC,_256_PRESC,_1024_PRESC}
	/*
	CLRBit(TCCR0,CS00);
	SETBit(TCCR0,CS01);
	CLRBit(TCCR0,CS02);
	*/
	TCCR0|=prescaler;
}
void Timer0_Stop(void) // stop the counter register TCNT0 from counting
{
	CLRBit(TCCR0,CS00);
	CLRBit(TCCR0,CS01);
	CLRBit(TCCR0,CS02);
}

void ResetTimer(void) // clearing the counter register for new counting
{
	TCNT0=0x00;        //   TCNT0 is the Timer COunter Register
	overflow=0;
}
void timer_delay_us(u32 delay)
{// 1000 000 / 1 ------ 1000 000 / 8
	ResetTimer();
	Maximum_Overflow_Times=ceil(delay/Overflow_Time);       // number of entire overflows till the end of the delay
	TCNT0=256-( (delay/Tick_Time)/Maximum_Overflow_Times ); // initial value that the counter will start with
	Timer0_Start();
	while( overflow<Maximum_Overflow_Times );
	Timer0_Stop();
}


void Force_Stop_Timer0()    // set all the bits of overflow variable , overflow =0x FF FF FF FF
{
	overflow=~0;
}
