/*
 * File:		gpio_k40.c
 * Purpose:		LED and Switch Example
 *
 */

#define GPIO_PIN_MASK            0x1Fu
#define GPIO_PIN(x)              (((1)<<(x & GPIO_PIN_MASK)))

#include "common.h"
void delay();

/********************************************************************/

void GPIO_main (void)
{
  printf("Hello World!\n");

  /* Turn on all port clocks */
  SIM_SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;


  //Set PTB11, PTC7, PTC8, and PTC9 to GPIO
  PORTB_PCR11=(0|PORT_PCR_MUX(1));
  PORTC_PCR7=(0|PORT_PCR_MUX(1));
  PORTC_PCR8=(0|PORT_PCR_MUX(1));
  PORTC_PCR9=(0|PORT_PCR_MUX(1));

  //Set PTC5 and PTC13 as GPIO for input using internal pull-ups
  PORTC_PCR5=(0|PORT_PCR_MUX(1)|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK);
  PORTC_PCR13=(0|PORT_PCR_MUX(1)|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK);


  //Set PTB11, PTC7, PTC8, PTC9 as an output
  GPIOB_PDDR = GPIO_POER_POE(GPIO_PIN(11));
  GPIOC_PDDR = GPIO_POER_POE(GPIO_PIN(7) | GPIO_PIN(8) | GPIO_PIN(9));	

  /*
    PTB11 - Blue (D17)
    PTC9 - Green (D14)
    PTC8 - Yellow (D15)
    PTC7 - Orange (D16)
  */

  while(1)
  {
    //Look at status of SW6 on PTC5
    if((GPIOC_PDIR&GPIO_PDIR_PDI(GPIO_PIN(5)))==0) //if pressed...
    {
      GPIOB_PDOR|=GPIO_PDOR_PDO(GPIO_PIN(11)); //Set PTB11 to 1 (turns off LED)
      GPIOC_PDOR|=GPIO_PDOR_PDO(GPIO_PIN(9)); //Set PTC11 to 1 (turns off LED)
    }
    else
    {
      GPIOB_PDOR&=~GPIO_PDOR_PDO(GPIO_PIN(11)); //Set PTB11 to 0 (turns on LED)
      GPIOC_PDOR&=~GPIO_PDOR_PDO(GPIO_PIN(9)); //Set PTC11 to 0 (turns on LED)
    }

    //Look at status of SW7 on PTC13
    if((GPIOC_PDIR&GPIO_PDIR_PDI(GPIO_PIN(13)))==0)  //If pressed...
    {
      GPIOC_PDOR|=GPIO_PDOR_PDO(GPIO_PIN(7) | GPIO_PIN(8)); //Set PTC7, PTC8 to 1 ((turns off LED)
    }
    else
    {
      GPIOC_PDOR&=~GPIO_PDOR_PDO(GPIO_PIN(7) | GPIO_PIN(8)); //Set PTC7, PTC8 to 0 (turns on LED)
    }
  }
}


void delay()
{
  unsigned int i, n, j;
  for(i=0;i<30000;i++)  //delay
  {
    for(n=0;n<1000;n++)
    {
      j=j*j*j;
    }
  }
}
/********************************************************************/
