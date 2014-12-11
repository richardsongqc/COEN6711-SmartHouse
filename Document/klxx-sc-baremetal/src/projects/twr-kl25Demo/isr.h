/******************************************************************************
* File:    isr.h
* Purpose: Define interrupt service routines referenced by the vector table.
* Note: Only "vectors.c" should include this header file.
******************************************************************************/

#ifndef __ISR_H
#define __ISR_H 1

#ifdef KEIL
#define DMA0_IRQHandler								DMA0_IRQHandler
#define DMA1_IRQHandler								DMA1_IRQHandler
#define DMA2_IRQHandler								DMA2_IRQHandler
#define DMA3_IRQHandler								DMA3_IRQHandler
#define Reserved20_IRQHandler					Reserved20_IRQHandler
#define FTFA_IRQHandler								FTFA_IRQHandler
#define LVD_LVW_IRQHandler						LVD_LVW_IRQHandler
#define LLW_IRQHandler								LLW_IRQHandler
#define I2C0_IRQHandler								I2C0_IRQHandler
#define I2C1_IRQHandler								I2C1_IRQHandler
#define SPI0_IRQHandler								SPI0_IRQHandler
#define SPI1_IRQHandler								SPI1_IRQHandler
#define UART0_IRQHandler							UART0_IRQHandler
#define UART1_IRQHandler							UART1_IRQHandler
#define UART2_IRQHandler							UART2_IRQHandler
#define adc0_isrv											ADC0_IRQHandler								
#define CMP0_IRQHandler								CMP0_IRQHandler
#define TPM0_IRQHandler								TPM0_IRQHandler
#define TPM1_IRQHandler								TPM1_IRQHandler
#define TPM2_IRQHandler								TPM2_IRQHandler
#define rtc_isrv											RTC_IRQHandler								
#define rtc_isrv_seconds							RTC_Seconds_IRQHandler	
#define Pit1_isrv											PIT_IRQHandler								
#define Reserved39_IRQHandler					Reserved39_IRQHandler
#define USB_ISR												USB0_IRQHandler								
#define DAC0_IRQHandler								DAC0_IRQHandler
#define TSI_ISR												TSI0_IRQHandler								
#define MCG_IRQHandler								MCG_IRQHandler
#define LPTimer_IRQHandler						LPTimer_IRQHandler
#define Reserved45_IRQHandler					Reserved45_IRQHandler
#define PORTA_IRQHandler							PORTA_IRQHandler
#define PORTD_IRQHandler							PORTD_IRQHandler


#else
extern void Pit1_isrv(void);
extern void TSI_ISR(void);
extern void adc0_isrv(void);
extern void rtc_isrv(void);
extern void rtc_isrv_seconds(void);
extern void USB_ISR (void);


#undef  VECTOR_036
#define VECTOR_036 rtc_isrv

#undef  VECTOR_037
#define VECTOR_037 rtc_isrv_seconds



#undef  VECTOR_038
#define VECTOR_038 Pit1_isrv

#undef  VECTOR_042
#define VECTOR_042  TSI_ISR



#undef  VECTOR_031
#define VECTOR_031 adc0_isrv


#undef VECTOR_040
#define VECTOR_040 USB_ISR
#endif



#endif  //__ISR_H

/* End of "isr.h" */
