

#ifndef REGISTERS_H_
#define REGISTERS_H_

#include "dataTypes.h"
/*A general function to provide the address*/
#define SELECTOR(ADDRESS) (*((volatile u8*)ADDRESS))
#define SELECTOR_16(ADDRESS) (*((volatile u16*)ADDRESS))



/*Port A Register*/                               
/*if the DDRx is set to be output and we write High to the PORTx 
* this will activate the internal Pull up resistor.
*/
#define PORTA  SELECTOR(0x3B)    //1->high output				0->low output  
#define DDRA   SELECTOR(0x3A)   //1->to make it output			0->to make it input    
#define PINA   SELECTOR(0x39)  //this register to read a value from a pin 

/*Port B Register*/
#define PORTB  SELECTOR(0x38)
#define DDRB  SELECTOR(0x37)
#define PINB  SELECTOR(0x36)

/*Port C Register*/
#define PORTC  SELECTOR(0x35)
#define DDRC   SELECTOR(0x34)
#define PINC  SELECTOR(0x33)

/*Port D Register*/
#define PORTD  SELECTOR(0x32)
#define DDRD   SELECTOR(0x31)
#define PIND  SELECTOR(0x30)

/*EXTERNAL INTERRUPT REGESTERS*/

#define MCUCR  SELECTOR(0X55)
#define MCUCSR SELECTOR(0X54)
#define GICR   SELECTOR(0X5B)
#define	GIFR   SELECTOR(0X5A)
#define SREG   SELECTOR(0x5F)

/*Timers Registers*/
#define TCCR0	SELECTOR(0x53)
#define OCR0	SELECTOR(0x5C)
#define TCNT0	SELECTOR(0x52)
#define TIMSK	SELECTOR(0x59)
#define TIFR	SELECTOR(0x58)


/*Timer0 registers*/
#define TCCR0   SELECTOR(0x53)
#define TCNT0   SELECTOR(0x52)
#define OCR0    SELECTOR(0x5C)
#define TIMSK	SELECTOR(0x59)
#define TIFR	SELECTOR(0x58)



typedef enum{PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7}PortA;
typedef enum{PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7}PortB;
typedef enum{PC0,PC1,PC2,PC3,PC4,PC5,PC6,PC7}PortC;
typedef enum{PD0,PD1,PD2,PD3,PD4,PD5,PD6,PD7}PortD;


#endif /* REGISTERS_H_ */