/* ISR
 * main.c
 * 
 */ 


#include "APP_I.h"


void ISR_init()
{
	GICR|=(1<<7); // enable INT1
	MCUCR|=(2<<2); // sense control of INT1 as Rising Edge
	DDRA|=1;     //pin 0 of port a as output
	sei();       // enable global interrupt
}

ISR(INT1_vect_)
{
	PORTA^=1; // toggle pin 0
}

