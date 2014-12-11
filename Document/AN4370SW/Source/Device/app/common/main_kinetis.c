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
 * @file main_kinetis.c
 *
 * @author
 *
 * @version
 *
 * @date
 *
 * @brief   This software is the USB driver stack
 *****************************************************************************/
#include "types.h"
#include "derivative.h" /* include peripheral declarations */
#include "user_config.h"
#include "RealTimerCounter.h"
#include "Wdt_kinetis.h"
#include "hidef.h"

#include "Boot_loader_task.h"

extern uint_32 ___VECTOR_RAM[];         //Get vector table that was copied to RAM

/*****************************************************************************
 * Global Functions Prototypes
 *****************************************************************************/
#if MAX_TIMER_OBJECTS
extern uint_8 TimerQInitialize(uint_8 ControllerId);
#endif
extern void TestApp_Init(void);
extern void TestApp_Task(void);

#define BSP_REF_CLOCK_SRC               (2000000ul)       // must be 2-4MHz

/* BSP_CLOCK_MUL from interval 24 - 55 */
#if (!defined(MCU_MKL25Z4))
#define BSP_CLOCK_MUL                   (24)    /* 48MHz for all Kinetis implementations with normal PLL (except K70) */
#define BSP_CORE_DIV                    (1)
#define BSP_FLASH_DIV                   (2)
#else
#define BSP_CLOCK_MUL                   (48)    /* 96MHz, KL2 doesn't have prescaler */
#define BSP_CORE_DIV                    (2)
#define BSP_FLASH_DIV                   (4)
#endif

#define BSP_BUS_DIV                     (1)
#define BSP_FLEXBUS_DIV                 (1)

#define BSP_REF_CLOCK_DIV               (BSP_CLOCK_SRC / BSP_REF_CLOCK_SRC)

#define BSP_CLOCK                       (BSP_REF_CLOCK_SRC * BSP_CLOCK_MUL)
#define BSP_CORE_CLOCK                  (BSP_CLOCK / BSP_CORE_DIV)      /* CORE CLK, max 100MHz */
#define BSP_SYSTEM_CLOCK                (BSP_CORE_CLOCK)                /* SYSTEM CLK, max 100MHz */
#define BSP_BUS_CLOCK                   (BSP_CLOCK / BSP_BUS_DIV)       /* max 50MHz */
#define BSP_FLEXBUS_CLOCK               (BSP_CLOCK / BSP_FLEXBUS_DIV)
#define BSP_FLASH_CLOCK                 (BSP_CLOCK / BSP_FLASH_DIV)     /* max 25MHz */

/*****************************************************************************
 * Local Functions Prototypes
 *****************************************************************************/
static void Init_Sys(void);
static unsigned char pll_init();
static unsigned char pll2_init();
static void wdog_disable(void);

#warning "Execute mass erase before downloading the DFU to MCU. Programming MCU doesnt mean a mass erase is executed!"

/****************************************************************************
 * Global Variables
 ****************************************************************************/

/*****************************************************************************
 * Global Functions
 *****************************************************************************/
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
    /* Point the VTOR to the new copy of the vector table */
    SCB_VTOR = (uint_32)___VECTOR_RAM;

#if  defined(MCU_MKL25Z4)/*L2K*/
    NVIC_ICPR |= 1 << ((INT_USB0 - 16)%32);
    NVIC_ISER |= 1 << ((INT_USB0 - 16)%32);
#elif  defined(MCU_MK20D5)/*K20*/
    NVICICER1|=(1<<3);                      /* Clear any pending interrupts on USB */
    NVICISER1|=(1<<3);                      /* Enable interrupts from USB module */
#elif (defined(MCU_MK60N512VMD100)|defined(MCU_MK40N512VMD100)|defined(MCU_MK70F15))/*KK40,K60,K70*/
    NVICICER2|=(1<<9);                      /* Clear any pending interrupts on USB */
    NVICISER2|=(1<<9);                      /* Enable interrupts from USB module */
    MPU_CESR=0x00;                          /* Allow USB master to access SRAM{H,L} */
#else
#error "Define macro for new Kinetis port"
#endif

    /************* USB Part **********************/
#if defined(MCU_MK70F15)
    /* Clock Configuration for K70*/
    pll2_init();
    /* USB Fractional Divider value for 120MHz input */
    /** USB Clock = PLL0 x (FRAC +1) / (DIV+1)       */
    /** USB Clock = 120MHz x (1+1) / (4+1) = 48 MHz    */
    SIM_CLKDIV2 = SIM_CLKDIV2_USBFSFRAC_MASK | SIM_CLKDIV2_USBFSDIV(4);         /** Divide reference clock to obtain 48MHz */
    SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL(1)       /** PLL0 reference */
              |  SIM_SOPT2_USBFSRC(0)         /** MCGPLLCLK as CLKC source */
              |  SIM_SOPT2_USBF_CLKSEL_MASK;  /** USB fractional divider like USB reference clock */
    SIM_SCGC4|=SIM_SCGC4_USBFS_MASK;             // USB Clock Gating
#else/*FSL: the rest of Kinetis devices*/
    /* Clock Configuration */
    pll_init();
    /*FSL: assuming 48MHz core clk*/
    /*********************************************/
#if (!defined(MCU_MKL25Z4))
    /* Configure USBFRAC = 0, USBDIV = 1 => frq(USBout) = 1 / 2 * frq(PLLin) */
    SIM_CLKDIV2 &= SIM_CLKDIV2_USBFRAC_MASK | SIM_CLKDIV2_USBDIV_MASK;
    SIM_CLKDIV2|= SIM_CLKDIV2_USBDIV(0);
#endif
    /* Configure USB to be clocked from PLL */
    SIM_SOPT2  |= SIM_SOPT2_USBSRC_MASK | SIM_SOPT2_PLLFLLSEL_MASK;
    /* Enable USB-OTG IP clocking */
    SIM_SCGC4|=(SIM_SCGC4_USBOTG_MASK);
#endif

    /* Configure enable USB regulator for device */
    SIM_SOPT1 |= SIM_SOPT1_USBREGEN_MASK;
}

/*****************************************************************************
 * @name     pll_init
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
static unsigned
char pll_init()
{
    /* Set system options dividers */
    SIM_CLKDIV1 =   0
                    | SIM_CLKDIV1_OUTDIV1(BSP_CORE_DIV - 1)     /* core/system clock */
                    #if (!defined(MCU_MKL25Z4))
                    | SIM_CLKDIV1_OUTDIV2(BSP_BUS_DIV - 1)      /* peripheral clock; */
                    #endif
                    #if (!(defined(MCU_MK20D5)|defined(MCU_MKL25Z4)))
                    | SIM_CLKDIV1_OUTDIV3(BSP_FLEXBUS_DIV - 1)  /* FlexBus clock driven to the external pin (FB_CLK)*/
                    #endif
                    | SIM_CLKDIV1_OUTDIV4(BSP_FLASH_DIV - 1)
                    ;     /* flash clock */

    /*This assumes that the MCG is in default FEI mode out of reset. */

    /* First move to FBE mode */
#ifdef EXTERNAL_OSC
    /* Enable external oscillator, RANGE=0, HGO=, EREFS=, LP=, IRCS= */
    MCG_C2 = 0;
#else
    /* Enable external oscillator, RANGE=2, HGO=1, EREFS=1, LP=0, IRCS=0 */
    MCG_C2 = MCG_C2_RANGE(2) | MCG_C2_HGO_MASK | MCG_C2_EREFS_MASK | MCG_C2_IRCS_MASK;
#endif

    /* Select external oscilator and Reference Divider and clear IREFS to start ext osc
	   CLKS=2, FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0 */
    MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(3);

#ifndef EXTERNAL_OSC
    /* wait for oscillator to initialize */
    while (!(MCG_S & MCG_S_OSCINIT_MASK))
    {;}
#endif

    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2)
    {;} /* Wait for clock status bits to show clock source is ext ref clk */

#ifdef EXTERNAL_OSC
    MCG_C5 = MCG_C5_PRDIV(BSP_REF_CLOCK_DIV - 1);
#else
    MCG_C5 = MCG_C5_PRDIV(BSP_REF_CLOCK_DIV - 1) | MCG_C5_PLLCLKEN_MASK;
#endif

    /* Ensure MCG_C6 is at the reset default of 0. LOLIE disabled,
     PLL enabled, clk monitor disabled, PLL VCO divider is clear */
    MCG_C6 = 0;

    /* Set the VCO divider and enable the PLL, LOLIE = 0, PLLS = 1, CME = 0, VDIV = */
    MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(BSP_CLOCK_MUL - 24); /* 2MHz * BSP_CLOCK_MUL */

    while (!(MCG_S & MCG_S_PLLST_MASK))
    {;} /* wait for PLL status bit to set */
    while (!(MCG_S & MCG_S_LOCK_MASK))
    {;} /* Wait for LOCK bit to set */

    /* Transition into PEE by setting CLKS to 0
    CLKS=0, FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0 */
    MCG_C1 &= ~MCG_C1_CLKS_MASK;

    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3)
    {;} /* Wait for clock status bits to update */

    return 0;
}

/*****************************************************************************
 * @name     pll_init
 *
 * @brief:   Initialization of the K70 special PLL.
 *
 * @param  : None
 *
 * @return : None
 *****************************************************************************
 * It will configure the MCU to disable STOP and COP Modules.
 * It also set the MCG configuration and bus clock frequency.
 ****************************************************************************/
static unsigned
char pll2_init()
{
    /*TODO: requires init for different freqs for osc and crystal: fixed to 120MHz with 50MHz osc input*/

    /* Set the system dividers */
    /* NOTE: The PLL init will not configure the system clock dividers,
     * so they must be configured appropriately before calling the PLL
     * init function to ensure that clocks remain in valid ranges.
     */
    SIM_CLKDIV1 = ( 0
                    | SIM_CLKDIV1_OUTDIV1(0)
                    | SIM_CLKDIV1_OUTDIV2(1)
                    | SIM_CLKDIV1_OUTDIV3(1)
                    | SIM_CLKDIV1_OUTDIV4(5) );

    // On rev. 1.0 of silicon there is an issue where the the input bufferd are enabled when JTAG is connected.
    // This has the affect of sometimes preventing the oscillator from running. To keep the oscillator amplitude
    // low, RANGE = 2 should not be used. This should be removed when fixed silicon is available.
    MCG_C2 = MCG_C2_RANGE(1);

    MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(5);

    while (!(MCG_S & MCG_S_IREFST_MASK))
    {;} /* Wait for IREFST bit to set */

    while (!(MCG_S & MCG_S_CLKST_MASK))
    {;} /* Wait for CLKST bit to set */

    MCG_C5 = MCG_C5_PRDIV(5 - 1);

    MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(0x18 - 16);

    while (!(MCG_S & MCG_S_PLLST_MASK))
    {;} /* wait for PLL status bit to set */
    while (!(MCG_S & MCG_S_LOCK_MASK))
    {;} /* Wait for LOCK bit to set */

    /* Transition into PEE by setting CLKS to 0
    CLKS=0, FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0 */
    MCG_C1 &= ~MCG_C1_CLKS_MASK;

    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3)
    {;} /* Wait for clock status bits to update */

    return 0;
}

/* EOF */