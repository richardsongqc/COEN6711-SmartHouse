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
*
* Comments:  This file includes operation of serial communication interface.
*
*
*END************************************************************************/
#include "sci.h"
#include "types.h"
#include "derivative.h"

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : sci_init
* Returned Value   :
* Comments         : This function initilizes the SCI 1 baud rate.
*
*
*END*----------------------------------------------------------------------*/
#define BAUDRATE 115200

void
sci_init(void)
{
    register uint_16 ubd;

#if defined(MCU_MK60N512VMD100)/*K60*/

    //TWR-OSJTAG:
    //******SIM_SCGC5   |= SIM_SCGC5_PORTE_MASK;    /* Enable clock gating to PORTE */
    /* Enable the UART5_TXD function on PTE8 */
    //*******PORTE_PCR8 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
    /* Enable the UART5_RXD function on PTE9 */
    //*******PORTE_PCR9 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
    //*****SIM_SCGC1 |= SIM_SCGC1_UART5_MASK;

    //TWR-SER
    SIM_SCGC5   |= SIM_SCGC5_PORTC_MASK;    /* Enable clock gating to PORTC */
    /* Enable the UART3_TXD function on PTC17 */
    PORTC_PCR17 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
    /* Enable the UART3_RXD function on PTC16 */
    PORTC_PCR16 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
    SIM_SCGC4 |= SIM_SCGC4_UART3_MASK;

#elif defined(MCU_MK40N512VMD100)/*K40*/

    //TWR-OSJTAG:
    //******SIM_SCGC5   |= SIM_SCGC5_PORTE_MASK;    /* Enable clock gating to PORTE */
    /* Enable the UART5_TXD function on PTE8 */
    //*******PORTE_PCR8 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
    /* Enable the UART5_RXD function on PTE9 */
    //*******PORTE_PCR9 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
    //*****SIM_SCGC1 |= SIM_SCGC1_UART5_MASK;

    //TWR-SER
    SIM_SCGC5   |= SIM_SCGC5_PORTC_MASK;    /* Enable clock gating to PORTC */
    /* Enable the UART3_TXD function on PTC17 */
    PORTC_PCR17 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
    /* Enable the UART3_RXD function on PTC16 */
    PORTC_PCR16 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
    SIM_SCGC4 |= SIM_SCGC4_UART3_MASK;

#elif defined(MCU_MK20D5)/*K20*/
    /*FSL: set for UART1*/

    SIM_SCGC5   |= SIM_SCGC5_PORTE_MASK;    /* Enable clock gating to PORTD */
    /* Enable the UART1_TXD function on PTE0 (1) */
    PORTE_PCR0 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
    /* Enable the UART1_RXD function on PTE1 (2) */
    PORTE_PCR1 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
    SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;
#elif defined(MCU_MKL25Z4)/*LK2*/
    /*FSL: set for UART0*/

    SIM_SCGC5   |= SIM_SCGC5_PORTA_MASK;    /* Enable clock gating to PORTD */
    /* Enable the UART_TXD function on PTA14 */
    PORTA_PCR14 = PORT_PCR_MUX(0x3); // UART0 is alt3 function for this pin
    /* Enable the UART_RXD function on PTA15 */
    PORTA_PCR15 = PORT_PCR_MUX(0x3); // UART0 is alt3 function for this pin
    SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
    SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1); // select the PLLFLLCLK as UART0 clock source

#if (UART_PORT==0)
#define TERM_PORT         UART0_BASE_PTR
#elif (UART_PORT==1)
#define TERM_PORT         UART1_BASE_PTR
#elif (UART_PORT==2)
#define TERM_PORT         UART2_BASE_PTR
#else
#error "Wrong implementation"
#endif

#elif defined(MCU_MK70F15)/*K70*/
    /*FSL: set for UART2*/

    SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;    /* Enable clock gating to PORTD */
    /* Enable the UART2_TXD function on PTD3 */
    PORTE_PCR16 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
    /* Enable the UART2_RXD function on PTD2 */
    PORTE_PCR17 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
    SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;
#else
#error "Needs SW for other Kinetis ports!"
#endif

    UART_C2_REG(TERM_PORT) &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
    UART_C1_REG(TERM_PORT) = 0;

    ubd = (uint_16)((SYSCLK)/(BAUDRATE * 16));
    UART_BDH_REG(TERM_PORT) |= UART_BDH_SBR(((ubd & 0x1F00) >> 8));
    UART_BDL_REG(TERM_PORT) = (uint_8)(ubd & UART_BDL_SBR_MASK);
    UART_C2_REG(TERM_PORT) |= (UART_C2_TE_MASK | UART_C2_RE_MASK );
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : uart_putchar
* Returned Value   :
* Comments         :
*                     This function sends a char via SCI.
*
*END*----------------------------------------------------------------------*/
void
uart_putchar (char ch)
{
    /* Wait until space is available in the FIFO */
    while(!(UART_S1_REG(TERM_PORT) & UART_S1_TDRE_MASK))
    {;}

    /* Send the character */
    UART_D_REG(TERM_PORT) = (uint_8)ch;
 }

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : uart_getchar
* Returned Value   : the char get via SCI
* Comments         :
*                     This function gets a char via SCI.
*
*END*----------------------------------------------------------------------*/
char
uart_getchar (void)
{
    /* Wait until character has been received */
    while (!(UART_S1_REG(TERM_PORT) & UART_S1_RDRF_MASK))
    {;}

    /* Return the 8-bit data from the receiver */
    return UART_D_REG(TERM_PORT);
}

/********************************************************************/
void
out_char (char ch)
{
    uart_putchar(ch);
}
