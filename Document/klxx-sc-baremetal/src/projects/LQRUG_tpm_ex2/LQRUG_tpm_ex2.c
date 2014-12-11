/*
 * File:		LQRUG_tpm_ex2.c
 * Purpose:		Main process
 * Description: - TPM0 used in central PWM mode,
 *              - just CH2 (PTC3) outputs used,
 *              - CH2 of TPM0 configured as normal PWM (high-true pulses)
 *              - CH1 of TPM0 configured as inverted PWM (low-true pulses)
 *              - duty cycle of PWM signal is set from buffer value on DMA request
 *              - DMA0 request set to TPM0 overflow, 
 *              - DMA source adrress is data buffer,
 *              - DMA destination address is TPM)_C2V
 *              - DMA source is incremented after each request, destination not
 *              - TPM0 and DMA running after VLPS entering,
 */

#include "common.h"

// constants definition
const uint16 au16PwmBuffSin[64] =                                               \
 { 2000, 2199, 2396, 2589, 2776, 2956, 3126, 3285, 3431, 3563, 3680, 3779, 3861,\
   3925, 3969, 3990, 3990, 3990, 3949, 3895, 3823, 3732, 3623, 3499, 3360, 3207,\
   3042, 2867, 2684, 2493, 2298, 2099, 1900, 1701, 1506, 1315, 1132, 957, 792,  \
   639, 500, 376, 267, 176, 104, 50, 15, 15, 15, 30, 74, 138, 220, 319, 436,    \
   568, 714, 873, 1043, 1223, 1410, 1603, 1800, 2000};

const uint16 au16PwmBuffTrg[64] =                                               \
 { 0, 133, 266, 399, 533, 667, 800, 933, 1066, 1200, 1333, 1466, 1600, 1733,    \
   1866, 2000, 2133, 2266, 2400, 2533, 2667, 2800, 2933, 3067, 3200, 3333,      \
   3466, 3600, 3733, 3867, 4000, 0, 133, 266, 399, 533, 667, 800, 933, 1066,    \
   1200, 1333, 1466, 1600, 1733, 1866, 2000, 2133, 2266, 2400, 2533, 2667,      \
   2800, 2933, 3067, 3200, 3333, 3466, 3600, 3733, 3867, 4000};

const uint16 au16PwmBuffSqr[64] =                                               \
 { 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000,\
   4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000,\
   4000, 4000, 4000, 4000, 4000, 4000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// Local function prototypes
void SIM_Init(void);
void MCGIRCLK_OutEnable(void);
void PORT_Init(void);
void TPM0_Init(void);
void DMA0_Init(void);
void SMC_VLPS_Allow(void);

// Isr prototypes
void porta_isr(void);
void dma0_isr(void);

// global variables definition
static volatile uint32 i = 0;
#pragma location = 0x20000000
static volatile uint16 au16PwmDuties[64];
uint16  *p_au16PwmDuties;
uint8 u8BuffSelect = 0;

/********************************************************************/
int main (void)
{
  char ch;

  printf("\nRunning the LQRUG_tpm_ex2 project.\n");

  SIM_Init();
  MCGIRCLK_OutEnable();
  PORT_Init();
  TPM0_Init();
  DMA0_Init();
  SMC_VLPS_Allow();
  
  // All interrupts enabled (no need bacause it is already set)
  //asm(" CPSIE i");

  while (1)
  {
    TPM0_C2V = 0;
    DMAMUX0_CHCFG0 &= ~DMAMUX_CHCFG_ENBL_MASK;
    if (!u8BuffSelect)
    {
      u8BuffSelect = 3;
    }
    switch(u8BuffSelect)
    {
      case 1: p_au16PwmDuties = (uint16*)au16PwmBuffSin; break;
      case 2: p_au16PwmDuties = (uint16*)au16PwmBuffTrg; break;
      case 3: p_au16PwmDuties = (uint16*)au16PwmBuffSqr; break;
      default: break;
    }
    for (i = 0; i < 64; i++)
    {
      au16PwmDuties[i] = p_au16PwmDuties[i]>>1;
    }
    DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK;
    SMC_PMCTRL |= SMC_PMCTRL_STOPM(2);
    SCB_SCR |= SCB_SCR_SLEEPDEEP_MASK;
    asm("WFI");
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
  SIM_SOPT2 = SIM_SOPT2_TPMSRC(3);
  SIM_SOPT4   = 0l;
  SIM_SOPT5   = 0l;
  SIM_SOPT7   = 0l;
  SIM_SCGC4   = 0l;
  SIM_SCGC5 = SIM_SCGC5_PORTA_MASK| SIM_SCGC5_PORTC_MASK;
  SIM_SCGC6 = SIM_SCGC6_TPM0_MASK| SIM_SCGC6_DMAMUX_MASK|SIM_SCGC6_FTF_MASK;
  SIM_SCGC7   = SIM_SCGC7_DMA_MASK;
}

/***************************************************************************//*!
 * @brief   MCGIRCLK output enableing.
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
  enable_irq(30); set_irq_priority(30, 1);
  PORTA_PCR4 = PORT_PCR_ISF_MASK |PORT_PCR_MUX(0x1)| PORT_PCR_PS_MASK|\
               PORT_PCR_PE_MASK|PORT_PCR_PFE_MASK| PORT_PCR_IRQC(9);
  PORTC_PCR3 = PORT_PCR_ISF_MASK |PORT_PCR_MUX(0x4)| PORT_PCR_DSE_MASK;
}

/***************************************************************************//*!
 * @brief   TPM0 module initialization.
 ******************************************************************************/
void TPM0_Init(void)
{
  TPM0_CNT = 0;
  TPM0_MOD = 0x7D0;
  TPM0_SC = TPM_SC_DMA_MASK|TPM_SC_CPWMS_MASK|TPM_SC_CMOD(1);
  TPM0_C2SC = TPM_CnSC_MSB_MASK|TPM_CnSC_ELSB_MASK;
  TPM0_C2V = 0x00;
}

/***************************************************************************//*!
 * @brief   TPM1 module initialization.
 ******************************************************************************/
void DMA0_Init(void)
{
  DMAMUX0_CHCFG0 = DMAMUX_CHCFG_SOURCE(54);
  enable_irq(0); set_irq_priority(0, 2);
  DMA_SAR0 = (uint32) &au16PwmDuties;
  DMA_DAR0 = (uint32) &TPM0_C2V;
  DMA_DCR0 = DMA_DCR_EINT_MASK| DMA_DCR_ERQ_MASK|DMA_DCR_CS_MASK|\
             DMA_DCR_EADREQ_MASK|DMA_DCR_SINC_MASK|DMA_DCR_SSIZE(2)|\
             DMA_DCR_DSIZE(2)| DMA_DCR_SMOD(4);
  DMA_DSR_BCR0 = DMA_DSR_BCR_BCR(64000);
}

/***************************************************************************//*!
 * @brief   Very low power modes enabling.
 ******************************************************************************/
void SMC_VLPS_Allow(void)
{
  SMC_PMPROT = SMC_PMPROT_AVLP_MASK;
}

/***************************************************************************//*!
 * @brief   PTA4 external interrupt service routine.
 ******************************************************************************/
void porta_isr(void)
{
  register uint32 temp32 = PORTA_ISFR;
  PORTA_ISFR |= temp32;
  u8BuffSelect--;           
}

/***************************************************************************//*!
 * @brief   DMA0 interrupt service routine.
 ******************************************************************************/
void dma0_isr(void)
{
  DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE_MASK;
  DMA_DSR_BCR0 |= DMA_DSR_BCR_BCR(64000);
  u8BuffSelect--;
}

