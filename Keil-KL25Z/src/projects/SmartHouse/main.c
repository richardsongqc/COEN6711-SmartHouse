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
#include "dht11.h"



	

int main(void)
{
	//=============================================================================
	// Local variables:
	char i;
	U8 uCmd = 0;
	U8 uRelay1 = 0;
	U8 uRelay2 = 0;
	U16 usRet = 0;
	U16 usDistance = 400;
	U8 uHiHumid = 55;
	U8 uLoHumid = 0;
	U8 uHiTemp  = 25;
	U8 uLoTemp  = 0;
	int nInfrared = 0;
	int nSize = sizeof(float);
	float fDistance = 0;
	
	Initialize();			//Initialize Port for tempreture sensor
	InitialSonic();		//Initiallize Port for ultrosonic and infrared
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
	SysTick_Config( SystemCoreClock/100000); /* Generate interrupt each 1 ms */
	//=============================================================================
	
	// Configure Uart0:
	uart0_init(UART0_BASE_PTR,48000,TERMINAL_BAUD); // Initializing Uart0 
	//=============================================================================
	

	while(1)
	{
		usRet = RecvBuffer(&uCmd);

		switch(uCmd )
		{
		case CMD_PIR             : 
			break; 		       		
		case CMD_ULTRASONIC_RANGE: 
			//GetUltrasonicRange( &usDistance);
			
			break;  		
		case CMD_RELAY_1		 : 
			GetRelay1(&uRelay1);
			break;			
		case CMD_RELAY_2		 : 
			GetRelay2(&uRelay2);
			break; 			
		case CMD_DHT			 : 
			GetDHT( &uHiHumid,  &uLoHumid, &uHiTemp, &uLoTemp );
			break; 	
		default:
			break;
		}
		
		if(GetTemp())		
		{
			SendDHT( (U8)(dht.RecHumI), (U8)dht.RecHumD, (U8)dht.RecTI, (U8)dht.RecTD);  
		}
		
		if (dht.RecTI> uHiTemp) 
		{
			CtrlRelay(1);
			SendRelay1(1);
		}
		else 
		{
			ReleaseRelay(1);
			SendRelay1(0);
		}   
		
		nInfrared = GetInfrared();
		SendPIR( nInfrared );
		if ( nInfrared )			//1 means there is a active moving object, 0 means no moving .
		{
			CtrlRelay(1);					//can be chaged according to the control algorithm.
			SendRelay1(1);
		}
		//else
		//{
		//	ReleaseRelay(1);
		//}
		//SonicDistance();
		
		fDistance = SonicDistance();
		SetDistance(fDistance);
		if( fDistance <20 )  //20cm is the Threshold of Distance, can be changed;
		{
			CtrlRelay(2);
			SendRelay2(1);
		}
		else
		{
			ReleaseRelay(2);
			SendRelay2(0);
		}
		
		
	}
}
