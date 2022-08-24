
#include "APPS.h"




STATE_type state=Yellow_BEFORE_GREEN; // states in normal mode {state 0=yellow , 1=green , 2=yellow , 3=red}
u8 pedestrian_mode_flag=0; // to indicate that the system entered pedestrian mode (button is pressed)
u32 ON_Period=DELAY_TIME; 
/* 5 sec period for every color in normal mode 
, when push button is pressed (pedestrian_mode_flag = 1) ON_Period is modified to zero 
so the lights are ignored and the normal mode is finished without lights or delay time  */


void APP_Start(void)
{
	Timer0_Init(NORMAL, _8_PRESC);
	SETBit(PD_DDR,PD_GREEN_PIN );   //DDR Pin of  Pedestrian's Green LED as output
	SETBit(PD_DDR,PD_YELLOW_PIN );  //DDR Pin of  Pedestrian's YELLOW LED as output
	SETBit(PD_DDR,PD_RED_PIN );     //DDR Pin of  Pedestrian's RED LED as output
	SETBit(CR_DDR,CR_GREEN_PIN );   //DDR Pin of  Vehicle's    Green LED as output
	SETBit(CR_DDR,CR_YELLOW_PIN );	//DDR Pin of  Vehicle's    YELLOW LED as output
	SETBit(CR_DDR,CR_RED_PIN );		//DDR Pin of  Vehicle's    RED LED as output
	INT_init(INT_0, rising_edge); // enables INT0 and Selects External Interrupt Sense Mode (Rising Edge)
}



void APP_Run(void)
{
	if (pedestrian_mode_flag==1) // system in Pedestrian Mode
	{
		Pedestrian_Mode();
		
	}
	else // system in Normal Mode
	{
		ON_Period=DELAY_TIME;
		if (state==Yellow_BEFORE_GREEN)  // Yellow_BEFORE_GREEN
		{
			for (u8 i=0;i<4;i++)
			{
				SETBit(CR_PORT,CR_YELLOW_PIN );
				timer_delay_us(ON_Period/8);
				CLRBit(CR_PORT,CR_YELLOW_PIN );
				timer_delay_us(ON_Period/8);
			}
			
			if (pedestrian_mode_flag==0)
			state=GREEN;
		}
		// GREEN
		else if (state==GREEN)            
		{
			SETBit(CR_PORT,CR_GREEN_PIN );
			timer_delay_us(ON_Period);
			CLRBit(CR_PORT,CR_GREEN_PIN );
			
			if (pedestrian_mode_flag==0)
			state=Yellow_AFTER_GREEN;
		}
		// Yellow_AFTER_GREEN
		else if (state==Yellow_AFTER_GREEN)  
		{
			for (u8 i=0;i<4;i++)
			{
				SETBit(CR_PORT,CR_YELLOW_PIN );
				timer_delay_us(ON_Period/8);
				CLRBit(CR_PORT,CR_YELLOW_PIN );
				timer_delay_us(ON_Period/8);
			}
			
			
			if (pedestrian_mode_flag==0)
			state=RED;
		}
		// RED
		else if (state==RED)                
		{
			SETBit(CR_PORT,CR_RED_PIN );
			timer_delay_us(ON_Period);
			CLRBit(CR_PORT,CR_RED_PIN );
			
			if (pedestrian_mode_flag==0)
			state=Yellow_BEFORE_GREEN;
		}
		
	}

}



ISR(INT0_vect) // the interrupt is generated at the rising edge so long press has no effect
{ 
	if(pedestrian_mode_flag==0 ) 
	{
		pedestrian_mode_flag=1; // the second press (double press) has no effect as the flag is not 0;
		ON_Period=0;
		Force_Stop_Timer0(); 
		/* Force Stopping Timer0 while the timer_delay_us() function in progress 
		 (which is waiting the rest of 5 sec to finish) */
	}
}

void Pedestrian_Mode()
{
	// system does not need resetting the lights 
	//because the system reaches the end of interrupted state before starting the Pedestrian Mode 
	//as the period time (5 sec) - which is the input of the delay function -  is modified to 0;
	// even if the delay function is interrupted the ISR has Force_Stop_Timer0(); funtion 
	// which maximize the time overflow variable - in Timers.c file - to 999999... // overflow=~0; 
	
	
	//Steps
	//A. Yellow of Pedestrian and Yellow of Vehicles blink for 5 sec
	//B. Green  of Pedestrian and Red    of Vehicles are on For 5 sec
	//C. Yellow of Pedestrian and Yellow of Vehicles blink for 5 sec
	//D. Red    of Pedestrian and Green  of Vehicles are on For 5 sec
	
	
	// step A is not active if the normal mode is red (Vehicles are already stopped)
	if (state==Yellow_BEFORE_GREEN || state==GREEN || state==Yellow_AFTER_GREEN)  // = (state!=RED)
	{
		SETBit(PD_PORT,PD_RED_PIN );// Red of Pedestrians will be on for 5 sec
		SETBit(CR_PORT,CR_GREEN_PIN );// Green of Vehicles will be on for 5 sec
		
		//Yellow of Pedestrian and Yellow of Vehicles Will blink for 5 sec
		//Red of Pedestrian and Green of Vehicles are on for 5 sec
		for (u8 i=0;i<4;i++)
		{
		SETBit(CR_PORT,CR_YELLOW_PIN );
		SETBit(PD_PORT,PD_YELLOW_PIN );
		timer_delay_us(DELAY_TIME/8);
		CLRBit(CR_PORT,CR_YELLOW_PIN );
		CLRBit(PD_PORT,PD_YELLOW_PIN );
		timer_delay_us(DELAY_TIME/8);
		}
		
		CLRBit(PD_PORT,PD_RED_PIN );
		CLRBit(CR_PORT,CR_GREEN_PIN );
	}
	// B. C. D.
	
	//B. Green of Ped. , Red of Veh. for 5 sec
	SETBit(PD_PORT,PD_GREEN_PIN );
	SETBit(CR_PORT,CR_RED_PIN );
	timer_delay_us(DELAY_TIME);
	
	
	//C. Yellow of Ped , Yellow of Veh for 5 sec
    //   Green of Ped. , Red of Veh. for 5 sec
	
	for (u8 i=0;i<4;i++)
	{
		SETBit(CR_PORT,CR_YELLOW_PIN );
		SETBit(PD_PORT,PD_YELLOW_PIN );
		timer_delay_us(DELAY_TIME/8);
		CLRBit(CR_PORT,CR_YELLOW_PIN );
		CLRBit(PD_PORT,PD_YELLOW_PIN );
		timer_delay_us(DELAY_TIME/8);
	}
	CLRBit(PD_PORT,PD_GREEN_PIN );
	CLRBit(CR_PORT,CR_RED_PIN );
	
	//D. Red of Ped. , Green of Veh.  for 5 sec
	SETBit(PD_PORT,PD_RED_PIN );
	SETBit(CR_PORT,CR_GREEN_PIN );
	timer_delay_us(DELAY_TIME);
	CLRBit(PD_PORT,PD_RED_PIN );
	CLRBit(CR_PORT,CR_GREEN_PIN );
	
	state = Yellow_AFTER_GREEN;             // normal mode will start again from yellow before red(after geen)
	pedestrian_mode_flag=0;    // now pedestrian mode is finished
}






