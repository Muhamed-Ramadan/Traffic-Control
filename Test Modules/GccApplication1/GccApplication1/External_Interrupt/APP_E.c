/* bitMath
 * main.c
 * 
 */ 



#include "APP_E.h"

void Ex_init()
{
	INT_init(INT_0 , rising_edge);
	DDRA|=1;
}

ISR(INT0_vect)
{
		PORTA^=1;
}

