/******************************************************************************
* File:    isr.c
* Purpose: Define interrupt service routines referenced by the vector table.
* Note: Only "vectors.c" should include this header file.
******************************************************************************/

#include "vectors.h"
#include "common.h"
#include "isr.h"
#include "lptmr.h"



extern uint32 converting;
extern uint32 result0A;
extern void enter_vlps();

void LPTMR0_IRQHandler(void)
{
    F_LED1_TOGGLE;

    // Clear LPTMR interrupt flag (by writing the flag)
    LPTMR0_CSR = ( LPTMR_CSR_TEN_MASK | LPTMR_CSR_TIE_MASK | LPTMR_CSR_TCF_MASK  );
    LPTMR0_CSR |= ( LPTMR_CSR_TEN_MASK | LPTMR_CSR_TIE_MASK | LPTMR_CSR_TCF_MASK  );
    // Toggle LED so that I know this is happening 
    F_LED1_TOGGLE;

}
#ifdef RAM16

__ramfunc void LLWU_IRQHandler(void)
{

    // Toggle LED so that I know this is happening (PTD6)
    F_LED0_ON;

    LPTMR0_CSR = ( LPTMR_CSR_TEN_MASK | LPTMR_CSR_TIE_MASK | LPTMR_CSR_TCF_MASK  );   
    LPTMR0_CSR |=  LPTMR_CSR_TCF_MASK;   // write 1 to TCF to clear the LPT timer compare flag

    // Get quickly back into VLPR  
    SMC_PMCTRL  = SMC_PMCTRL_RUNM(0x2)|SMC_PMCTRL_STOPM(3); 
                      
    //Start an ADC conversion
    ADC0_SC1A = 0x00000004;  
    
    NVIC_ICPR |= 1 << (LPTMR_irq_no%32); 
//    NVIC_ISER |= 1 << (LPTMR_irq_no%32);
   
}
#else
void LLWU_IRQHandler(void)
{

    // Toggle LED so that I know this is happening (PTD6)
    F_LED0_ON;

    LPTMR0_CSR = ( LPTMR_CSR_TEN_MASK | LPTMR_CSR_TIE_MASK | LPTMR_CSR_TCF_MASK  );   
    LPTMR0_CSR |=  LPTMR_CSR_TCF_MASK;   // write 1 to TCF to clear the LPT timer compare flag

    // Get quickly back into VLPR  
    SMC_PMCTRL  = SMC_PMCTRL_RUNM(0x2)|SMC_PMCTRL_STOPM(3); 
                      
    //Start an ADC conversion
    ADC0_SC1A = 0x00000004;  
    
    NVIC_ICPR |= 1 << (LPTMR_irq_no%32); 
    NVIC_ISER |= 1 << (LPTMR_irq_no%32);
   
}
#endif
/******************************************************************************
* adc0_isr(void)
*
* use to signal ADC0 end of conversion
* In:  n/a
* Out: exponentially filtered potentiometer reading!
* The ADC0 is used to sample the potentiometer on the A side and the B side:
* ping-pong.  That reading is filtered for an agregate of ADC0 readings: exponentially_filtered_result1
* thus the filtered POT output is available for display.
******************************************************************************/

void adc0_isr(void)
{
  result0A = ADC0_RA;           // this will clear the COCO bit that is also the interrupt flag
  converting = 0;       // clear the converting flag

    
}

