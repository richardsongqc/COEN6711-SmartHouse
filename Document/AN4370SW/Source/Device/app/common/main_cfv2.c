/******************************************************************************
 *
 * Freescale Semiconductor Inc.
 * (c) Copyright 2004-2010 Freescale Semiconductor, Inc.
 * ALL RIGHTS RESERVED.
 *
 ******************************************************************************
 *
 * THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
 * IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 **************************************************************************//*!
 *
 * @file main_cfv2.c
 *
 * @author
 *
 * @version
 *
 * @date
 *
 * @brief   This software is the USB driver stack for Coldfire V2 family
 *****************************************************************************/

#include "types.h"      	/* User Defined Data Types */
#include "derivative.h" 	/* include peripheral declarations */
#include "user_config.h"
#include "RealTimerCounter_cfv2.h"
#include "wdt_cfv2.h"
#include "exceptions_cfv2.h"
#include "sci.h"
#include "Boot_loader_task.h"

/*****************************************************************************
 * Global Functions Prototypes
 *****************************************************************************/
extern void TestApp_Init(void);
extern void TestApp_Task(void);
/*****************************************************************************
 * Local Functions Prototypes
 *****************************************************************************/
static void Init_Sys(void);
extern void Watchdog_Reset(void);

static void pll_init(void);
static void Mcu_init(void);

#warning "Execute mass erase before downloading the DFU to MCU. Programming MCU doesnt mean a mass erase is executed!"

/******************************************************************************
 * @name        main
 *
 * @brief       This routine is the starting point of the application
 *
 * @param       None
 *
 * @return      None
 *
 *****************************************************************************
 * This function initializes the system, enables the interrupts and calls the
 * application
 *****************************************************************************/
void 
main(void)
{
	GPIO_Bootloader_Init();
	Switch_mode();     /* switch between the application and the bootloader mode */

	Init_Sys();        /* initial the system */
    
#if MAX_TIMER_OBJECTS
    (void)TimerQInitialize(0);
#endif
    (void)TestApp_Init(); /* Initialize the USB Test Application */
    
    while(TRUE)
    {
       Watchdog_Reset();        
       
       /* Call the application task */
       TestApp_Task();
    }
}

/*****************************************************************************
 * Local Functions
 ****************************************************************************/
 
/*****************************************************************************
 *
 *    @name     Init_Sys
 *
 *    @brief    This function Initializes the system
 *
 *    @param    None
 *
 *    @return   None
 *
 ****************************************************************************
 * Intializes the MCU, MCG, KBI, RTC modules
 ***************************************************************************/
static void 
Init_Sys(void)
{
    /* initialize the MCU registers */
    pll_init();
    Mcu_init();
 
}

/********************************************************************/
void 
pll_init(void)
{
#if defined(__MCF52259_H__)
	MCF_CLOCK_OCLR = 0xF0;
    MCF_CLOCK_CCHR = 0x05;
    MCF_CLOCK_SYNCR = 0
                    | MCF_CLOCK_SYNCR_RFD(0)
                    | MCF_CLOCK_SYNCR_MFD(3)
                    | MCF_CLOCK_SYNCR_PLLMODE
                    | MCF_CLOCK_SYNCR_PLLEN
                    ;
      
    /* wait for PLL locks before switching clock source */
    while (!(MCF_CLOCK_SYNSR & MCF_CLOCK_SYNSR_LOCK))
    {;}
    
    MCF_CLOCK_CCLR = 0;
    MCF_CLOCK_SYNCR |= MCF_CLOCK_SYNCR_CLKSRC;
    
    /* wait for PLL lock again */
    while (!(MCF_CLOCK_SYNSR & MCF_CLOCK_SYNSR_LOCK))
    {;}
#else
#error "Need port for specific MCU"
#endif
}

/********************************************************************/
void 
Mcu_init(void)
{
#if defined(__MCF52259_H__)
	/************************************************
	 * Enable on-chip modules to access internal SRAM
	 ************************************************/
	MCF_SCM_RAMBAR = (0
		| MCF_SCM_RAMBAR_BA(RAMBAR_ADDRESS)
		| MCF_SCM_RAMBAR_BDE);
		
   	/*
	 * Disable Software Watchdog Timer
	 */
	MCF_SCM_CWCR = 0;
		
    /************************************************
	 * USB low level init
	 ************************************************/
		
	// setup gpio state, func and dirrections
    MCF_GPIO_PUAPAR &= 0x3f;
    MCF_GPIO_PORTUA |= MCF_GPIO_PORTUA_PORTUA3;   // turn Vbus off
	MCF_GPIO_DDRUA |= MCF_GPIO_DDRUA_DDRUA3;      // CTS0 as output
	
	MCF_GPIO_PUAPAR &= 0xcf;
	MCF_GPIO_DDRUA &= ~MCF_GPIO_PORTUA_PORTUA3;     // RTS0 as input
	
    //reg_ptr->GPIO.PORTUA |= MCF5225_GPIO_PORTxP3; // turn Vbus off
	MCF_GPIO_PORTUA &= ~MCF_GPIO_PORTUA_PORTUA3;  // turn Vbus on
	
    MCF_GPIO_PQSPAR |= MCF_GPIO_PQSPAR_PQSPAR5(3) | MCF_GPIO_PQSPAR_PQSPAR6(3);
    
    
    /* unmask USB interrupts in INTC0 */
	MCF_INTC0_IMRL &= ~(MCF_INTC_IMRL_MASKALL);
	MCF_INTC0_IMRH &= ~(MCF_INTC_IMRH_INT_MASK53);
	
	/* Setup the interrupt level and priority for USB */
	MCF_INTC0_ICR53 = MCF_INTC_ICR_IL(6) | MCF_INTC_ICR_IP(6);
#else
#error "Need port for specific MCU"
#endif
}

/* EOF */
