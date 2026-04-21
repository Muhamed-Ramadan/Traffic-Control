/* bitMath
 * main.c
 * 
 */ 


#include "App_M.h"


void bit_Math_Init()
{
	SETBit(DDRA,PA0 ); // Pin 0 of port a as output
	SETBit(DDRA,PA1);  // Pin 1 of port a as output
}

void bit_Math_Test()
{
	SETBit(PORTA,PA0); // pin 0 is HIGH
	TGLBit(PORTA,PA1); // toggle pin 1
	_delay_ms(1000);
	CLRBit(PORTA,PA0);  // pin 0 is LOW
	TGLBit(PORTA,PA1); // toggle pin 1
	_delay_ms(1000);
	
}
