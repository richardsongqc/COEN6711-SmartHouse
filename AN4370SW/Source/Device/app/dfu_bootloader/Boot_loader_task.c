/**HEADER********************************************************************
*
* Copyright (c) 2010 Freescale Semiconductor;
* All Rights Reserved
*
***************************************************************************
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
**************************************************************************
*
* $FileName: main.c$
* $Version :
* $Date    :
*
* Comments:
*
*   This file contains device driver for mass storage class. This code tests
*   the FAT functionalities.
*
*END************************************************************************/

/**************************************************************************
Include the OS and BSP dependent files that define IO functions and
basic types. You may like to change these files for your board and RTOS
**************************************************************************/
/**************************************************************************
Include the USB stack header files.
**************************************************************************/
#include "derivative.h"
#include "hidef.h"
#include "user_config.h"
#include <string.h>
#include <stdlib.h>    /* ANSI memory controls */
#include "stdio.h"
#include "sci.h"
#include "Bootloader.h"
#include "Boot_loader_task.h"
#if defined(_MCF51_H)
#include "exceptions.h"
#endif

/*FSL: flash algorithms*/

#if (defined __MCF52xxx_H__)
#include "flash_cfv2.h"
#elif defined(_MCF51_H)
#include "flash.h"
#elif ((defined __MK_xxx_H__)||(defined _MCF51_PLUS_H))
#include "flash_FTFL.h"
#elif (defined _MC9S08_H)
#include "flash_hcs.h"
#endif
/*****************************************************************************
 * Constant and Macro's - None
 *****************************************************************************/
#define MIN(a,b)  ((a) < (b) ? (a) : (b))
/**************************************************************************
   Global variables
**************************************************************************/
#if (!(defined   _MC9S08_H))
static uint_32                      New_sp,New_pc; /* stack pointer and program counter */
#endif
/* running mode */
#if defined(_MC9S08_H)
#ifdef _MC9S08JM60_H
uint_8 boot_mode@0x0100;
#else
#error "Need to define support for this S08 core"
#endif
#endif
/**************************************************************************
   Funciton prototypes
**************************************************************************/
#if MAX_TIMER_OBJECTS
extern uint_8 TimerQInitialize(uint_8 ControllerId);
#endif

extern int _startup    (void);

#if defined(__MK_xxx_H__)
extern void
boot_app(uint_32 SP, uint_32 PC);
#endif

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : GPIO_Init
* Returned Value : none
* Comments       : Init LEDs and Buttons
*
*END*--------------------------------------------------------------------*/
void
GPIO_Bootloader_Init()
{
    /* Body */
#ifdef _MC9S08JM60_H
    //PTGDD_PTGDD0    = 0;                       /* PTG0 is input */
    //PTGPE_PTGPE0    = 1;                       /* internal pullup for PTG0  */
    PTBDD_PTBDD5    = 0;                       /* PTB5 is input */
    PTBPE_PTBPE5    = 1;                       /* internal pullup for PTB5 */

    PTEDD_PTEDD2    = 1;
    PTEDD_PTEDD3    = 1;
    PTED_PTED2      = 1;
    PTED_PTED3      = 1;
#elif defined(MCU_MKL25Z4)
    SIM_SCGC5   |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTC_MASK;    /* Enable clock gating to port B & C */
    /* Enable LEDs Port A pin 5 & 16 */
    PORTA_PCR5 |= PORT_PCR_SRE_MASK        /* Slow slew rate */
                |  PORT_PCR_DSE_MASK        /* High drive strength */
            ;
    PORTA_PCR5 |= PORT_PCR_MUX(1);
    PORTA_PCR16 |= PORT_PCR_SRE_MASK        /* Slow slew rate */
                |  PORT_PCR_DSE_MASK        /* High drive strength */
            ;
    PORTA_PCR16 |= PORT_PCR_MUX(1);
    GPIOA_PSOR  |= 1 << 5 | 1 << 16;
    GPIOA_PDDR  |= 1 << 5 | 1 << 16;
    /* set input PORTC pin 3 */
    PORTC_PCR3 =  PORT_PCR_MUX(1);
    GPIOC_PDDR &= ~((uint_32)1 << 3);
    PORTC_PCR3 |= PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;/* pull up*/
#elif defined(MCU_MK20D5)
    SIM_SCGC5   |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK;    /* Enable clock gating to port B & C */
    /* Enable LEDs Port B pin 0 & 1 */
    PORTB_PCR0 |= PORT_PCR_SRE_MASK        /* Slow slew rate */
                |  PORT_PCR_ODE_MASK        /* Open Drain Enable */
                |  PORT_PCR_DSE_MASK        /* High drive strength */
            ;
    PORTB_PCR0 |= PORT_PCR_MUX(1);
    PORTB_PCR1 |= PORT_PCR_SRE_MASK        /* Slow slew rate */
                |  PORT_PCR_ODE_MASK        /* Open Drain Enable */
                |  PORT_PCR_DSE_MASK        /* High drive strength */
            ;
    PORTB_PCR1 |= PORT_PCR_MUX(1);
    GPIOB_PSOR  |= 1 << 1 | 1 << 0;
    GPIOB_PDDR  |= 1 << 1 | 1 << 0;
    /* set input PORTC pin 1 */
    PORTC_PCR1 =  PORT_PCR_MUX(1);
    GPIOC_PDDR &= ~((uint_32)1 << 1);
    PORTC_PCR1 |= PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;/* pull up*/
#elif defined(MCU_MK40N512VMD100)
    SIM_SCGC5   |= SIM_SCGC5_PORTC_MASK;    /* Enable clock gating to PORTC */
    /* Enable LEDs Port C pin 9 & 11 */
    PORTC_PCR9 |= PORT_PCR_SRE_MASK        /* Slow slew rate */
                |  PORT_PCR_ODE_MASK        /* Open Drain Enable */
                |  PORT_PCR_DSE_MASK        /* High drive strength */
            ;
    PORTC_PCR9 |= PORT_PCR_MUX(1);
    PORTC_PCR11 |= PORT_PCR_SRE_MASK        /* Slow slew rate */
                |  PORT_PCR_ODE_MASK        /* Open Drain Enable */
                |  PORT_PCR_DSE_MASK        /* High drive strength */
            ;
    PORTC_PCR11 |= PORT_PCR_MUX(1);
    GPIOC_PSOR  |= 1 << 9 | 1 << 11;
    GPIOC_PDDR  |= 1 << 9 | 1 << 11;
    /* set input PORTC pin 5 */
    PORTC_PCR5 =  PORT_PCR_MUX(1);
    GPIOC_PDDR &= ~((uint_32)1 << 5);
    PORTC_PCR5 |= PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;/* pull up*/
#elif defined(MCU_MK60N512VMD100)
    /* Enable clock gating to PORTA & B */
    SIM_SCGC5   |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK;
    /* Enable LEDs Port A pin 28 & 29 */
    PORTA_PCR28 |= PORT_PCR_SRE_MASK        /* Slow slew rate */
                |  PORT_PCR_ODE_MASK        /* Open Drain Enable */
                |  PORT_PCR_DSE_MASK        /* High drive strength */
            ;
    PORTA_PCR28 |= PORT_PCR_MUX(1);
    PORTA_PCR29 |= PORT_PCR_SRE_MASK        /* Slow slew rate */
                |  PORT_PCR_ODE_MASK        /* Open Drain Enable */
                |  PORT_PCR_DSE_MASK        /* High drive strength */
            ;
    PORTA_PCR29 |= PORT_PCR_MUX(1);
    GPIOA_PSOR  |= 1 << 28 | 1 << 29;
    GPIOA_PDDR  |= 1 << 28 | 1 << 29;
    /* set input PORTA pin 19 */
    PORTA_PCR19 =  PORT_PCR_MUX(1);
    GPIOA_PDDR &= ~((uint_32)1 << 19);
    PORTA_PCR19 |= PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;/* pull up*/
#elif defined(MCU_MK70F15)
    /* Enable clock gating to PORTB & E */
    SIM_SCGC5   |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTE_MASK;
    /* Enable LEDs Port B pin 2 & 3 */
    PORTB_PCR2  |= PORT_PCR_SRE_MASK        /* Slow slew rate */
                |  PORT_PCR_ODE_MASK        /* Open Drain Enable */
                |  PORT_PCR_DSE_MASK        /* High drive strength */
            ;
    PORTB_PCR2  |= PORT_PCR_MUX(1);
    PORTB_PCR3  |= PORT_PCR_SRE_MASK        /* Slow slew rate */
                |  PORT_PCR_ODE_MASK        /* Open Drain Enable */
                |  PORT_PCR_DSE_MASK        /* High drive strength */
            ;
    PORTB_PCR3  |= PORT_PCR_MUX(1);
    GPIOA_PSOR  |= 1 << 3 | 1 << 2;
    GPIOA_PDDR  |= 1 << 3 | 1 << 2;
    /* set input PORTE pin 26 */
    PORTE_PCR26 =  PORT_PCR_MUX(1);
    GPIOE_PDDR &= ~((uint_32)1 << 26);
    PORTE_PCR26 |= PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;/* pull up*/
#elif defined(_MCF51JM128_H)
    /* init buttons on port G */
    PTGDD &= 0xF0;                          /* set PTG0-3 to input */
    PTGPE |= 0x0F;                          /* enable PTG0-3 pull-up resistor */
    /* Enable LEDs Port E pin 2 & 3 */
    PTEDD_PTEDD2    = 1;
    PTEDD_PTEDD3    = 1;
    PTED_PTED2      = 1;
    PTED_PTED3      = 1;
#elif defined(MCU_mcf51jf128)
#ifndef SECOND_WAY_TO_SWITCH_TO_BOOTLOADER
    //enable PORT A,B,C clocks
    SIM_SCGC6 |= SIM_SCGC6_PORTA_MASK | SIM_SCGC6_PORTC_MASK | SIM_SCGC6_PORTB_MASK;

    /* Enable LEDs Port A pin 0 & Port C pin 5 */
    // use PTA0 as GPIO
    MXC_PTAPF4 = (MXC_PTAPF4 & (~MXC_PTAPF3_A0_MASK)) | MXC_PTAPF3_A0(1)  ;
    // set GPIO low
    PTA_D = PTA_D | (0x0<<0);
    // Make PTC4 as o/p
    PTA_DD = PTA_DD | (0x1<<0);

    // use PTC5 as GPIO
    MXC_PTCPF2 = (MXC_PTCPF2 & (~MXC_PTCPF1_C5_MASK)) | MXC_PTCPF1_C5(1)  ;
    // set GPIO low
    PTC_D = PTC_D | (0x0<<5);
    // Make PTC5 as o/p
    PTC_DD = PTC_DD | (0x1<<5);

    /* set in put PORTB pin 0 */
    // use PTB0 as GPIO
    MXC_PTBPF4 = (MXC_PTBPF4 & (~MXC_PTBPF3_B0_MASK)) | MXC_PTBPF3_B0(1)  ;
    // Make PTB0 as i/p
    PTB_DD = PTB_DD & ~(0x1<<0);
    /* pull up for PTB0*/
    PCTLB_PUE |= (0x1<<0);
    PCTLB_PUS |= (0x1<<0);
#else
    //enable PORT A,B,C,D clocks
    SIM_SCGC6 |= SIM_SCGC6_PORTB_MASK;

    /* set in put PORTB pin 5 */
    // use PTB5 as GPIO
    MXC_PTBPF2 = (MXC_PTBPF2 & (~MXC_PTBPF1_B5_MASK)) | MXC_PTBPF1_B5(1)  ;
    // Make PTB5 as i/p
    PTB_DD = PTB_DD & ~(0x1<<5);
    /* pull up for PTB5*/
    PCTLB_PUE |= (0x1<<5);
    PCTLB_PUS |= (0x1<<5);
#endif
#elif defined(__MCF52259_H__)

    /*FSL: adding support for ABORT button*/
    MCF_GPIO_PNQPAR &= ~MCF_GPIO_PNQPAR_PNQPAR7(3);
	MCF_GPIO_DDRNQ &= ~MCF_GPIO_DDRNQ_DDRNQ7;

	/* Configure LED io pins to be outputs.
	 * M52259EVB: LED to port mapping
	 * D10  D11  D12  D13
	 *  |    |    |    |
	 *  ^    ^    ^    ^
	 * PTC0 PTC1 PTC2 PTC3
	 */
	MCF_GPIO_DDRTC = (MCF_GPIO_DDRTC_DDRTC0 |
					  MCF_GPIO_DDRTC_DDRTC1 |
					  MCF_GPIO_DDRTC_DDRTC2 |
					  MCF_GPIO_DDRTC_DDRTC3);
	/* Assign PORTC[0-3] as GPIO */
	MCF_GPIO_PTCPAR = MCF_GPIO_PTCPAR_PTCPAR0(MCF_GPIO_PTCPAR_DTIN0_GPIO) |
	                  MCF_GPIO_PTCPAR_PTCPAR1(MCF_GPIO_PTCPAR_DTIN1_GPIO) |
	                  MCF_GPIO_PTCPAR_PTCPAR2(MCF_GPIO_PTCPAR_DTIN2_GPIO) |
	                  MCF_GPIO_PTCPAR_PTCPAR3(MCF_GPIO_PTCPAR_DTIN3_GPIO);

#else
#error "Turn on MCU logic to switch between bootloader and application, also turn on LEDs"
#endif

} /* EndBody */
/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : Switch_mode
* Returned Value : none
* Comments       : Jump between application and bootloader
*
*END*--------------------------------------------------------------------*/
void
Switch_mode(void)
{
    /* Body */
    volatile uint_32  temp;

#ifdef _MC9S08_H
    uint_8* UserEntryCheck;
#else/*FSL: 32-bit architectures*/
    /* Get PC and SP of application region */
    New_sp  = IMAGE_ADDR[0];
    New_pc  = IMAGE_ADDR[1];
    /* Check switch is pressed*/
#endif

#if defined(__MCF52259_H__)
    temp =(uint_32) ((1<<7) & MCF_GPIO_SETNQ);            /* DES READ SW1 of M52259EVB */
#elif defined(MCU_MKL25Z4)
    temp =(uint_32) ((1<<3) & GPIOC_PDIR);                /* DES READ SW2 of TWK20 */
#elif defined(MCU_MK20D5)
    temp =(uint_32) ((1<<1) & GPIOC_PDIR);                /* DES READ SW2 of TWK20 */
#elif defined(MCU_MK40N512VMD100)
    temp =(uint_32) ((1<<5) & GPIOC_PDIR);                /* DES READ SW1 of TWK40 */
#elif defined(MCU_MK60N512VMD100)
    temp = (uint_32) ((1<<19) & GPIOA_PDIR);               /* DES READ SW1 of TWK60 */
#elif defined(MCU_MK70F15)
    temp = (uint_32) ((1<<26) & GPIOE_PDIR);               /* DES READ SW2 of TWK70 */
#elif defined(_MCF51JM128_H)
    temp =(uint_32) ((1<<1) & PTGD);                      /* DES READ SW1 of JM128EVB */
#elif defined(_MC9S08JM60_H)
    temp =(uint_32) (PTBD_PTBD5/*PTGD_PTGD0*/);                         /* DES READ SW1 of JM60 */
#elif defined(MCU_mcf51jf128)
#ifndef SECOND_WAY_TO_SWITCH_TO_BOOTLOADER
    temp =(uint_32) (1<<0) & (PTB_D);                     /* DES READ SW1 of TWR-JF */
#else
    temp =(uint_32) (1<<5) & (PTB_D);                     /* READ OBSEL pin of TWR-MPC8 */
#endif
#else
#error "Define a valid MCU as the GPIO switch between Bootloader or Application"
#endif
    if(temp)
    {
#ifdef _MC9S08_H
        UserEntryCheck = (byte*)USER_ENTRY_ADDRESS;
        /* check there is a valid jump op-code */
        if((*UserEntryCheck) == 0xCC)
        {
            /* Increment pointer to check jump address */
            UserEntryCheck += 1;
            if((*UserEntryCheck != 0xFF) && (*(UserEntryCheck+1) != 0xFF))         // check there is a valid jump vector
            {
                boot_mode = APP_MODE;
                /* jump to user entry */
                asm (JMP USER_ENTRY_ADDRESS);
            }
        }
#else/*32-bit architectures*/
        if((New_sp != 0xffffffff)&&(New_pc != 0xffffffff))
        {
            /* Run the application */
#if defined (__MCF52xxx_H__)/*ColdFire assembler*/
            asm
            {
                move.w   #0x2700,sr
                move.l   New_sp,a0
                move.l   New_pc,a1
                move.l   a0,a7
                jmp     (a1)
            }
            /*FSL: assembler for ARM Cortex-M4 using CW and IAR*/
#elif defined(__MK_xxx_H__)
#if defined(CORTEX_M0_PLUS)
            boot_app(New_sp,New_pc);
#else
#if defined(__CWCC__)
            asm
            {
                ldr   r4,=New_sp
                ldr   sp, [r4]
                ldr   r4,=New_pc
                ldr   r5, [r4]
                blx   r5
            }
#elif defined(__IAR_SYSTEMS_ICC__)
            asm("mov32   r4,New_sp");
            asm("ldr     sp,[r4]");
            asm("mov32   r4,New_pc");
            asm("ldr     r5, [r4]");
            asm("blx     r5");
#else
#error "Establish platfrom jump for your own port"
#endif
#endif /*CORTEX_M0_PLUS*/
#else
#error "Define valid routine for this platform!"
#endif
        }
#endif
    } /* EndIf */
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : erase_sectors
* Returned Value : none
* Comments       : erase sectors contained ONLY in the flash area needed to be downloaded
*                  by deleting the specific sectors and not the whole application area,
*                  DFU application will run more faster and not adding latency to the
*                  USB app during the time interrupts are disabled. For slow MCUs with
*                  plenty of FLASH space, problem is exacerbated, then this function is
*                  required.
* TODO: improve for non 32 bit architectures: 8 and 16 bits (b06862)
*
*END*--------------------------------------------------------------------*/

uint_8
erase_sectors(uint_32 *FlashStartAddress, uint_32 NumberOfBytes)
{
  /* Body */
  uint_16 remain_bytes,counter;
  uint_16 temp_length;
  uint_8 delete_flag = 0;
  uint_32 start_add = (uint_32)FlashStartAddress;
  uint_32 current_sector = (uint_32)( start_add/ERASE_SECTOR_SIZE );

  /*FSL: new to fix the s08 addresses not aligned to sector size*/
  uint_32 sector_offset = (uint_32)( start_add%ERASE_SECTOR_SIZE );

  uint_8 error = Flash_OK;
  do
  {
    remain_bytes = (uint_16)(((current_sector+1)*ERASE_SECTOR_SIZE)-start_add);
    temp_length = (uint_16)MIN(NumberOfBytes,remain_bytes);/*FSL: special case*/

    /*FSL: check for changes on that area of the flash memory in bytes*/
    for(counter=0;counter<temp_length;counter++)
    {
      /*check byte per byte*/
      if( *(uint_8 *)(start_add+counter) != 0xFF/*empty character*/ )
      {
        delete_flag = 1;
        break;/*no need to check the following bytes*/
      }
    }
    /*FSL: delete if necessary*/
    if(delete_flag)
    {
      if( (error=Flash_SectorErase( (uint_32*)((current_sector*ERASE_SECTOR_SIZE)+sector_offset/*new!*/) )) != Flash_OK )
        break;/*out of do-while loop*/
      delete_flag = 0;/*done*/
    }

    /*FSL: update indexes for next round*/
    NumberOfBytes -= temp_length;
    start_add += temp_length;
    current_sector++;
  }
  while(NumberOfBytes);
  return error;
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : SetOutput
* Returned Value : None
* Comments       : Set out put of the LEDs
*
*END*--------------------------------------------------------------------*/

void
SetOutput
    (
        /* [IN] the output pin */
        uint_32 output,
        /* [IN] the state to set */
        boolean state
    )
{
    /* Body */
#if (defined __MCF52259_H__)/* For 52259EVB */
    if(state)
        MCF_GPIO_PORTTC |= output;
    else
        MCF_GPIO_PORTTC &=~ output;
#elif (defined MCU_MKL25Z4)/* For TWR-LK2 */
    if(state)
        GPIOC_PCOR |= output;
    else
        GPIOC_PSOR |= output;
#elif (defined MCU_MK20D5)/* For TWR-K20 */
    if(state)
        GPIOC_PCOR |= output;
    else
        GPIOC_PSOR |= output;
#elif (defined MCU_MK40N512VMD100)/* For TWR-K40 */
    if(state)
        GPIOC_PCOR |= output;
    else
        GPIOC_PSOR |= output;
#elif (defined MCU_MK60N512VMD100)/* For TWR-K60 */
    if(state)
        GPIOA_PCOR |= output;
    else
        GPIOA_PSOR |= output;
#elif (defined MCU_MK70F15)/* For TWR-K70 */
    if(state)
        GPIOE_PCOR |= output;
    else
        GPIOE_PSOR |= output;
#elif (defined _MCF51JM128_H)/* For 51JM128EVB */
    if(state)
        PTED &=~ output;
    else
        PTED |= output;
#elif (defined _MC9S08JM60_H)/*JM60*/
    if(state)
        PTED &=~ output;
    else
        PTED |= output;
#elif (defined MCU_mcf51jf128)/* For TWR-51JF */
#ifndef SECOND_WAY_TO_SWITCH_TO_BOOTLOADER
    if(state)
    	PTA_D = (uint_8)(PTA_D & ~(0x1<<output));
    else
    	PTA_D = (uint_8)(PTA_D | (0x1<<output));
#else
    /*FSL: empty declaration for this board*/
    UNUSED(output)
    UNUSED(state)
#endif
#else
#error "Define a valid MCU routine to turn on LEDs"
#endif
} /* EndBody */

/* EOF */
