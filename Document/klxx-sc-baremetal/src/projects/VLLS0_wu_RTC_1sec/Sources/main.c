/*
 * main implementation: use this 'C' sample to create your own application
 *
 */



/*#include <stdio.h>*/ /* uncomment to enable 'puts' below */
#define  TOWER
#include "derivative.h" /* include peripheral declarations */
#include "llwu.h"
#include "smc.h"
#include "tower.h"
#include "rtc.h"
#include "adc16.h"
#include "main.h"


/* global variables */
signed char result[512];  // Holds accelerometer readings
signed char avgtilt = 0;  // Holds the value for the average tilt of the board at startup
tADC_Config Master_Adc_Config;

int core_clk_khz;
int counter;

/* main */
int main(void)

{
    int i;
	for(;;) {	   
	   	
	   	SIM_SRVCOP = 0x55;	   	// 1st write to service COP
	   	SIM_SRVCOP = 0xaa;   //2nd Write to service COP
        PMC_REGSC |= PMC_REGSC_ACKISO_MASK; //write to release hold on I/O 
        #ifdef USE_ADC
        for (i=0;i<1000;i++){
             if (ADC0_SC2 & ADACT_ACTIVE)
                    F_LED2_TOGGLE;
             else
                    break;
             }
             if (counter >511)
            	 counter = 0;
             result[counter] = ADC0_RA ;      
        #endif
        F_LED0_ON;
        //enter_vlps();
        
        enter_lls();
        //enter_vlls1();	// lowest power mode that the COP can work.  If you use VLLS disable COP
        //enter_vlls0();	// lowest power mode that the COP can work.  If you use VLLS disable COP
	}
	
	return 0;
}


void enable_irq (int irq)
{   
    /* Make sure that the IRQ is an allowable number. Up to 32 is 
     * used.
     *
     * NOTE: If you are using the interrupt definitions from the header
     * file, you MUST SUBTRACT 16!!!
     */
    if (irq <= 32){
      /* Set the ICPR and ISER registers accordingly */
      NVIC_ICPR |= 1 << (irq%32);
      NVIC_ISER |= 1 << (irq%32);
    }
}
void disable_irq (int irq)
{
    
    /* Make sure that the IRQ is an allowable number. Right now up to 48 is 
     * used.
     */
    if (irq > 32)
        while (1);  /* error trap*/
    
    /* Determine which of the NVICICERs corresponds to the irq */
    
    NVIC_ICER = 1 << (irq%32);
 }
void SW_LED_Init(void)
{


   LED0_EN;
   LED0_DIR;
   LED1_EN; 
   LED1_DIR;
   LED2_EN; 
   LED2_DIR;
   LED3_EN; 
   LED3_DIR;
   
}
void porta_isr(void)
{
   F_LED0_OFF;
   if(MCM_CPO & MCM_CPO_CPOACK_MASK){
      MCM_CPO &= ~MCM_CPO_CPOREQ_MASK;
      while (MCM_CPO & MCM_CPO_CPOACK_MASK);                              
  }
  
  if(PORTA_ISFR == (1<<4))
  {
   PORTA_PCR4 |= PORT_PCR_ISF_MASK;    
  }
  return;
}
