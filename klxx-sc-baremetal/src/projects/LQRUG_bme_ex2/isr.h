/******************************************************************************
* File:    isr.h
* Purpose: Define interrupt service routines referenced by the vector table.
* Note: Only "vectors.c" should include this header file.
******************************************************************************/

#ifndef __ISR_H
#define __ISR_H 1


/* Example */
/*
#undef  VECTOR_101
#define VECTOR_101 lpt_isr


// ISR(s) are defined in your project directory.
extern void lpt_isr(void);
*/

extern void adc0_isr(void);         // ISR for ADC
extern void lptmr_isr(void);		// ISR for LPTMR

#undef  VECTOR_031   // as it was previously defined in vectors.h
#undef	VECTOR_044	 // as it was previously defined in vectors.h

                                    // address     vector irq   perihperal
#define VECTOR_031      adc0_isr    // 0x0000_007C 31     15    ADC0
#define VECTOR_044		lptmr_isr	// 0x0000_00B0 44	  28	LPTMR0	

#endif  //__ISR_H

/* End of "isr.h" */
