/**
  \mainpage 
  \n Copyright (c) 2011 Freescale
  \n Freescale Confidential Proprietary 
  \brief 
  \author   	Freescale Semiconductor
  \author    
  \version      11
  \date         Sep 18, 2011
  
  Put here all the information needed of the Project. Basic configuration as well as information on
  the project definition will be very useful 
*/
/****************************************************************************************************/
/*                                                                                                  */
/* All software, source code, included documentation, and any implied know-how are property of      */
/* Freescale Semiconductor and therefore considered CONFIDENTIAL INFORMATION.                       */
/* This confidential information is disclosed FOR DEMONSTRATION PURPOSES ONLY.                      */
/*                                                                                                  */
/* All Confidential Information remains the property of Freescale Semiconductor and will not be     */
/* copied or reproduced without the express written permission of the Discloser, except for copies  */
/* that are absolutely necessary in order to fulfill the Purpose.                                   */
/*                                                                                                  */
/* Services performed by FREESCALE in this matter are performed AS IS and without any warranty.     */
/* CUSTOMER retains the final decision relative to the total design and functionality of the end    */
/* product.                                                                                         */
/* FREESCALE neither guarantees nor will be held liable by CUSTOMER for the success of this project.*/
/*                                                                                                  */
/* FREESCALE disclaims all warranties, express, implied or statutory including, but not limited to, */
/* implied warranty of merchantability or fitness for a particular purpose on any hardware,         */
/* software ore advise supplied to the project by FREESCALE, and or any product resulting from      */
/* FREESCALE services.                                                                              */
/* In no event shall FREESCALE be liable for incidental or consequential damages arising out of     */
/* this agreement. CUSTOMER agrees to hold FREESCALE harmless against any and all claims demands or */
/* actions by anyone on account of any damage,or injury, whether commercial, contractual, or        */
/* tortuous, rising directly or indirectly as a result of the advise or assistance supplied CUSTOMER*/ 
/* in connectionwith product, services or goods supplied under this Agreement.                      */
/*                                                                                                  */
/****************************************************************************************************/

/*****************************************************************************************************
* Include files
*****************************************************************************************************/
#include "common.h"
#include "vectors.h"
#include "isr.h"
#include "lptmr.h"
/*****************************************************************************************************
* Declaration of module wide FUNCTIONs - NOT for use in other modules
*****************************************************************************************************/
void LLWU_Init(void);
void disable_uart(void);
void SW_LED_Init(void);
void init_ADC16(void);
/*****************************************************************************************************
* Definition of module wide MACROs / #DEFINE-CONSTANTs - NOT for use in other modules
*****************************************************************************************************/

/*****************************************************************************************************
* Declaration of module wide TYPEs - NOT for use in other modules
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of module wide VARIABLEs - NOT for use in other modules
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs - NOT for use in other modules
*****************************************************************************************************/

/*****************************************************************************************************
* Code of project wide FUNCTIONS
*****************************************************************************************************/


#include "common.h"
#include "smc.h"
#include "adc16.h"
#include "llwu.h"

#define ADC
// Global Variables
extern signed char result[512];  // Holds accelerometer readings
signed char avgtilt = 0;  // Holds the value for the average tilt of the board at startup
tADC_Config Master_Adc_Config;
extern uint32 __VECTOR_RAM[];

void ram_main(void);
char nmi_wake_up_flag;
char llwu_wake_up_flag;

uint32 converting = 0;
uint32 result0A = 0;
uint32 dummyread;

void main (void)
{
#ifndef RAM16 
        int i;
        int counter = 0;
#endif
        
#ifdef KEIL
	start();
#endif
        
                
        LLWU_Init();
       
        SW_LED_Init(); 
        LED0_ON;
        LED1_ON;
        LED2_ON;
        LED3_ON;

        // Dynamic interrupt vector modification whilst those interrupts are disabled
//        __VECTOR_RAM[INT_ADC0] = (uint32)adc0_isr;  // plug isr into vector table in case not there already
        
         
        if ((RCM_SRS0 & RCM_SRS0_POR_MASK) || (RCM_SRS0 & RCM_SRS0_PIN_MASK))
        {
            printf("\nRunning VLPR LLS ADC Demo Project");
//           ch = in_char();
//	   out_char(ch);
  //       disable_uart();
        
           init_ADC16();            
            
        }
        lptmr_init(100,  LPTMR_USE_LPOCLK); 
        
        /* Enable LPTMR wakeup interrupt */
        enable_irq(LPTMR_irq_no);
        
        /* Enable ADC ready for this interrupt. */
//        enable_irq(ADC0_irq_no) ;    
        
        EnableInterrupts;
           
        //Set the STOPM field to 0b011 for LLS mode  
        SMC_PMCTRL |= SMC_PMCTRL_STOPM(0x3) ; 
        dummyread=SMC_PMCTRL;
      
        // Set the SLEEPDEEP bit to enable deep sleep mode (STOP) 
        SCB_SCR |= SCB_SCR_SLEEPDEEP_MASK;	         
#ifdef RAM16
       ram_main();        
#else
       while(1)
        {            
            // WFI instruction will start entry into LLS mode
          counter++;
	  if (counter & 0x0001)
	   	      F_LED0_ON;
	  else
	   	      F_LED0_OFF;
	  if (counter & 0x0002)
	   	      F_LED3_ON;
	  else
	   		  F_LED3_OFF;
	  if (counter & 0x0004)
	   	      F_LED2_ON;
	  else
	   	      F_LED2_OFF;
          if (counter & 0x0008)
	   	      F_LED1_ON;
          else
        	      F_LED1_OFF;
	       
	  asm("WFI");
          for (i=0;i<1000;i++){
             if (ADC0_SC2 & ADACT_ACTIVE)
                F_LED2_TOGGLE;
             else
                break;
          }
          result0A = ADC0_RA ;    
        }
#endif       
}
/********************************************************************/
__ramfunc void ram_main(void)
{
        int i;
        SIM_FCFG1 = SIM_FCFG1_FLASHDIS_MASK;
        while(1)
        {            
            // WFI instruction will start entry into LLS mode
            F_LED0_OFF;
	    asm("WFI");
            for (i=0;i<1000;i++){
              if (ADC0_SC2 & ADACT_ACTIVE)
                F_LED2_TOGGLE;
              else
                break;
            }
            result0A = ADC0_RA ;    
        }
}
/*******************************************************************************************************/
void LLWU_Init(void)
{
    enable_irq(LLWU_irq_no);
//    llwu_configure(0x0080/*PTC3*/, LLWU_PIN_RISING, 0x1);
    llwu_configure(0x0000, LLWU_PIN_RISING, 0x1);
}

/********************************************************************/



/**********************************************************************/
void disable_uart(void)
{
  	/* Enable the pins for the selected UART */
      if (TERM_PORT_NUM == 0){
         PORTA_PCR14 = PORT_PCR_MUX(0); // 0); // Analog is alt0 function for this pin
         PORTA_PCR15 = PORT_PCR_MUX(0x0); // Analog is alt0 function for this pin}
         SIM_SCGC4 &= SIM_SCGC4_UART0_MASK;
      } else if (TERM_PORT_NUM == 1) {
         PORTC_PCR4 = PORT_PCR_MUX(0x0);
         PORTC_PCR3 = PORT_PCR_MUX(0x0);
         SIM_SCGC4 &= SIM_SCGC4_UART1_MASK;
      } else if (TERM_PORT_NUM == 2) {
         PORTD_PCR3 = PORT_PCR_MUX(0x0);
         PORTD_PCR2 = PORT_PCR_MUX(0x0);
         SIM_SCGC4 &= SIM_SCGC4_UART2_MASK;
      }     
 
}
/*******************************************************************************************************/
void SW_LED_Init(void)
{
     SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTA_MASK ; /* PORTC clock enablement */ 
   LED0_EN; // (PORTA_PCR16 = PORT_PCR_MUX(1))
   GPIOA_PDDR |= (1<<16);
   LED1_EN; // (PORTA_PCR17 = PORT_PCR_MUX(1))
   GPIOA_PDDR |= (1<<17);
   LED2_EN; // (PORTB_PCR8 = PORT_PCR_MUX(1))
   GPIOB_PDDR |= (1<<8);
   LED3_EN; // (PORTA_PCR5 = PORT_PCR_MUX(1))
   GPIOA_PDDR |= (1<<5);
       
}

void init_ADC16(void){
  
          // Turn on the ADC0 clock as well as the PDB clocks to test ADC triggered by PDB
            SIM_SCGC6 |= (SIM_SCGC6_ADC0_MASK );
//            SIM_SCGC6 |= SIM_SCGC6_PDB_MASK ; pdb
            PMC_REGSC |= PMC_REGSC_BGBE_MASK ;  

               
            // setup the initial ADC default configuration              
            Master_Adc_Config.CONFIG1  = ADLPC_LOW
              | ADC_CFG1_ADIV(ADIV_1)
              | ADLSMP_LONG
              | ADC_CFG1_MODE(MODE_16)
              | ADC_CFG1_ADICLK(ADICLK_BUS);
            Master_Adc_Config.CONFIG2  = MUXSEL_ADCA
              | ADACKEN_DISABLED
              | ADHSC_HISPEED
              | ADC_CFG2_ADLSTS(ADLSTS_20) ;
            Master_Adc_Config.COMPARE1 = 0x1234u ;                 // can be anything
            Master_Adc_Config.COMPARE2 = 0x5678u ;                 // can be anything
            // since not using
            // compare feature
            Master_Adc_Config.STATUS2  = ADTRG_SW
              | ACFE_DISABLED
              | ACFGT_GREATER
              | ACREN_DISABLED
              | DMAEN_DISABLED
              | ADC_SC2_REFSEL(REFSEL_EXT);
            
            Master_Adc_Config.STATUS3  = CAL_OFF
              | ADCO_SINGLE
              | !AVGE_ENABLED
              | ADC_SC3_AVGS(AVGS_4);
            
            Master_Adc_Config.STATUS1A = !AIEN_ON | DIFF_SINGLE | ADC_SC1_ADCH(31);
              
            // Configure ADC as it will be used, but becuase ADC_SC1_ADCH is 31,
            // the ADC will be inactive.  Channel 31 is just disable function.
            // There really is no channel 31.
            
            ADC_Config_Alt(ADC0_BASE_PTR, &Master_Adc_Config);  // config ADC
              
            
            // Calibrate the ADC in the configuration in which it will be used:
            ADC_Cal(ADC0_BASE_PTR);                    // do the calibration
            
            // The structure still has the desired configuration.  So restore it.
            // Why restore it?  The calibration makes some adjustments to the
            // configuration of the ADC.  The are now undone:
            
            // config the ADC again to desired conditions
            ADC_Config_Alt(ADC0_BASE_PTR, &Master_Adc_Config);
}

  