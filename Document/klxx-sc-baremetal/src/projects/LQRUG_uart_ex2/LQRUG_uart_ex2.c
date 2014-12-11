/*
 * File:		LQRUG_uart_ex2.c
 * Purpose:		Main process
 * Description: - MCGIRCLK output enabled for UART,
 *              - UART0 with RDRF interrupt generated (9600kbps, 8-bit, no parity),
 *              - match address operation used to wake up from low power mode,
 *              - to enter VLPS 'e' or 'E' have to be send to UART0,
 *              - to wake up from send 0x81 or 0xAA (address with address mark)
 */

#include "common.h"

// Local function prototypes
void SIM_Init(void);
void MCGIRCLK_OutEnable(void);
void PORT_Init(void);
void UART0_Init(void);
void SMC_VLPS_Allow(void);
void EnterVLPS(void);
void UART0_PutStr(uint8* str);

// Isr prototypes
void uart0_isr(void);

// global variables definition
static volatile uint8 c;

/********************************************************************/
int main (void)
{
  SIM_Init();
  MCGIRCLK_OutEnable();
  PORT_Init();
  UART0_Init();
  SMC_VLPS_Allow();
  
  // All interrupts enabled (no need bacause it is already set)
  //asm(" CPSIE i");
  
  UART0_PutStr("\nRunning the LQRUG_uart_ex1 project.\n");

  while(1)
  {
    UART0_PutStr("\n\rPress  <<e>> to enter VLPS mode with UART0 address match wake up");
    c = 0x00;
    while (!c);
    if ((c == 'e') || (c == 'E')) 
    {
      UART0_PutStr("\n\rVLPS entered …\n\rSend 0x81 or 0xAA to wake up\n\r");
      while( !(UART0_S1&UART_S1_TC_MASK));
      UART0_C2 &= ~(UART0_C2_TE_MASK|UART0_C2_RE_MASK);
      UART0_C4 |= UART0_C4_MAEN1_MASK|UART0_C4_MAEN2_MASK;
      UART0_C2 |= (UART0_C2_TE_MASK|UART0_C2_RE_MASK);
      EnterVLPS();
      UART0_C4 &= ~(UART0_C4_MAEN1_MASK|UART0_C4_MAEN2_MASK);
    }
    else
    {
      UART0_PutStr("\n\rWrong setting. Try again.\n\r");
    }
  } 
}
/********************************************************************/

/***************************************************************************//*!
 * @brief   SIM module initialization.
 ******************************************************************************/
void SIM_Init(void)
{
  SIM_SOPT1   = 0l;
  SIM_SOPT1CFG= 0l;
  SIM_SOPT2 = SIM_SOPT2_UART0SRC(3);
  SIM_SOPT4   = 0l;
  SIM_SOPT5   = 0l;
  SIM_SOPT7   = 0l;
  SIM_SCGC4 = SIM_SCGC4_UART0_MASK;
  SIM_SCGC5 = SIM_SCGC5_PORTA_MASK;
  SIM_SCGC6 = SIM_SCGC6_FTF_MASK;
  SIM_SCGC7   = 0;
}

/***************************************************************************//*!
 * @brief   MCGIRCLK output enabling.
 ******************************************************************************/
void MCGIRCLK_OutEnable(void)
{
  MCG_C1 |= MCG_C1_IRCLKEN_MASK|MCG_C1_IREFSTEN_MASK;
  MCG_SC &= ~MCG_SC_FCRDIV(7);
  MCG_C4 |= MCG_C4_FCTRIM(0xA);
  MCG_C2 |= MCG_C2_IRCS_MASK;
  while (!(MCG_S & MCG_S_IRCST_MASK));
}

/***************************************************************************//*!
 * @brief   PORT module initialization.
 ******************************************************************************/
void PORT_Init(void)
{
  PORTA_PCR14 = PORT_PCR_ISF_MASK|PORT_PCR_MUX(0x3);
  PORTA_PCR15 = PORT_PCR_ISF_MASK|PORT_PCR_MUX(0x3);
}

/***************************************************************************//*!
 * @brief   UART0 module initialization.
 ******************************************************************************/
void UART0_Init(void)
{
  UART0_C2 &= ~ (UART0_C2_TE_MASK| UART0_C2_RE_MASK);
  enable_irq(12); set_irq_priority(12, 3);
  UART0_BDH = 0x00;
  UART0_BDL = 0x1A;
  UART0_C4 = 0x0F;
  UART0_C1 = 0x00;
  UART0_C3 = 0x00;
  UART0_MA1 = 0x81;
  UART0_MA2 = 0xAA;
  UART0_C2 = UART0_C2_RIE_MASK;
  UART0_S1 |= 0x1F;
  UART0_S2 |= 0xC0;
  UART0_C2 |= UART0_C2_TE_MASK| UART0_C2_RE_MASK;
}

/***************************************************************************//*!
 * @brief   Very low power modes enableing.
 ******************************************************************************/
void SMC_VLPS_Allow(void)
{
  SMC_PMPROT = SMC_PMPROT_AVLP_MASK;
}

void EnterVLPS(void)
{
     SMC_PMCTRL |= SMC_PMCTRL_STOPM(2);
     SCB_SCR |= SCB_SCR_SLEEPDEEP_MASK;
     asm("WFI");
}

/***************************************************************************//*!
 * @brief   Send string over UART0.
 ******************************************************************************/
void UART0_PutStr(uint8* str)
{
    uint16 i=0;
    while(str[i] != 0)
    {
        while( !(UART0_S1&UART_S1_TDRE_MASK));
        UART0_D = str[i];
        i++;
    }
}

/***************************************************************************//*!
 * @brief   UART0 read data register full interrupt service routine.
 ******************************************************************************/
void uart0_isr(void)
{
  if (UART0_S1&UART_S1_RDRF_MASK)
  {
    c = UART0_D;
  }
}
