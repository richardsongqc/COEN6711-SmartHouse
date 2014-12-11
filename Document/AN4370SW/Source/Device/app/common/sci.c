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
* Comments:  This file includes operation of serial communication interface.
*
*END************************************************************************/
#include "types.h"
#include "sci.h"
#include "derivative.h"
#include "user_config.h"

#if defined(_MCF51_H)
#include <Uart.h>
#endif

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : sci_init
* Returned Value   :
* Comments         : This function initilizes the SCI 1 baud rate.
*    
*
*END*----------------------------------------------------------------------*/
void 
sci_init(void) 
{
    /* Body */
    
    /* Configure SCI baud rate */
    SCI1BDH = 0; 
    SCI1BDL = 0x0D;
    SCI1C1  = 0;
    SCI1C2  = 0x0C;
    SCI1S2  = 0x04;
} /* EndBody */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : TERMIO_PutChar
* Returned Value   :
* Comments         :
*                     This function sends a char via SCI.
*
*END*----------------------------------------------------------------------*/
void 
TERMIO_PutChar(char send) 
{
    /* Body */
    char dummy;
    while(!SCI1S1_TDRE){};
    dummy = SCI1S1;
    SCI1D  = send;    
} /* EndBody */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : TERMIO_GetChar
* Returned Value   : the char get via SCI
* Comments         :
*                     This function gets a char via SCI.
*
*END*----------------------------------------------------------------------*/
char 
TERMIO_GetChar(void) 
{
    /* Body */
    char dummy;
    while(!SCI1S1_RDRF){};
    dummy = SCI1S1;
    return SCI1D;  
} /* EndBody */

#if defined(_MCF51_H)
/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : Initialize UART
* Returned Value   :
* Comments         :
*                    This function initializes the UART for console printf/scanf
*
*END*----------------------------------------------------------------------*/
UARTError 
InitializeUART(UARTBaudRate baudRate)
{
    /* Body */
    uint_32     baud_divisor;
    baudRate = kBaud115200;
    /* Calculate baud settings */
    /* baud_divisor = clock_speed/ baudrate + 0.5 */
    baud_divisor = (SYSCLK + (8 * (uint_32)baudRate)) / (16 * (uint_32)baudRate);
    if (baud_divisor > 0x1fff) 
    {
        return kUARTUnknownBaudRate;
    } /* EndIf */
    SCI1BDH = (uint_8)((baud_divisor >> 8) & 0xFF);
    SCI1BDL = (uint_8)(baud_divisor & 0xFF);
    return kUARTNoError;
} /* EndBody */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : WriteUARTN
* Returned Value   :
* Comments         :
*                    This function writes N bytes on the SCI1 console output
*
*END*----------------------------------------------------------------------*/
UARTError 
WriteUARTN
    (
        const void* bytes, 
        unsigned long length
    ) 
{
    /* Body */
    int i;
    char* src = (char*)bytes;
    for(i = 0; i< length; i++) 
    {
        TERMIO_PutChar(*src++);
    } /* EndFor */
    return kUARTNoError;  
} /* EndBody */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : ReadUARTN
* Returned Value   :
* Comments         :
*                    This function reads N bytes on the SCI1 console input
*
*END*----------------------------------------------------------------------*/
UARTError 
ReadUARTN
    (
        void* bytes, 
        unsigned long length
    )
{
    /* Body */
    int i;
    char *dst = (char*)bytes;
    for(i = 0; i< length; i++) 
    {
        *dst++ = TERMIO_GetChar();
    } /* EndFor */
    return kUARTNoError;
} /* EndBody */

#endif

/* EOF */
