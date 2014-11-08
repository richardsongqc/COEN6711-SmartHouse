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
 * @file init_hw_52259.c
 *
 * @author
 *
 * @version
 *
 * @date
 *
 * @brief   This file contains the HW initialization for the MC52259 controller
 *****************************************************************************/

#include "MCF52259.h"
#include "init_hw.h"

__declspec(register_abi) static asm void set_vbr (unsigned int vbr)
{

   movec   d0,VBR
   //.long   0x4e7b0801      /* movec d0,VBR */
   rts
   
} 

/*****************************************************************************
 * @name     __initialize_hardware
 *
 * @brief:   Called from startcf_cfv2.
 *
 * @param  : None
 *
 * @return : None
 *****************************************************************************
 *
 *
 ****************************************************************************/
void 
__initialize_hardware(void)
{
	/*******************************************************
	*	Out of reset, the low-level assembly code calls this 
	*	routine to initialize the MCF52259 modules.
	********************************************************/

	asm 
	{
	    /* Initialize IPSBAR */
	    move.l  #__IPSBAR,d0
	    andi.l  #0xC0000000,d0 // need to mask
	    add.l   #0x1,d0
	    move.l  d0,0x40000000

	    /* Initialize FLASHBAR */
	    move.l  #__FLASHBAR,d0
	    andi.l  #0xFFF80000,d0 // need to mask
	    add.l   #0x61,d0
	    movec   d0,FLASHBAR
	}
	
	set_vbr(0);
}
