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
extern void dma0_isr(void);
extern void porta_isr(void);

#undef  VECTOR_016
#define VECTOR_016 dma0_isr

#undef  VECTOR_046
#define VECTOR_046 porta_isr

#endif  //__ISR_H

/* End of "isr.h" */
