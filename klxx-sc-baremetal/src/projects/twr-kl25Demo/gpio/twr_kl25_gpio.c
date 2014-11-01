/****************************************************************************************************/
/**
Copyright (c) 2011 Freescale Semiconductor
Freescale Confidential Proprietary
\file       twr_kl25_gpio.c
\brief      
\author     
\author     
\version    1.0
\date       Sep 26, 2011
*/


#include "twr_kl25_gpio.h"
#include "common.h"

void  Twr_gpio_init(void)
{
  
#ifdef LED1
  LED1_PCR = PCR_OUTPUT_CONFIG;
  LED1_OFF;
  LED1_OUTPUT_EN;
#endif
    
#ifdef LED2
  LED2_PCR = PCR_OUTPUT_CONFIG;
  LED2_OFF;
  LED2_OUTPUT_EN;
#endif

#ifdef LED3
  LED3_PCR = PCR_OUTPUT_CONFIG;
  LED3_OFF;
  LED3_OUTPUT_EN;
#endif
  
#ifdef LED4
  LED4_PCR = PCR_OUTPUT_CONFIG;
  LED4_OFF;
  LED4_OUTPUT_EN;
#endif
  
#ifdef SW1
   SW1_PCR = PCR_INPUT_CONFIG;
   SW1_INPUT_EN;
#endif
  
#ifdef SW2
   SW2_PCR = PCR_INPUT_CONFIG;
   SW2_INPUT_EN;
#endif

   
 
}

char input_rise(char input,char *mem)
{
 char result=0;
 if (input && !*mem) result=1;
 *mem = input;
 return result;
}

char input_fall(char input,char *mem)
{
 char result=0;
 if (!input && *mem) result=1;
 *mem = input;
 return result;
}
