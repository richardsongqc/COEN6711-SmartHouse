#include "common.h"
#include "lptmr.h"
#include "BME.h"

void lptmr_isr(void)
{
  //GPIOB_PSOR = 0x00080000;           // clear Port B 19 indicate wakeup
	GPIOA_PDOR ^= (1<<16);
  //printf("\n****LPT ISR entered*****\r\n");

  // enable timer
  // enable interrupts
  // clear the flag
  LPTMR0_CSR |=  LPTMR_CSR_TCF_MASK;   // write 1 to TCF to clear the LPT timer compare flag
  LPTMR0_CSR = ( LPTMR_CSR_TEN_MASK | LPTMR_CSR_TIE_MASK | LPTMR_CSR_TCF_MASK  );
   

}
/*******************************************************************************
*
*   PROCEDURE NAME:
*       lptmr_init -
*
*******************************************************************************/
void Lptmr_Init(int count, int clock_source)
{
    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;
	
	SIM_SOPT1 &= ~SIM_SOPT1_OSC32KSEL_MASK;
    SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL(2); // ERCLK32 is RTC OSC CLOCK
	
	PORTC_PCR1 &= ~PORT_PCR_MUX_MASK;
	PORTC_PCR1 |= PORT_PCR_MUX(1);//enable ptc1 alt1 functions to select RTC_CLKIN function
	
	/********************************************************************************* 
	*	On L2K tower board, we use external 32kHz clock instead of 32kHz crystal, so please
	*	don't enable the 32kHz crystal oscillator 
	**********************************************************************************/
	/*
	RTC_CR |= RTC_CR_OSCE_MASK |
              RTC_CR_CLKO_MASK |
              RTC_CR_SC8P_MASK  ;	*/

	LPTMR0_PSR &= ~LPTMR_PSR_PRESCALE_MASK;
    LPTMR0_PSR |=  LPTMR_PSR_PRESCALE(0); // 0000 is div 2
    LPTMR0_PSR |= LPTMR_PSR_PBYP_MASK;  // LPO feeds directly to LPT
    LPTMR0_PSR &= ~LPTMR_PSR_PCS_MASK;
	LPTMR0_PSR |= LPTMR_PSR_PCS(clock_source); // use the choice of clock
	
    if (clock_source== 0)
      printf("\n LPTMR Clock source is the MCGIRCLK \n\r");
    if (clock_source== 1)
      printf("\n LPTMR Clock source is the LPOCLK \n\r");
    if (clock_source== 2)
      printf("\n LPTMR Clock source is the ERCLK32 \n\r");
    if (clock_source== 3)
      printf("\n LPTMR Clock source is the OSCERCLK \n\r");
             
    LPTMR0_CMR = LPTMR_CMR_COMPARE(count);  //Set compare value

    LPTMR0_CSR |=(  LPTMR_CSR_TCF_MASK   // Clear any pending interrupt
                 | LPTMR_CSR_TIE_MASK   // LPT interrupt enabled
                 |!LPTMR_CSR_TPP_MASK   //TMR Pin polarity
                 |!LPTMR_CSR_TFC_MASK   // Timer Free running counter is reset whenever TMR counter equals compare
                 |!LPTMR_CSR_TMS_MASK   //LPTMR0 as Timer
                );
	
}

void Lptmr_BME_Init(int count, int clock_source)
{
	BME_OR_W(&SIM_SCGC5, SIM_SCGC5_LPTMR_MASK);
	
	BME_BFI_W(&SIM_SOPT1, 2<<SIM_SOPT1_OSC32KSEL_SHIFT, SIM_SOPT1_OSC32KSEL_SHIFT, 2);	// ERCLK32 is RTC OSC CLOCK
	
	BME_BFI_W(&PORTC_PCR1, 1<<PORT_PCR_MUX_SHIFT, PORT_PCR_MUX_SHIFT, 3);//enable ptc1 alt1 functions to select RTC_CLKIN function
	
	/********************************************************************************* 
	*	On L2K tower board, we use external 32kHz clock instead of 32kHz crystal, so please
	*	don't enable the 32kHz crystal oscillator 
	**********************************************************************************/
	/*
	RTC_CR |= RTC_CR_OSCE_MASK |
              RTC_CR_CLKO_MASK |
              RTC_CR_SC8P_MASK  ;	*/
	
	BME_BFI_W(&LPTMR0_PSR, 0 << LPTMR_PSR_PRESCALE_SHIFT, LPTMR_PSR_PRESCALE_SHIFT, 4);	//div by 2
	BME_BFI_W(&LPTMR0_PSR, clock_source << LPTMR_PSR_PCS_SHIFT, LPTMR_PSR_PCS_SHIFT, 2);
	BME_OR_W(&LPTMR0_PSR, LPTMR_PSR_PBYP_MASK);	//LPO feeds directly to LPT

    if (clock_source== 0)
      printf("\n LPTMR Clock source is the MCGIRCLK \n\r");
    if (clock_source== 1)
      printf("\n LPTMR Clock source is the LPOCLK \n\r");
    if (clock_source== 2)
      printf("\n LPTMR Clock source is the ERCLK32 \n\r");
    if (clock_source== 3)
      printf("\n LPTMR Clock source is the OSCERCLK \n\r");
             
    LPTMR0_CMR = LPTMR_CMR_COMPARE(count);  //Set compare value
	
	BME_OR_W(&LPTMR0_CSR,  LPTMR_CSR_TCF_MASK   // Clear any pending interrupt
                 | LPTMR_CSR_TIE_MASK   // LPT interrupt enabled
                 |!LPTMR_CSR_TPP_MASK   //TMR Pin polarity
                 |!LPTMR_CSR_TFC_MASK   // Timer Free running counter is reset whenever TMR counter equals compare
                 |!LPTMR_CSR_TMS_MASK   //LPTMR0 as Timer
			);
}

void Lptmr_Start(void)
{
	LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;   //Turn on LPT and start counting
}
