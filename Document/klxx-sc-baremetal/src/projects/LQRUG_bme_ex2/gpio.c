#include "common.h"
#include "gpio.h"
#include "BME.h"

void Init_GPIO(void)
{	
  	PORTA_PCR5 &= ~PORT_PCR_MUX_MASK;
	PORTA_PCR5 |= PORT_PCR_MUX(1);	//connects to orange LED, indicate ADC conversion complete
	
	PORTA_PCR16 &= ~PORT_PCR_MUX_MASK;
	PORTA_PCR16 |= PORT_PCR_MUX(1);	//connects to yellow LED, indicate LPTMR timeout
	
	GPIOA_PDDR |= (1<<5)|(1<<16);	//set as output  
}

void Init_BME_GPIO(void)
{	
	BME_BFI_W(&PORTA_PCR5, 1<<PORT_PCR_MUX_SHIFT, PORT_PCR_MUX_SHIFT, 3);	//connects to orange LED, indicate ADC conversion complete

	BME_BFI_W(&PORTA_PCR16, 1<<PORT_PCR_MUX_SHIFT, PORT_PCR_MUX_SHIFT, 3);	//connects to yellow LED, indicate LPTMR timeout

	BME_OR_W(&GPIOA_PDDR, (1<<5)|(1<<16));	//set as output
	
}

