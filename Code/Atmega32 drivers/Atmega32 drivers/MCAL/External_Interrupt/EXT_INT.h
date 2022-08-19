


#ifndef EXT_INT_H_
#define EXT_INT_H_

#include "dataTypes.h"
#include "bitMath.h"
#include "registers.h"
#include "ISR_Interrupt.h"

typedef enum {INT_2=5,INT_0,INT_1}INT_NUM; //selects the external interrupt number
typedef enum {low_level,any_level,rising_edge,falling_edge }SENSE_CONTROL;  //selects the sense control mode

void INT_init(INT_NUM int_num , SENSE_CONTROL sense_control); // initialize the configuration for the external interrupt
// it takes 2 inputs interrupt number and sense control mode




#endif /* EXT_INT_H_ */