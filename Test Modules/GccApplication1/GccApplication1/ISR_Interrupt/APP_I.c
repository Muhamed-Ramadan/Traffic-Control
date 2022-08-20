/* bitMath
 * main.c
 * 
 */ 


#include "APP_I.h"


void ISR_init()
{
	GICR|=(1<<7);
	MCUCR|=(2<<2);
	DDRA|=1;
	sei();
}

ISR(INT1_vect_)
{
	PORTA^=1;
}

