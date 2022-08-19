


#ifndef APPS_H_
#define APPS_H_


#define  F_CPU 16000000UL
#include "bitMath.h"
#include "dataTypes.h"
#include "registers.h"
#include "APPS.h"
#include "EXT_INT.h"
#include "Timers.h"
#include "ISR_Interrupt.h"



#define PD_PORT                   PORTA  // DIO Port of Pedestrians
#define PD_DDR                    DDRA   // DIO DDR  of Pedestrians
#define CR_PORT                   PORTB  // DIO Port of Vehicles
#define CR_DDR                    DDRB   // DIO DDR  of Vehicles

#define BUTTON_PIN                INT0  // Push Button external interrupt pin
#define PD_RED_PIN                PA0   // Pedestrians RED    Led
#define PD_YELLOW_PIN             PA1   // Pedestrians YELLOW Led
#define PD_GREEN_PIN              PA2   // Pedestrians GREEN  Led
#define CR_RED_PIN                PB0   // Vehicles    RED    Led
#define CR_YELLOW_PIN             PB1   // Vehicles    YELLOW Led
#define CR_GREEN_PIN              PB2   // Vehicles    GREEN  Led
#define DELAY_TIME                (5000000) // Delay Period (5 sec)

typedef enum {Yellow_BEFORE_GREEN,GREEN,Yellow_AFTER_GREEN,RED}STATE_type;

void APP_Start(void);// initializes the timer (Normal Mode) , PORTA pins (Pedestrians LEDs), 
                     //PORTB pins (Vehicles LEDs) and External Interrupt Sense Mode (Rising Edge) and enables INT0
void APP_Run(void);
void Pedestrian_Mode(); // starts actions of the Pedestrian's Mode
//ISR(INT0_vect);       // making the system enter the Pedestrian's Mode by push button pressing event





#endif /* APPS_H_ */