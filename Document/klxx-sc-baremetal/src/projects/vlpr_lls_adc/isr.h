/******************************************************************************
* File:    isr.h
* Purpose: Define interrupt service routines referenced by the vector table.
* Note: Only "vectors.c" should include this header file.
******************************************************************************/

#ifndef __ISR_H
#define __ISR_H 1
/* function prototypes */
void LPTMR0_IRQHandler(void);
void LLWU_IRQHandler(void);
void adc0_isr(void);
#include "lptmr.h"

/* Example */
/*
#undef  VECTOR_101
#define VECTOR_101 lpt_isr


// ISR(s) are defined in your project directory.
extern void lpt_isr(void);
*/
#undef  VECTOR_044
#define VECTOR_044 LPTMR0_IRQHandler
#undef  VECTOR_023 
#define VECTOR_023 LLWU_IRQHandler
#undef  VECTOR_031 
#define VECTOR_031 adc0_isr


#endif  //__ISR_H

/* End of "isr.h" */
