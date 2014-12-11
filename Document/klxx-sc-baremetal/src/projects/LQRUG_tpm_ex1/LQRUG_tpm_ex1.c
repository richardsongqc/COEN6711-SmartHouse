/*
 * File:		LQRUG_tpm_ex1.c
 * Purpose:		Main process
 * Description: - TPM0 used in PWM mode,
 *              - both CH1 (PTC2) and CH2 (PTC3) outputs used,
 *              - CH0 of TPM0 configured as normal PWM (high-true pulses)
 *              - CH1 of TPM0 configured as inverted PWM (low-true pulses)
 *              - duty cycle of PWM signal is set by UART0
 *              - TPM1 used in input capture mode, 
 *              - both CH0 (PTA12) and CH1 (PTA13) inputs used,
 *              - both channels configured to capture rising edge
 *              - if output channel of TPM0 module is interconnected with both of
 *                TMP1 (PTC2<->PTA12 and PTC2<->PTA13) pulse width can be measured.
 *              - NOTE: if PTA12 and PTA13 are not connected to the source of 
 *                pulses, pulse witdth measurement is not possible.
 */

#include "common.h"
#include "uart.h"

// this represents 1/10 of TMP0_MOD value
#define TPM_MOD_1TO10    0x1E0

// Local function prototypes
void SIM_Init(void);
void PORT_Init(void);
void TPM0_Init(void);
void TPM1_Init(void);

// Isr prototypes
void tpm0_isr(void);
void tpm1_isr(void);

// global variables definition
static volatile uint16 u16PWMDuty;
static volatile int16 i16PulseWidth ;


/********************************************************************/
int main (void)
{
  char ch;

  printf("\nRunning the LQRUG_tpm_ex1 project.\n");

  SIM_Init();
  PORT_Init();
  TPM0_Init();
  TPM1_Init();

  while(1)
  {
    printf("\nChange PWM duty: 0-9 (0-90%%)\n");
    ch = in_char();
    u16PWMDuty = TPM_MOD_1TO10*(ch - 0x30);
    out_char(ch);
    printf("\nPress any key to detect pulse width");
    ch = in_char();
    printf("\nMeasured Pulse width: %d TPM1 cycles\n\n", i16PulseWidth);
  } 
}
/********************************************************************/

/***************************************************************************//*!
 * @brief   SIM module initialization.
 ******************************************************************************/
void SIM_Init(void)
{
  SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);
  SIM_SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK;
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK| SIM_SCGC5_PORTC_MASK;
  SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK| SIM_SCGC6_TPM1_MASK;
}

/***************************************************************************//*!
 * @brief   PORT module initialization.
 ******************************************************************************/
void PORT_Init(void)
{
  PORTA_PCR12 = PORT_PCR_ISF_MASK |PORT_PCR_MUX(0x3);
  PORTA_PCR13 = PORT_PCR_ISF_MASK |PORT_PCR_MUX(0x3);
  PORTC_PCR2 = PORT_PCR_ISF_MASK |PORT_PCR_MUX(0x4)| PORT_PCR_DSE_MASK;
  PORTC_PCR3 = PORT_PCR_ISF_MASK |PORT_PCR_MUX(0x4)| PORT_PCR_DSE_MASK;
}

/***************************************************************************//*!
 * @brief   TPM0 module initialization.
 ******************************************************************************/
void TPM0_Init(void)
{
  enable_irq(17); set_irq_priority(17, 3);
  TPM0_CNT = 0;
  TPM0_MOD = 0x12C0;
  TPM0_SC = TPM_SC_TOIE_MASK|TPM_SC_CMOD(1);
  TPM0_C1SC = TPM_CnSC_MSB_MASK| TPM_CnSC_ELSB_MASK;
  TPM0_C1V = 0x00;
  TPM0_C2SC = TPM_CnSC_MSB_MASK| TPM_CnSC_ELSA_MASK;
  TPM0_C2V = 0x00;
}

/***************************************************************************//*!
 * @brief   TPM1 module initialization.
 ******************************************************************************/
void TPM1_Init(void)
{
  enable_irq(18); set_irq_priority(18, 1);
  TPM1_CONF = TPM_CONF_TRGSEL(8)|TPM_CONF_CSOO_MASK|TPM_CONF_CSOT_MASK;
  TPM1_CNT = 0;
  TPM1_MOD = 0x2BC0;
  TPM1_SC = TPM_SC_TOIE_MASK|TPM_SC_CMOD(1);
  TPM1_C0SC = TPM_CnSC_ELSA_MASK;
  TPM1_C0V = 0x00;
  TPM1_C1SC = TPM_CnSC_ELSB_MASK;
  TPM1_C1V = 0x00;
}

/***************************************************************************//*!
 * @brief   TPM0 overflow interrupt service routine.
 ******************************************************************************/
void tpm0_isr(void)
{
  TPM0_SC |= TPM_SC_TOF_MASK;
  TPM0_C1V = (uint16)u16PWMDuty;
  TPM0_C2V = (uint16)u16PWMDuty; 
}

/***************************************************************************//*!
 * @brief   TPM1 overflow interrupt service routine.
 ******************************************************************************/
void tpm1_isr(void)
{
  TPM1_SC |= TPM_SC_TOF_MASK;
  i16PulseWidth =  (int16)TPM1_C1V;
  i16PulseWidth -= (int16)TPM1_C0V;
  TPM1_C0SC |= TPM_CnSC_CHF_MASK;
  TPM1_C1SC |= TPM_CnSC_CHF_MASK;
}
