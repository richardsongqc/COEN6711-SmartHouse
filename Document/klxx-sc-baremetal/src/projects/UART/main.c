//--------------------------------------------------------------------------------
// Name: main.c
//--------------------------------------------------------------------------------
// Company: Concordia University
// Author : MohammadHossein Askari-Hemmat
// 
// Create Date:    11/05/2014 
// Design Name: 	 UART 
// Target Devices: KL25Z128VLK4
// Platform: FRDM-KL25Z (Freescale Freedom Development Platform for Kinetis)
// Compiler Versions: Keil 5.1
// Version: 1.0
// Prject Description:
// In this project ADC0 is configured and pin PE20 is assigned to ADC0 module.
// After initializing ADC, the samples are read by Read_ADC(ADC0_BASE_PTR)
// command and then by mapping it to 3.3 the mapped value is sent to UART0 
// UART0 is configured for 57600bps 
//----------------------------------------------------------------------------
#include "MKL25Z4.h"
#include "system_MKL25Z4.h"
//#include <core_cm0plus.h>

#include "freedom.h"

#include "MemMapPtr_KL25Z4.h"
#include "uart.h"
#include "mcg.h"
//#include "Delay.h"

#include "..\..\cpu\headers\CMSIS\MKL25Z4\Include\MKL25Z4.h"
#include <core_cm0plus.h>
//========================================================================
// Delay function:
void delay()
{
	int i=0;
	for (i=0;i<1000000;i++)
	{
	}
}

uint32_t msTicks = 0; /* Variable to store millisecond ticks */
void SysTick_Handler(void) 
{ 								// SysTick interrupt Handler.
	msTicks++; 			// See startup file startup_LPC17xx.s for SysTick vector */
}

int main(void)
{
	//=============================================================================
	// Local variables:
	char i;
	//=============================================================================
	// Configuring clock:
	i=pll_init(8000000, LOW_POWER, CRYSTAL, PLL0_PRDIV, PLL0_VDIV, MCGOUT);
	if (i!= 0) { while(1);} // Make sure that the PLL is locked on the correct Frequency
	i=what_mcg_mode();
	if (i!= PEE) { while(1);} // Make sure that the PEE mode is enabled
	SystemCoreClockUpdate(); // Update the System Core Clock with the new MCG mode
	i=what_mcg_mode();
	if (i!= PEE) { while(1);} // Make sure that the PEE mode is enabled	
	//=============================================================================
	// SysTick Configuration:
	SysTick_Config(SystemCoreClock/1000); /* Generate interrupt each 1 ms */
	//=============================================================================
	// Configure Uart0:
	uart0_init(UART0_BASE_PTR,48000,TERMINAL_BAUD); // Initializing Uart0 
	//=============================================================================
	while(1)
	{
		printf("Hello World!\n\r");
		delay(); // delay for 1s
	}
}
