/*
 * File:		LQRUG_bme_ex2.c
 * Purpose:		Main process
 *
 */

#include "common.h"
#include "lptmr.h"
#include "gpio.h"
#include "LQRUG_bme_ex2.h"
#include "adc.h"
#include "systick.h"
#include "llwu.h"
#include "smc.h"

#ifdef CMSIS
#include "start.h"
#endif



/********************************************************************/
int main (void)
{  
	uint32_t execution_cycle;	//actual execution cycle
	char ch;

#ifdef CMSIS  // If we are conforming to CMSIS, we need to call start here
    start();
#endif

	printf("\n\rRunning the LQRUG_bme_ex2 project.\n\r");
	

	if (RCM_SRS0 & RCM_SRS0_WAKEUP_MASK)
	{
	  	printf("Wakeup initialization flow\n\r");
		
		systick_init();
		
		cnt_start_value = SYST_CVR;
			
		Init_BME_GPIO();
		
		ADC_BME_Trigger();
		
		//Set LPTMR to timeout about 1 second
		Lptmr_BME_Init(1000, LPOCLK);	
		
		ADC_BME_Init();
		Calibrate_BME_ADC();
		ADC_BME_Init();
		ADC_Start(ADC0_CHANB);
		
		// Enable the ADC interrupt in NVIC
#ifdef CMSIS
		enable_irq(ADC0_IRQn) ;   // ready for this interrupt.  
		enable_irq(LPTimer_IRQn);
#else
		enable_irq(ADC0_irq_no) ;   // ready for this interrupt.  
		enable_irq(LPTMR0_irq_no);
#endif
	  
		cnt_end_value = SYST_CVR;
		
		execution_cycle = cnt_start_value - cnt_end_value - overhead;
		
		systick_disable();
		
#ifdef DEBUG_PRINT
		printf("Systick start value: 0x%x\n\r", cnt_start_value);
		printf("Systick end value: 0x%x\n\r", cnt_end_value);
		printf("Actual execution cycle for initialization phase in normal C code: 0x%x\n\r", execution_cycle);
#endif	
	}
	else
	{
	  	printf("Normal initialization flow\n\r");	//make sure the two printf has the same characters to output
		
	  	systick_init();
	
		cnt_start_value = SYST_CVR;
			
		Init_GPIO();
		
		ADC_Trigger();
		
		//Set LPTMR to timeout about 1 second
		Lptmr_Init(1000, LPOCLK);	
		
		ADC_Init();
		Calibrate_ADC();
		ADC_Init();
		
		ADC_Start(ADC0_CHANB);
		
		// Enable the ADC interrupt in NVIC
#ifdef CMSIS
		enable_irq(ADC0_IRQn) ;   // ready for this interrupt.  
		enable_irq(LPTimer_IRQn);
#else
		enable_irq(ADC0_irq_no) ;   // ready for this interrupt.  
		enable_irq(LPTMR0_irq_no);
#endif

		cnt_end_value = SYST_CVR;
		
		execution_cycle = cnt_start_value - cnt_end_value - overhead;
		
		systick_disable();
		
#ifdef DEBUG_PRINT
		printf("Systick start value: 0x%x\n\r", cnt_start_value);
		printf("Systick end value: 0x%x\n\r", cnt_end_value);
		printf("Actual execution cycle for initialization phase in normal C code: 0x%x\n\r", execution_cycle);
#endif	
	}
	
	Lptmr_Start();
	
#ifndef FREEDOM
	printf("ADC conversion for potentiometer started, press any key to stop ADC conversion\n\r");
#else
	printf("No potentiometer or LED on FREEDOM board, press any key to stop ADC conversion\n\r");
#endif
	
	while(!char_present()) 
	{
#ifndef FREEDOM
		if (cycle_flags == ADC0A_DONE) 
		{
			printf("\r  R0A=%8d",result0A); 
			cycle_flags &= ~ADC0A_DONE ;
		}	
#endif
	} 
	
	in_char();	//Read out any available characters 
	
	ADC_Stop();
	
	printf("ADC conversion stopped, press 'l' to enter VLLS1 mode\n\r");
	
#ifndef FREEDOM
	printf("Press SW3 or SW4(Reset button) on TWR-KL25Z48M to exit VLLS1 mode\n\r");	
#else
	printf("Press SW1(Reset button) on FREEDOM board to exit VLLS1 mode\n\r");
#endif	
	
	while(1)
	{
	  ch = in_char();
	  //out_char(ch);
	  if(ch != 'l')
	  	printf("Incorrect character input, Press 'l' to enter VLLS1 mode\n\r");	
	  else
		break;
	}
		
	llwu_configure(0x0080/*PTC3*/, LLWU_PIN_FALLING, 0x0);
	
	/* Configure SW3 - init for GPIO PTC3/LLWU_P7/UART1_RX/FTM0_CH2/CLKOUT*/
	PORTC_PCR3 = ( PORT_PCR_MUX(1) |
				   PORT_PCR_PE_MASK |
				   PORT_PCR_PFE_MASK |
				   PORT_PCR_PS_MASK);
	  
	enter_vlls1();		  
	
}


/********************************************************************/
// End with new line to suppress KEIL warning.
