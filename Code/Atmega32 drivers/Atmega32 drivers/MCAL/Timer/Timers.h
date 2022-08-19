
#ifndef TIMERS_H_
#define TIMERS_H_

#include "dataTypes.h"
#include "bitMath.h"
#include "registers.h"

typedef enum{NORMAL,Phase_PWM,CTC,FPWM}timer_modes; // selects the timer modes
typedef enum{STOP,NO_PRESC,_8_PRESC,_16_PRESC,_64_PRESC,_256_PRESC,_1024_PRESC}Prescaler; // selects the prescaler of the frequency of the timer
/*this bits define the prescaler*/	
#define CS00 0
#define CS01 1
#define CS02 2	
/*this bits define compare output mode*/
#define COM00 4
#define COM01 5	

/*this bits define compare output mode*/
#define WGM00	6
#define WGM01	3

	
/*Timer/Counter Interrupt Mask*/	
#define TOIE0	0





/*Timer0 function*/
void Timer0_Init(timer_modes Tmode); // selects the mode and sets the configuration of the timer
// it has 1 input, the mode of the timer, NORMAL mode is selected in this system
void Timer0_start(void);	    // starts counting
void Timer0_Stop(void);		    // finishes counting
void ResetTimer(void);		    // resets the counting and counter register to zero
void timer_delay_us(u32 delay); // busy waiting (delay function)
void Force_Stop_Timer0();       // force stopping busy waiting
//ISR(TIMER0_OVF_vect) increments the overflow variable when the counter register reaches max









#endif




