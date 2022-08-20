/* External Interrupt
 * main.c
 * 
 */ 



#include "APP_E.h"

void Ex_init()
{
	INT_init(INT_0 , rising_edge);
	DDRA|=1; // pin 0 of port a as output
}

ISR(INT0_vect)
{
		PORTA^=1; // toggle pin 0
}

