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
#include <MKL25Z4.h>
#include "freedom.h"
#include "mcg.h"
#include "uart.h"
#include "MemMapPtr_KL25Z4.h"
#include <SerialProtocol.h>
#include "Delay.h"


	U8 uPIR 	 = 0;
	U16 usDistance = 2;
	U8 uRelay1   = 0;
	U8 uRelay2   = 0;
	U8 uHiHumid  = 20;
	U8 uLoHumid  = 0;	
	U8 uHiTemp   = 0;	
	U8 uLoTemp   = 0;	
	
int main(void)
{
	//=============================================================================
	// Local variables:
	char i;
	//=============================================================================
	// Configuring clock:
	i=pll_init(8000000, LOW_POWER, CRYSTAL, PLL0_PRDIV, PLL0_VDIV, MCGOUT);
	
	// Make sure that the PLL is locked on the correct Frequency
	if (i!= 0) 
	{ 
		while(1);		
	} 
	
	i=what_mcg_mode();
	
	// Make sure that the PEE mode is enabled
	if (i!= PEE) 
	{ 
		while(1);
	}
	
	SystemCoreClockUpdate(); // Update the System Core Clock with the new MCG mode
	i=what_mcg_mode();
	
	// Make sure that the PEE mode is enabled	
	if (i!= PEE) 
	{ 
		while(1);
	} 
	
	//=============================================================================
	// SysTick Configuration:
	SysTick_Config(SystemCoreClock/1000); /* Generate interrupt each 1 ms */
	//=============================================================================
	
	// Configure Uart0:
	uart0_init(UART0_BASE_PTR,48000,TERMINAL_BAUD); // Initializing Uart0 
	//=============================================================================
	

	
	while(1)
	{
		
		SendPIR(uPIR);								 	
		//print("\n\r");
		Delay(1000); // delay for 1s
		uPIR = ( uPIR == 0 ) ? 1 : 0;
		
		SendUltrasonicRange(usDistance);	
		//print("\n\r");
		Delay(1000); // delay for 1s
		usDistance++;
		if( usDistance > 400 )
		{
			usDistance = 2;
		}
		
		SendRelay1(uRelay1);							
		//print("\n\r");
		Delay(1000); // delay for 1s
		uRelay1 = ( uRelay1 == 0 ) ? 1 : 0;
		
		SendRelay2(uRelay2);	
		//print("\n\r");
		Delay(1000); // delay for 1s
		uRelay2 = ( uRelay2 == 0 ) ? 1 : 0;
		
		
		SendDHT( uHiHumid++, uLoHumid, uHiTemp++, uLoHumid);  
		//print("\n\r");
		Delay(1000); // delay for 1s
		if( uHiHumid > 90 )
		{
			uHiHumid = 20;
		}
		if( uHiTemp > 50 )
		{
			uHiTemp = 0;
		}
		if( uLoHumid > 99 )
		{
			uLoHumid = 0;
		}
		
		//print("===========================\n\r");
	}
}
