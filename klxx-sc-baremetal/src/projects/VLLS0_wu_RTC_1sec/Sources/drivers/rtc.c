/*
 * File:        rtc.c
 * Purpose:     Provide common RTC routines
 *
 * Notes:       
 *              
 */


#include "derivative.h"
#include "tower.h"
#include "rtc.h"
#include "main.h"

extern void enable_irq (int irq);
extern void disable_irq (int irq);
extern int counter;
/********************************************************************/
/*
 * Initialize the RTC
 *
 *
 * Parameters:
 *  seconds         Start value of seconds register
 *  alarm           Time in seconds of first alarm. Set to 0xFFFFFFFF to effectively disable alarm
 *  c_interval      Interval at which to apply time compensation can range from 1 second (0x0) to 256 (0xFF)
 *  c_value         Compensation value ranges from -127 32kHz cycles to +128 32 kHz cycles
 *                  80h Time prescaler register overflows every 32896 clock cycles.
 *                  FFh Time prescaler register overflows every 32769 clock cycles.
 *                  00h Time prescaler register overflows every 32768 clock cycles.
 *                  01h Time prescaler register overflows every 32767 clock cycles.
 *                  7Fh Time prescaler register overflows every 32641 clock cycles.
 *  interrupt       TRUE or FALSE
 */

void rtc_init(unsigned int seconds, unsigned int alarm, unsigned int c_interval, unsigned char c_value, unsigned char interrupt)
{
  int i;
  
  /*enable the clock to SRTC module register space*/
  SIM_SCGC6 |= SIM_SCGC6_RTC_MASK;
  
  /*Only VBAT_POR has an effect on the SRTC, RESET to the part does not, so you must manually reset the SRTC to make sure everything is in a known state*/
  /*clear the software reset bit*/
    RTC_CR  = RTC_CR_SWR_MASK;
    RTC_CR  &= ~RTC_CR_SWR_MASK;  
  
    if (RTC_SR & RTC_SR_TIF_MASK){
        RTC_TSR = 0x00000000;   //  this action clears the TIF
     }
  /*Set time compensation parameters*/
  RTC_TCR = RTC_TCR_CIR(c_interval) | RTC_TCR_TCR(c_value);
  
  /*Enable the counter*/
  if (seconds >0) {
     /*Enable the interrupt*/
     if(interrupt >1){
        enable_irq(interrupt);
     }
  /* enable the RTC_CLKIN function */
     SIM_SOPT1 &= SIM_SOPT1_OSC32KSEL_MASK;
     SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL(2); /* Selects the 32 kHz clock source (ERCLK32K) for RTC */
     PORTC_PCR1 =  PORT_PCR_MUX(1); 
     
    RTC_IER |= RTC_IER_TSIE_MASK;
    RTC_SR |= RTC_SR_TCE_MASK;
    /*Configure the timer seconds and alarm registers*/
    RTC_TSR = seconds;

  } else {
    RTC_IER &= ~RTC_IER_TSIE_MASK;
  }
  if (alarm >0) {
    RTC_IER |= RTC_IER_TAIE_MASK;
    RTC_SR |= RTC_SR_TCE_MASK;
    /*Configure the timer seconds and alarm registers*/
    RTC_TAR = alarm;
     /*Enable the interrupt*/
     if(interrupt >1){
        enable_irq(interrupt);
     }
  
    /*Wait to all the 32 kHz to stabilize, refer to the crystal startup time in the crystal datasheet*/
    for(i=0;i<0x600000;i++);
  } else {
    RTC_IER &= ~RTC_IER_TAIE_MASK;
  }
 
}


void rtc_isr(void) 
{   
	F_LED0_OFF;
	if((RTC_SR & RTC_SR_TIF_MASK)== 0x01)
     {
   	   RTC_SR &= 0x07;  //clear TCE, or RTC_TSR can  not be written
   	   RTC_TSR = 0x00000000;  //clear TIF 
           //RTC_IER &= ~RTC_IER_TIIE_MASK;

     }	
    if((RTC_SR & RTC_SR_TOF_MASK) == 0x02)
    {
   	   RTC_SR &= 0x07;  //clear TCE, or RTC_TSR can  not be written
   	   RTC_TSR = 0x00000000;  //clear TOF
    }	 	
    if((RTC_SR & RTC_SR_TAF_MASK) == 0x04)
    {
   	   RTC_TAR += 11;// Write new alarm value, to generate an alarm every second add 1
    }	
    counter++;
    if (counter & 0x0001)
	   	      F_LED3_ON;
    else
	   	      F_LED3_OFF;
    if (counter & 0x0002)
	   	      F_LED2_ON;
    else
     		  F_LED2_OFF;
    if (counter & 0x0004)
     	      F_LED1_ON;
    else
    		  F_LED1_OFF;

    return;
}
