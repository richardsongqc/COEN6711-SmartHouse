/**************************************************
 *
 * Copyright 2010 IAR Systems. All rights reserved.
 *
 * $Revision: #1 $
 *
 **************************************************/

;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;
        /*TWR: LK2 specific*/

        MODULE  ?cstartup

        INCLUDE user_config.h

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:ROOT(2)

        EXTERN  __iar_program_start
        PUBLIC  ___VECTOR_RAM
        PUBLIC  __vector_table
        EXTERN  USB_ISR

#if MAX_TIMER_OBJECTS
        EXTERN  Timer_ISR
#endif

        DATA
___VECTOR_RAM
__vector_table
        DCD     sfe(CSTACK)               ; Top of Stack
        DCD     __iar_program_start       ; Reset Handler
        DCD     NMI_Handler               ; NMI Handler
        DCD     HardFault_Handler         ; Hard Fault Handler
        DCD     MemManage_Handler         ; MPU Fault Handler
        DCD     BusFault_Handler          ; Bus Fault Handler
        DCD     UsageFault_Handler        ; Usage Fault Handler
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     SVC_Handler               ; SVCall Handler
        DCD     DebugMon_Handler          ; Debug Monitor Handler
        DCD     0                         ; Reserved
        DCD     PendSV_Handler            ; PendSV Handler
        DCD     SysTick_Handler           ; SysTick Handler
        ; External Interrupts
        DCD     DMA0_IRQHandler           ; 0:
        DCD     DMA1_IRQHandler           ; 1:
        DCD     DMA2_IRQHandler           ; 2:
        DCD     DMA3_IRQHandler           ; 3:
        DCD     0                         ; 4:
        DCD     FTFA_IRQHandler           ; 5:
        DCD     LVD_IRQHandler            ; 6:
        DCD     LLWU_IRQHandler           ; 7:
        DCD     I2C0_IRQHandler           ; 8:
        DCD     I2C1_IRQHandler           ; 9:
        DCD     SPI0_IRQHandler           ;10:
        DCD     SPI1_IRQHandler           ;11:
        DCD     UART0SE_IRQHandler        ;12:
        DCD     UART1SE_IRQHandler        ;13:
        DCD     UART2SE_IRQHandler        ;14:
        DCD     ADC0_IRQHandler           ;15:
        DCD     CMP0_IRQHandler           ;16:
        DCD     FTM0_IRQHandler           ;17:
        DCD     FTM1_IRQHandler           ;18:
        DCD     FTM2_IRQHandler           ;19:
        DCD     RTCA_IRQHandler           ;20:
        DCD     RTCS_IRQHandler           ;21:
#if MAX_TIMER_OBJECTS
        DCD     Timer_ISR                 ;22:
#else
        DCD     PIT_IRQHandler            ;22:  PIT interrupt
#endif
        DCD     0                         ;23:
        DCD     USB_ISR                   ;24:
        DCD     DAC_IRQHandler            ;25:
        DCD     TSI_IRQHandler            ;26:
        DCD     MCG_IRQHandler            ;27:
        DCD     LPTMR_IRQHandler          ;28:
        DCD     0                         ;29:
        DCD     PortA_IRQHandler          ;30:
        DCD     PortD_IRQHandler          ;31:

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;

      PUBWEAK NMI_Handler
      PUBWEAK HardFault_Handler
      PUBWEAK MemManage_Handler
      PUBWEAK BusFault_Handler
      PUBWEAK UsageFault_Handler
      PUBWEAK SVC_Handler
      PUBWEAK DebugMon_Handler
      PUBWEAK PendSV_Handler
      PUBWEAK SysTick_Handler
      PUBWEAK DMA0_IRQHandler
      PUBWEAK DMA1_IRQHandler
      PUBWEAK DMA2_IRQHandler
      PUBWEAK DMA3_IRQHandler
      PUBWEAK FTFA_IRQHandler
      PUBWEAK LVD_IRQHandler
      PUBWEAK LLWU_IRQHandler
      PUBWEAK I2C0_IRQHandler
      PUBWEAK I2C1_IRQHandler
      PUBWEAK SPI0_IRQHandler
      PUBWEAK SPI1_IRQHandler
      PUBWEAK UART0SE_IRQHandler
      PUBWEAK UART1SE_IRQHandler
      PUBWEAK UART2SE_IRQHandler
      PUBWEAK ADC0_IRQHandler
      PUBWEAK CMP0_IRQHandler
      PUBWEAK FTM0_IRQHandler
      PUBWEAK FTM1_IRQHandler
      PUBWEAK FTM2_IRQHandler
      PUBWEAK RTCA_IRQHandler
      PUBWEAK RTCS_IRQHandler
      PUBWEAK PIT_IRQHandler
      PUBWEAK DAC_IRQHandler
      PUBWEAK TSI_IRQHandler
      PUBWEAK MCG_IRQHandler
      PUBWEAK LPTMR_IRQHandler
      PUBWEAK PortA_IRQHandler
      PUBWEAK PortD_IRQHandler

      SECTION .text:CODE:REORDER(1)
      THUMB
NMI_Handler
HardFault_Handler
MemManage_Handler
BusFault_Handler
UsageFault_Handler
SVC_Handler
DebugMon_Handler
PendSV_Handler
SysTick_Handler
DMA0_IRQHandler
DMA1_IRQHandler
DMA2_IRQHandler
DMA3_IRQHandler
FTFA_IRQHandler
LVD_IRQHandler
LLWU_IRQHandler
I2C0_IRQHandler
I2C1_IRQHandler
SPI0_IRQHandler
SPI1_IRQHandler
UART0SE_IRQHandler
UART1SE_IRQHandler
UART2SE_IRQHandler
ADC0_IRQHandler
CMP0_IRQHandler
FTM0_IRQHandler
FTM1_IRQHandler
FTM2_IRQHandler
RTCA_IRQHandler
RTCS_IRQHandler
PIT_IRQHandler
DAC_IRQHandler
TSI_IRQHandler
MCG_IRQHandler
LPTMR_IRQHandler
PortA_IRQHandler
PortD_IRQHandler
Default_Handler
        B Default_Handler
        END
