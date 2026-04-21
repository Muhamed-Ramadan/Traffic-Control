
#include "EXT_INT.h"


void INT_init(INT_NUM int_num , SENSE_CONTROL sense_control) // takes 2 parameters 1:
//                                            {INT0,INT1,INT2} 2:{rising edge,falling edge,low_level,any_level}
{
	GICR|=(1<<int_num); // Enable INT0 , INT1 or INT2 Interrupt
	switch(int_num) // writing the sense control
	{
		case INT_0:
		MCUCR|=sense_control;               // writing in bit 0 and bit 1
		break;
		case INT_1:
		MCUCR|=(sense_control<<2);         // writing in bit 2 and bit 3
		break;
		case INT_2:
		MCUCSR|=((sense_control & 0x01)<<6);   // writing in bit 6 // only 2 sense_control rising edge or falling edge
	}
	sei(); // enable global interrupt
}


