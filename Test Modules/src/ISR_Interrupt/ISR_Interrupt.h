#ifndef INTERRUPT_ISR_H_
#define INTERRUPT_ISR_H_




# define sei()  __asm__ __volatile__ ("sei" ::: "memory") // enables the global interrupt
# define cli()  __asm__ __volatile__ ("cli" ::: "memory") // disables the global interrupt

#define INT0_vect_		__vector_1      //This Macro defines IRQ0 Handler (external Interrupt INT0)
#define INT1_vect_		__vector_2
#define TIMER0_OVF_vect		__vector_11 //This Macro defines Timer0 Overflow Handler

// general Macro Function for ISRs
#define ISR(INT_VECT)	void INT_VECT(void)__attribute__((signal,used));\
						void INT_VECT(void)

#endif /* INTERRUPT_H_ */