/*
 * main.h
 *
 *  Created on: Oct 4, 2012
 *      Author: RXAA60
 */

#ifndef MAIN_H_
#define MAIN_H_

/* function prototypes */

void enable_irq (int irq);
void disable_irq (int irq);
void SW_LED_Init(void);
void porta_isr(void);
void set_4_lp(unsigned char outof_lp);
/*********************************************************/
/* if you do not use any of these defines you will 
 * stay in FEI at default SIM_CLKDIV settings */
/*********************************************************/
/* define USE_VLPR to run the MCU in BLPI at 4 or 2 MHZ */
#define USE_VLPR
#define USE_ADC
#define USE_LPTMR
#define USE_RTC
/*********************************************************/
/* set this define to set the 
 * SIM_CLKDIV1 = (   SIM_CLKDIV1_OUTDIV1(0)
 *                 | SIM_CLKDIV1_OUTDIV4(4) ) 
 */
#define USE_4MHZ_CORE
/* OR - set this define to set the 
 *  SIM_CLKDIV1 = (   SIM_CLKDIV1_OUTDIV1(1)
 *                  | SIM_CLKDIV1_OUTDIV4(2) );  
 *div 3 for flash clk margin */
//#define USE_2MHZ_CORE
/*********************************************************/
#endif /* MAIN_H_ */
