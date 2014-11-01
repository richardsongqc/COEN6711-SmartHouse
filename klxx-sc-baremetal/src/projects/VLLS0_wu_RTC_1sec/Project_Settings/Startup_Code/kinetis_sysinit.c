/*
 *    kinetis_sysinit.c - Default init routines for Flycatcher
 *                     		Kinetis ARM systems
 *    Copyright © 2012 Freescale semiConductor Inc. All Rights Reserved.
 */
 
#include "kinetis_sysinit.h"
#include "derivative.h"
#include "tower.h"
#include "llwu.h"
#include "rtc.h"
#include "mcg.h"
#include "main.h"
#include "smc.h"
#include "lptmr.h"
#include "adc16.h"

/**
 **===========================================================================
 **  External declarations
 **===========================================================================
 */
#if __cplusplus
extern "C" {
#endif
extern uint32_t __vector_table[];
extern 	int counter;
extern unsigned long _estack;
extern void __thumb_startup(void);
extern void rtc_isr(void);
extern void llwu_isr(void);
extern void porta_isr(void);
extern void lptmr_isr(void);

#if __cplusplus
}
#endif

/**
 **===========================================================================
 **  Default interrupt handler
 **===========================================================================
 */
void Default_Handler()
{
	__asm("bkpt");
}

/**
 **===========================================================================
 **  Reset handler
 **===========================================================================
 */
void __init_hardware()
{
	int mcg_clk_hz;
	SCB_VTOR = (uint32_t)__vector_table; /* Set the interrupt vector table position */
	
	// Disable the Watchdog because it may reset the core before entering main().
	SIM_COPC = SIM_COPC_COPT(3) ;
	/* one time write - allow VLPR and LLS and VLLS */
	SMC_PMPROT =  SMC_PMPROT_AVLLS_MASK |
	              SMC_PMPROT_ALLS_MASK  |    
	              SMC_PMPROT_AVLP_MASK;

#ifdef USE_4MHZ_CORE
	/* Internal Fast IRC is 4 MHz so div by 2 for sysclk and div 3 for flash clock*/
	/* results in Core clock at 4 MHz and flash clock at 800 KHz Hz */
	SIM_CLKDIV1 = (   SIM_CLKDIV1_OUTDIV1(0)
                    | SIM_CLKDIV1_OUTDIV4(4) ); // div 5 for flash clk margin
#endif
#ifdef USE_2MHZ_CORE
	/* Internal Fast IRC is 4 MHz so div by 2 for sysclk and div 3 for flash clock*/
	/* results in Core clock at 2 MHz and flash clock at 660 KHz Hz */
	SIM_CLKDIV1 = (   SIM_CLKDIV1_OUTDIV1(1)
                    | SIM_CLKDIV1_OUTDIV4(2) ); // div 3 for flash clk margin
#endif

#ifdef USE_VLPR	
	mcg_clk_hz = fei_fbi(fast_irc_freq,FAST_IRC);
	mcg_clk_hz = fbi_blpi(fast_irc_freq,FAST_IRC);

	MCG_C1 |= MCG_C1_IRCLKEN_MASK;
    MCG_C2 |= MCG_C2_IRCS_MASK;
    enter_vlpr();   // get into VLPR 
#endif
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK
                  | SIM_SCGC5_PORTB_MASK
                  | SIM_SCGC5_PORTC_MASK
                  | SIM_SCGC5_PORTD_MASK
                  | SIM_SCGC5_PORTE_MASK );
    /*Configure SW3*/
     PORTA_PCR4 =  PORT_PCR_PS_MASK |
                      PORT_PCR_PE_MASK | 
                      PORT_PCR_PFE_MASK |
                      PORT_PCR_IRQC(10) | 
                     PORT_PCR_MUX(1); /* IRQ Falling edge */  
    /*PTC3(SW4) is configured to wake up MCU from VLLSx and LLS modes, Interrupt*/
    /* Configure SW4 - init for GPIO PTC3/LLWU_P7/UART1_RX/FTM0_CH2/CLKOUT*/
    PORTC_PCR3 = ( PORT_PCR_MUX(1) |
                 PORT_PCR_PE_MASK |
                 PORT_PCR_PFE_MASK |
                 PORT_PCR_PS_MASK);
    SW_LED_Init();
    
	if (RCM_SRS0 & RCM_SRS0_WAKEUP_MASK)
	{
		  /* enable the RTC_CLKIN function */
		     SIM_SOPT1 &= SIM_SOPT1_OSC32KSEL_MASK;
		     SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL(2); /* Selects the 32 kHz clock source (ERCLK32K) for RTC */
		     PORTC_PCR1 =  PORT_PCR_MUX(1); 
		     
	}
		  
	if (RCM_SRS0 & RCM_SRS0_POR_MASK)
		counter = 0;
	
#ifdef USE_RTC
	rtc_init(1, 0, 0, 0, RTCS_irq_no) ;
#endif
#ifdef USE_LPTMR
	lptmr_init(100,  LPTMR_USE_LPOCLK); 
	        
	/* Enable LPTMR wakeup interrupt */
    enable_irq(LPTMR_irq_no);
#endif
	enable_irq(LLWU_irq_no);
#ifdef USE_ADC
	init_ADC16();    
#endif
    llwu_configure(0x0080/*PTC3*/, LLWU_PIN_RISING, 0x81); // enable PTC3 ,LPTMR and RTC SEC
    /* substitute the next line if you want to measure the 
     * static current while in LLS with RTC enabled */
//	llwu_configure(0x0080/*PTC3*/, LLWU_PIN_RISING, 0x00); // enable PTC3 and RTC SEC
//    PORTA_PCR0 =  PORT_PCR_MUX(0); // Disable Debug 
//    PORTA_PCR3 =  PORT_PCR_MUX(0); // Disable Debug  
}

/* Weak definitions of handlers point to Default_Handler if not implemented */
void NMI_Handler() __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler() __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler() __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler() __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler() __attribute__ ((weak, alias("Default_Handler")));

void DMA0_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void DMA1_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void DMA2_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void DMA3_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void MCM_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void FTFL_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void PMC_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void LLWU_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void I2C0_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void I2C1_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void SPI0_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void SPI1_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void UART0_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void UART1_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void UART2_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void ADC0_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void CMP0_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void FTM0_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void FTM1_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void FTM2_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void RTC_Alarm_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void RTC_Seconds_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void PIT_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void USBOTG_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void DAC0_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void TSI0_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void MCG_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void LPTimer_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void PORTA_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void PORTD_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));

/* The Interrupt Vector Table */
void (* const InterruptVector[])() __attribute__ ((section(".vectortable"))) = {
    /* Processor exceptions */
    (void(*)(void)) &_estack,
    __thumb_startup,
    NMI_Handler,
    HardFault_Handler,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    SVC_Handler,
    0,
    0,
    PendSV_Handler,
    SysTick_Handler,

    /* Interrupts */
    DMA0_IRQHandler, /* DMA Channel 0 Transfer Complete and Error */
    DMA1_IRQHandler, /* DMA Channel 1 Transfer Complete and Error */
    DMA2_IRQHandler, /* DMA Channel 2 Transfer Complete and Error */
    DMA3_IRQHandler, /* DMA Channel 3 Transfer Complete and Error */
    MCM_IRQHandler, /* Normal Interrupt */
    FTFL_IRQHandler, /* FTFL Interrupt */
    PMC_IRQHandler, /* PMC Interrupt */
    llwu_isr, /* Low Leakage Wake-up */
    I2C0_IRQHandler, /* I2C0 interrupt */
    I2C1_IRQHandler, /* I2C1 interrupt */
    SPI0_IRQHandler, /* SPI0 Interrupt */
    SPI1_IRQHandler, /* SPI1 Interrupt */
    UART0_IRQHandler, /* UART0 Status and Error interrupt */
    UART1_IRQHandler, /* UART1 Status and Error interrupt */
    UART2_IRQHandler, /* UART2 Status and Error interrupt */
    ADC0_IRQHandler, /* ADC0 interrupt */
    CMP0_IRQHandler, /* CMP0 interrupt */
    FTM0_IRQHandler, /* FTM0 fault, overflow and channels interrupt */
    FTM1_IRQHandler, /* FTM1 fault, overflow and channels interrupt */
    FTM2_IRQHandler, /* FTM2 fault, overflow and channels interrupt */
    RTC_Alarm_IRQHandler, /* RTC Alarm interrupt */
    rtc_isr, /* RTC Seconds interrupt */
    PIT_IRQHandler, /* PIT timer all channels interrupt */
    Default_Handler, /* Reserved interrupt 39/23 */
    USBOTG_IRQHandler, /* USB interrupt */
    DAC0_IRQHandler, /* DAC0 interrupt */
    TSI0_IRQHandler, /* TSI0 Interrupt */
    MCG_IRQHandler, /* MCG Interrupt */
    lptmr_isr, /* LPTimer interrupt */
    Default_Handler, /* Reserved interrupt 45/29 */
    porta_isr, /* Port A interrupt */
    PORTD_IRQHandler /* Port D interrupt */
};

