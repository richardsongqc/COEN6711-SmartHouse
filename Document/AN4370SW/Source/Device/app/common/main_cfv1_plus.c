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
 * @file main_cfv1_plus.c
 *
 * @author
 *
 * @version
 *
 * @date
 *
 * @brief   This software is the USB driver stack for S08 family
 *****************************************************************************/
#include <hidef.h>
#include "types.h"
#include "derivative.h" /* include peripheral declarations */
#include "user_config.h"
#include "RealTimerCounter.h"

#include <stdio.h>

/* Bootloader application */
#include "Boot_loader_task.h"

#warning "Execute mass erase before downloading the DFU to MCU. Programming MCU doesnt mean a mass erase is executed!"

/*****************************************************************************
 * Macros
 *****************************************************************************/

#define BSP_REF_CLOCK_DIV               (BSP_CLOCK_SRC / 2000000)-1

/*****************************************************************************
 * Global Functions Prototypes
 *****************************************************************************/
#if MAX_TIMER_OBJECTS
extern uint_8 TimerQInitialize(uint_8 ControllerId);
#endif
extern void TestApp_Init(void);
extern void TestApp_Task(void);

/*****************************************************************************
 * Local Functions Prototypes
 *****************************************************************************/
void Watchdog_Reset(void);
static void Init_Sys(void);
static void Mcu_Init(void);
static void MCG_Init(void);

/*FSL: allows to switch between bootloader or loaded App*/

const unsigned char FOPT@0x40E = 0xFD;/*not allow EzPort init*/

/****************************************************************************
 * Global Variables
 ****************************************************************************/

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
    /* Bootloader application */
	GPIO_Bootloader_Init();
	Switch_mode();              /* switch between the application and the bootloader mode */

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
 *****************************************************************************/

void 
Watchdog_Reset(void)
{
	SIM_SRVCOP = 0x55;
	SIM_SRVCOP = 0xAA;
}

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
    Mcu_Init(); /* initialize the MCU registers */
    MCG_Init(); /* initialize the MCG to generate 24MHz bus clock */     
}

/*****************************************************************************
 * @name     MCU_Init
 *
 * @brief:   Initialization of the MCU.
 *
 * @param  : None
 *
 * @return : None
 *****************************************************************************
 * It will configure the MCU to disable STOP and COP Modules.
 * It also set the MCG configuration and bus clock frequency.
 ****************************************************************************/

static void 
Mcu_Init()
{
	//SIM_SCGC1 = 0;
	//SIM_SCGC2 = SIM_SCGC2_VREF_MASK;
	//SIM_SCGC3 = 0;
	//SIM_SCGC4 = SIM_SCGC4_FTFL_MASK;
	//SIM_SCGC5 = 0;
	//SIM_SCGC6 = 0;
	
	/* Enable clock source for MTIM */
	SIM_SCGC3 |= (SIM_SCGC3_MTIM_MASK);
	/* Enable clock source for USB module */
	SIM_SCGC6 |= SIM_SCGC6_USBOTG_MASK;
	
    /*FSL: unknown reason LTPMR0/1 is fired after a correct download corrupting other downloads!*/
#if (defined _MCF51_PLUS_H)
    LPTMR0_CSR = 0;
    LPTMR0_PSR = 0;
    LPTMR0_CMR = 0;
    LPTMR0_CNR = 0;
    LPTMR1_CSR = 0;
    LPTMR1_PSR = 0;
    LPTMR1_CMR = 0;
    LPTMR1_CNR = 0;
#endif
}

/*****************************************************************************
 * @name     MCG_Init
 *
 * @brief:   Initialization of the Multiple Clock Generator.
 *
 * @param  : None
 *
 * @return : None
 *****************************************************************************
 * Provides clocking options for the device, including a phase-locked
 * loop(PLL) and frequency-locked loop (FLL) for multiplying slower reference
 * clock sources
 ****************************************************************************/
static void 
MCG_Init()
{
	SIM_SCGC5 |= SIM_SCGC5_MCG_MASK;

#ifndef EXTERNAL_OSC
	SIM_SCGC5 |= SIM_SCGC5_OSC2_MASK;
#endif
    
    /* First move to FBE mode */
    /* Enable external oscillator, RANGE=1, HGO=1, EREFS=1, LP=0, IRCS=0 */
#ifdef EXTERNAL_OSC
    MCG_C2 = 0;
#else
    MCG_C2 = MCG_C2_RANGE(1) | MCG_C2_HGO_MASK | MCG_C2_EREFS_MASK;
#endif
    /* Select external oscillator and Reference Divider and clear IREFS to start ext. osc. */
    // CLKS=2, FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0
    MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(3); 
    /* Wait for oscillator to initialize */
#ifndef EXTERNAL_OSC
    while (!(MCG_S & MCG_S_OSCINIT_MASK))
    {;}
#endif
    /* Wait for Reference clock Status bit to clear */
    while (MCG_S & MCG_S_IREFST_MASK)
    {;} 
    /* Wait for clock status bits to show clock source is ext. ref. clk. */
    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2)
    {;}    
 
    /* Configure PLL Ref Divider, PLLCLKEN=0, PLLSTEN=0, PRDIV=5
       The crystal frequency is used to select the PRDIV value. Only even frequency crystals are supported
       that will produce a 2MHz reference clock to the PLL. */
    MCG_C5 = MCG_C5_PRDIV(BSP_REF_CLOCK_DIV); // Set PLL ref. divider to match the crystal used
    
    /* Ensure MCG_C6 is at the reset default of 0. LOLIE disabled, PLL disabled, clk. monitor disabled, PLL VCO divider is clear */
    MCG_C6 = 0x0;   
 
    /* Set system options dividers
       MCG=PLL, core = MCG  */ 
	SIM_CLKDIV0 =  SIM_CLKDIV0_OUTDIV(0);
    /* Set the VCO divider and enable the PLL for 48MHz, LOLIE=0, PLLS=1, CME=0, VDIV=0 */
    MCG_C6 = MCG_C6_PLLS_MASK; //VDIV = 0 (x24)    
    
    /* Wait for PLL status bit to set */
    while (!(MCG_S & MCG_S_PLLST_MASK))
    {;} 
    /* Wait for LOCK bit to set */
    while (!(MCG_S & MCG_S_LOCK_MASK))
    {;}    
 
    /* Now running PBE Mode */
    /* Transition into PEE by setting CLKS to 0
       CLKS=0, FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0 */
    MCG_C1 &= ~MCG_C1_CLKS_MASK;

    /* Wait for clock status bits to update */
    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3)
    {;}

    /* Now running PEE Mode */            
    SIM_SOPT3 |= SIM_SOPT3_RWE_MASK; 
    SIM_SOPT1 |= SIM_SOPT1_REGE_MASK;
    SIM_SOPT7 |= 0x80;
    
    /* Ensure PLL is selected as USB CLK source */
    SIM_CLKDIV1 = 0x00;            
}

