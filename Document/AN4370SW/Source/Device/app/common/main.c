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
 * @file main.c
 *
 * @author
 *
 * @version
 *
 * @date
 *
 * @brief   This software is the USB driver stack for S08 family
 *****************************************************************************/
#include "types.h"
#include "derivative.h" /* include peripheral declarations */
#include "user_config.h"
#include "RealTimerCounter.h"
#include "Boot_loader_task.h"
#include "Bootloader.h"
#include "sci.h"

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

#warning "Execute mass erase before downloading the DFU to MCU. Programming MCU doesnt mean a mass erase is executed!"

/****************************************************************************
 * Global Variables
 ****************************************************************************/

/*****************************************************************************
 * Global Functions
 *****************************************************************************/

#if defined(_MCF51_H)
/* unsecure flash */
const uint_8 nvprot@0x040d = 0xFF;/*FSL: all sectors available*/
const uint_8 nvopt@0x040f = 0;    /*FSL: no secure*/
/* checksum bypass */
const uint_8 checksum_bypass@0x040a = 0x0; 
#endif

#if defined(_MC9S08_H)
const byte NVOPT_INIT  @0x0000FFBF = 0x02;    // vector redirect, flash unsecure
const byte NVPROT_INIT @0x0000FFBD = 0x01;    // no flash is protected 
#endif

void 
Watchdog_Reset(void)
{
    /* Body */
    SRS = 0x55;
    SRS = 0xAA;
} /* EndBody */

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
#if defined(_MC9S08_H)
extern uint_8 boot_mode;
#endif
void main(void)
{
    /* Body */

    uint_32 i;
#ifdef _MC9S08_H
    boot_mode = BOOT_MODE;
#endif
    GPIO_Bootloader_Init();
    /* switch between the application and the bootloader mode */
    Switch_mode();              
     /* delay some time for oscillator and GPIO */
    for(i=0;i<10000;i++)
    {;}

    /* Application */
    Init_Sys();                 /* initial the system */
#if MAX_TIMER_OBJECTS
    (void)TimerQInitialize(0);
#endif
    (void)TestApp_Init(); /* Initialize the USB Test Application */
    
    for(;;)
    {
        Watchdog_Reset();

       /* Call the application task */
       TestApp_Task();
    } /* EndWhile */
} /* EndBody */

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
    Mcu_Init(); /* initialize the MCU registers */
    MCG_Init(); /* initialize the MCG to generate 24MHz bus clock */
}

/*****************************************************************************
 * @name     MCU_Init
 *
 * @brief:   Initialization of the MCU.
 *
 * @param    None
 *
 * @return   None
 *****************************************************************************
 * It will configure the MCU to disable STOP and COP Modules.
 * It also set the MCG configuration and bus clock frequency.
 ****************************************************************************/
static void 
Mcu_Init()
{
#if (/*defined(_MC9S08JM16_H) ||*/ defined(_MC9S08JM60_H))    
    /* System Options 1 Register */
    SOPT1 = 0x20; /*Enable the COP,  and enable the MCU stop mode*/
                      /*
                 *  0b00100000
                 *    ||||||||__ bit0: Reserved, Read Only bit
                 *    |||||||___ bit1: Reserved, Read Only bit
                 *    ||||||____ bit2: Reserved, Read Only bit
                 *    |||||_____ bit3: Reserved, Read Only bit
                 *    ||||______ bit4: WAIT Mode Enable
                 *    |||_______ bit5: STOP Mode enable
                 *    ||________ bit6: COP Watchdog Timeout
                 *    |_________ bit7: COP Watchdog Timeout
                 */
  /* System Options 2 Register */
    SOPT2 = 0x00; /*
                 *  0b00000000
                 *    ||||||||__ bit0: ACMP output not connected to
                 *    |||||||                  TPM1 input channel 0
                 *    |||||||___ bit1: Input filter disabled on SPI2
                 *    ||||||           (allows for higher maximum baud rate)
                 *    ||||||____ bit2: Input filter disabled on SPI21
                 *    |||||            (allows for higher maximum baud rate)
                 *    |||||_____ bit3: Clock source to CMT is BUSCLK
                 *    ||||______ bit4: BUSCLK not avail on the extern pin PTF4
                 *    |||_______ bit5: 0 Little Endian
                 *    ||________ bit6: COP Window Mode - Normal mode
                 *    |_________ bit7: COP Watchdog Clock Select -
                 *                     Internal 1 kHz LPOCLK is source to COP
                 */
    /* System Power Management Status and Control 1 Register */
    SPMSC1 = 0x40;/*
                 *  0b01000000
                 *    ||||||||__ bit0: Bandgap buffer disabled
                 *    |||||||___ bit1: Reserved, Read Only bit
                 *    ||||||____ bit2: Low-Voltage Detect(LVD) logic disabled
                 *    |||||_____ bit3: LVD disabled during stop mode
                 *    ||||______ bit4: LVDF does not generate hardware resets
                 *    |||_______ bit5: Hardware interrupt disabled(use polling)
                 *    ||________ bit6: If LVWF = 1, a low-voltage
                 *    |                condition has occurred
                 *    |_________ bit7: Low-voltage warning is not present
                 */
  /* System Power Management Status and Control 2 Register */
  SPMSC2 = 0x00;/*
                 *  0b00000000
                 *    ||||||||__ bit0: Stop3 low power mode enabled
                 *    |||||||___ bit1: Reserved, Read Only bit
                 *    ||||||____ bit2: Writing 1 to PPDACK clears the PPDF bit
                 *    |||||_____ bit3: read-only status : Microcontroller has
                 *    ||||             not recovered from Stop2 mode
                 *    ||||______ bit4: Low trip point selected
                 *    |||_______ bit5: Low trip point selected
                 *    ||________ bit6: Reserved, Read Only bit
                 *    |_________ bit7: Reserved, Read Only bit
                 */
#elif (defined(_MCF51JM128_H))/*FSL:new!*/
  /* Disable watchdog. */
    SOPT1 = 0x0;  /* disable COP; enable bkgd, stop and wait mode */
 
    SOPT2 = 0;    /* set right endianess for USB module */
    SOPT3 = 0;    /* route TX1,RX1 to PTD6,PTD7 */
#elif (defined(_MCF51MM256_H) || defined(_MCF51JE256_H))
  /* Disable watchdog. */
    SOPT1 = 0x23; /* disable COP; enable bkgd, stop and wait mode */
 
    SOPT2 = 0x06; /* set right endianess for USB module */
    SOPT3 = SOPT3_SCI1PS_MASK;/* route TX1,RX1 to PTD6,PTD7 */
#else
#error "Need a port for this S08 or V1 board"
#endif
}

/*****************************************************************************
 * @name     MCG_Init
 *
 * @brief:   Initialization of the Multiple Clock Generator.
 *
 * @param    None
 *
 * @return   None
 *****************************************************************************
 * Provides clocking options for the device, including a phase-locked
 * loop(PLL) and frequency-locked loop (FLL) for multiplying slower reference
 * clock sources
 ****************************************************************************/
static void 
MCG_Init()
{
    /* Body */
#if (defined(_MCF51JM128_H) /*|| defined(_MC9S08JM16_H)*/ || defined(_MC9S08JM60_H) /*|| defined(_MC9S08JS16_H)*/)  
    /* the MCG is default set to FEI mode, it should be change to FBE mode*/
    /* MCG Control Register 2 */
    MCGC2 = 0x36;
                   /*
                *  0b00110110
                *    ||||||||__ bit0:   External ref clock disabled in stop
                *    |||||||___ bit1:   MCGERCLK active
                *    ||||||____ bit2:   Oscillator requested
                *    |||||_____ bit3:   FLL/PLL not disabled in bypass modes
                *    ||||______ bit4:   Configure crystal oscillator for
                *    |||                high gain operation
                *    |||_______ bit5:   High frequency range selected for
                *    ||                 crystal oscillator of 1 MHz to 16 MHz
                *    ||________ bit6:   Divides selected clock by 1
                *    |_________ bit7:   Divides selected clock by 1
                */
  /* MCG Status and Control Register */
    while(!(MCGSC & 0x02))
    {;}       /*Wait for the OSC stable*/
                   /*
                *  0b00000010
                *    ||||||||__ bit0:   FTRIM       MCG fine trim
                *    |||||||___ bit1:   OSCINIT OSC initialization
                *    ||||||____ bit2:   CLKST       Clock Mode status
                *    |||||_____ bit3:   CLKST
                *    ||||______ bit4:   IREFST  Internal reference status
                *    |||_______ bit5:   PLLST       PLL select status
                *    ||________ bit6:   LOCK        Lock Status
                *    |_________ bit7:   LOLS        Loss of lock status
                */
  /* MCG Control Register 1 */
    MCGC1 = 0x9B;
                   /*
                *  0b10011011
                *    ||||||||__ bit0:   Internal ref clock stays enabled in stop
                *    |||||||            if IRCLKEN is set or if MCG is in FEI,
                *    |||||||            FBI or BLPI mode before entering stop.
                *    |||||||___ bit1:   MCGIRCLK active
                *    ||||||____ bit2:   External reference clock selected
                *    |||||_____ bit3:   External Reference Divider
                *    ||||______ bit4:   External Reference Divider
                *    |||_______ bit5:   External Reference Divider
                *    ||________ bit6:   External reference clock is selected
                *    |_________ bit7:   External reference clock is selected
                */

    /* check the external reference clock is selected or not*/
    while((MCGSC & 0x1C ) != 0x08)
    {;}

    /* Switch to PBE mode from FBE*/
    /* MCG Control Register 3 */
    MCGC3 = 0x48;
                     /*
                *  0b01001000
                *    ||||||||__ bit0:   VCO Divider - Multiply by 32
                *    |||||||___ bit1:   VCO Divider - Multiply by 32
                *    ||||||____ bit2:   VCO Divider - Multiply by 32
                *    |||||_____ bit3:   VCO Divider - Multiply by 32
                *    ||||______ bit4:   Divide-by-32 is disabled
                *    |||_______ bit5:   Clock monitor is disabled
                *    ||________ bit6:   PLL is selected
                *    |_________ bit7:   Generate an interrupt request on loss of lock
                */
    while ((MCGSC & 0x48) != 0x48)
    {;}   /*wait for the PLL is locked & */

    /*Switch to PEE mode from PBE mode*/
    MCGC1 &= 0x3F;
    while((MCGSC & 0x6C) != 0x6C)
    {;}
    
#if 0/*FSL: not supported/tested ports*/
#elif (defined(_MC9S08MM128_H) || defined(_MC9S08JE128_H))
    //------FBE MODE------  
    /* Assume 16MHz external clock source connected. */
    // RANGE = 1; HGO = 1; ERCLKEN=1; EREFS = 1; BDIV = 000
    MCGC2 = 0x36;
  
    // DIV32 =1
    MCGC3 = 0x10;
  
    // CLKS = 10; RDIV = 100; IREFS = 0;
    MCGC1 = 0xA0; 

    // Wait for Reference Status bit to update
    while (MCGSC_IREFST)
    {;} 
    // Wait for clock status bits to update 
    while (MCGSC_CLKST != 0b10)
    {;}       

    //------PBE MODE------ 

    // PLLS =1; DIV32 = 1; VDIV = 1001 
    MCGC3 = 0x5c;
  
    // wait for PLL status bit to update
    while (!MCGSC_PLLST)
    {;} 
    // Wait for LOCK bit to set 
    while (!MCGSC_LOCK)
    {;}
  
    //------PEE MODE------   

    // CLKS = 00; RDIV = 100; IREFS = 0
    MCGC1 = 0x20;

    // Wait for clock status bits to update 
    while (MCGSC_CLKST != 0b11)
    {;}
    
    /* Now MCGOUT=48MHz, BUS_CLOCK=24MHz */  
#elif ((defined _MCF51MM256_H) || (defined _MCF51JE256_H))
    /* Assume 12MHz external clock source connected. */
  
    /* In order to use the USB we need to enter PEE mode and MCGOUT set to 48 MHz. 
     Out of reset MCG is in FEI mode. */
  
    //------FBE MODE------  
    /* Assume 16MHz external clock source connected. */
    // RANGE = 1; HGO = 1; ERCLKEN=1; EREFS = 1; BDIV = 000
    MCGC2 = 0x36;
  
    // DIV32 =1
    MCGC3 = 0x10;
  
    // CLKS = 10; RDIV = 100; IREFS = 0;
    MCGC1 = 0xA0; 

    // Wait for Reference Status bit to update
    while (MCGSC_IREFST)
    {;} /* EndWhile */
    // Wait for clock status bits to update 
    while (MCGSC_CLKST != 0b10) 
    {;} /* EndWhile */

    //------PBE MODE------ 

    // PLLS =1; DIV32 = 1; VDIV = 1001 
    MCGC3 = 0x5c;
  
    // wait for PLL status bit to update
    while (!MCGSC_PLLST) 
    {;} /* EndWhile */
    // Wait for LOCK bit to set 
    while (!MCGSC_LOCK) 
    {;} /* EndWhile */

    //------PEE MODE------   

    // CLKS = 00; RDIV = 100; IREFS = 0
    MCGC1 = 0x20;

    // Wait for clock status bits to update 
    while (MCGSC_CLKST != 0b11) 
    {;} /* EndWhile */
#endif/*FSL: end of no supported ports*/
    
#else
#error "Need a port for this S08 or V1 board port"
#endif
} /* EndBody */

/* EOF */
