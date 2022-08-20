/* bitMath
 * main.c
 * 
 */ 


#include "App_M.h"


void bit_Math_Init()
{
	SETBit(DDRA,PA0 );
	SETBit(DDRA,PA1);
}

void bit_Math_Test()
{
	SETBit(PORTA,PA0);
	TGLBit(PORTA,PA1);
	_delay_ms(1000);
	CLRBit(PORTA,PA0);
	TGLBit(PORTA,PA1);
	_delay_ms(1000);
	
}
