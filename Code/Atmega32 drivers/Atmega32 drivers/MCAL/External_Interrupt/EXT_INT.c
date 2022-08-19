
#include "EXT_INT.h"


void INT_init(u8 int_num , u8 sense_control)
{
	GICR|=(1<<int_num);
	switch(int_num)
	{
		case INT_2:
		MCUCSR|=((sense_control & 0x01)<<6);
		break;
		
		case INT_0:
		MCUCR|=sense_control;
		break;
		case INT_1:
		MCUCR|=(sense_control<<2);
	}
	sei();
}


