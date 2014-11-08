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
* $FileName:    sci2.c
* $Version :
* $Date    :
*
* Comments: This file conatains APIs for SCI module
*
*
*END************************************************************************/
#include "sci.h"
#include "types.h"
#include "derivative.h"

/**************************************************************************
* Function Name    : sci_init

* Returned Value   : None	

* Comments         : Initialize Serial Communication Interface
*    
**************************************************************************/
void 
sci_init(void) 
{
#if defined(__MCF52259_H__)/*mcf52259*/
   	//MCF_GPIO_PUAPAR &= ~(MCF_GPIO_PUAPAR_PUAPAR0(3) | MCF_GPIO_PUAPAR_PUAPAR1(3));
   	//MCF_GPIO_PUAPAR |= (MCF_GPIO_PUAPAR_UTXD0_UTXD0 | MCF_GPIO_PUAPAR_URXD0_URXD0);
   	MCF_GPIO_PUBPAR |= (MCF_GPIO_PUBPAR_UTXD1_UTXD1 | MCF_GPIO_PUBPAR_URXD1_URXD1);
   	//MCF_GPIO_PUCPAR |= (MCF_GPIO_PUCPAR_UTXD2_UTXD2 | MCF_GPIO_PUCPAR_URXD2_URXD2);
#else

#error "Need a basic port for the UART using this new V2 MCU"

#endif

    /*UART initialization is taken care when calling printf*/
    
    /*TODO: if using non-CW compiler, need to init uart here*/
   	
}

