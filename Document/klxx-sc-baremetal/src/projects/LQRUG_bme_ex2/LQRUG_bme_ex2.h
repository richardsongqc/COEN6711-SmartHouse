#ifndef __LQRUG_DEMO_BME_EX2_H
#define __LQRUG_DEMO_BME_EX2_H

//On L2K tower board, potentiometer connects to ADC0_SE4B
#define ADC0_CHANA    4                                    // set to desired ADC0 channel trigger A    
#define ADC0_CHANB    4                                    // set to desired ADC0 channel trigger B    

#define ADC0A_DONE   0x01       
#define ADC0B_DONE   0x02       

#define ADC0_irq_no		15
#define LPTMR0_irq_no	28

/************************************************************************
*
*	Clock Pins on 80LQFP part
*	Pin Location	Main function	GPIO		Tower connection
*	56			RTC_CLKIN	PTC1		32kHz OSC
*	40			EXTAL0		PTA18	8MHz OSC
*	41			XTAL0		PTA19	8MHz OSC
*		
************************************************************************/


/* ADC alternate trigger select */
typedef enum 
{
	EXTRG = 0,
	CMP0_OUT,
	PIT_TRG0 = 4,
	PIT_TRG1, 
	TPM0_OVF = 8,
	TPM1_OVF,
	TPM2_OVF,
	RTC_ALARM = 12,
	RTC_SECONDS,
	LPTMR0_TRG
} ADCTRG_SEL; 

/* LPTMR clock source select */
typedef enum {
	MCGIRCLK = 0,	//MCG output of the slow or fast internal reference clock
	LPOCLK,			//PMC 1kHz output
	ERCLK32K,		//Clock source for some modules that is chosen as OSC32KCLK or RTC_CLKIN, selected by SIM_SOPT1
	OSCERCLK		//System oscillator 32kHz output
} LPTMR_CLKSEL;

/* 32kHz OSC clock select */
typedef enum {
	SYSTEM_OSC = 0,	
	RTC_CLKIN = 2,
	LPOCLK_1KHZ
} ERCLK32K_CLKSEL;

#endif /* __LQRUG_DEMO_BME_EX2_H */
