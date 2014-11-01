/*
 * File:        tower.h
 * Purpose:     Kinetis tower CPU card definitions
 *
 * Notes:
 */

#ifndef __TOWER_H__
#define __TOWER_H__

#include "mcg.h"

/********************************************************************/

/* Global defines to use for all Tower cards */
#define DEBUG_PRINT

#define CPU_MKL25Z128LK4

/*
* Input Clock Info
*/
#define CLK0_FREQ_HZ        8000000
#define CLK0_TYPE           CRYSTAL

/*
   * PLL Configuration Info
   */
//  #define NO_PLL_INIT  // define this to skip PLL initilaization and stay in default FEI mode

/* The expected PLL output frequency is:
 * PLL out = ((CLKIN/PRDIV) x VDIV)
 * where the CLKIN is CLK0_FREQ_HZ.
 *
 * For more info on PLL initialization refer to the mcg driver files.
 */

  #define PLL0_PRDIV      4       // divide reference by 4 = 2 MHz
  #define PLL0_VDIV       24      // multiply reference by 24 = 48 MHz

  /* Serial Port Info */

  /**************************************************************************
   * Note:
   * 
   * Because of the changes to the UART modules, we can no longer define
   * the TERM_PORT as a base pointer.  The uart functions have been modified 
   * accommodate this change.  Now, TERM_PORT_NUM must be defined as the 
   * number of the UART port desired to use
   *
   * TERM_PORT_NUM = 0  -- This allows you to use UART0; default pins are
   *						PTA14 and PTA15
   *
   * TERM_PORT_NUM = 1  -- This allows you to use UART1; default pins are
   *						PTC3 and PTC4
   * 
   * TERM_PORT_NUM = 2  -- This allows you to use UART2; default pins are
   *						PTD2 and PTD3
   *
   *************************************************************************/
  #define TERM_PORT_NUM       0

  #define TERMINAL_BAUD       19200
  #undef  HW_FLOW_CONTROL

/* tower module LED defines */
  #define LED0_EN (PORTA_PCR16 = PORT_PCR_MUX(1))
  #define LED1_EN (PORTA_PCR17 = PORT_PCR_MUX(1))
  #define LED2_EN (PORTB_PCR8 = PORT_PCR_MUX(1))
  #define LED3_EN (PORTA_PCR5 = PORT_PCR_MUX(1))

  #define LED0_TOGGLE (GPIOA_PTOR = (1<<16))
  #define LED1_TOGGLE (GPIOA_PTOR = (1<<17))
  #define LED2_TOGGLE (GPIOB_PTOR = (1<<8))
  #define LED3_TOGGLE (GPIOA_PTOR = (1<<5))

  #define LED0_OFF (GPIOA_PSOR = (1<<16))
  #define LED1_OFF (GPIOA_PSOR = (1<<17))
  #define LED2_OFF (GPIOB_PSOR = (1<<8))
  #define LED3_OFF (GPIOA_PSOR = (1<<5))

  #define LED0_ON (GPIOA_PCOR = (1<<16))
  #define LED1_ON (GPIOA_PCOR = (1<<17))
  #define LED2_ON (GPIOB_PCOR = (1<<8))
  #define LED3_ON (GPIOA_PCOR = (1<<5))
  /* fast GPIO through IOPORT */
  #define _LED0_TOGGLE (FGPIOA_PTOR = (1<<16))
  #define _LED1_TOGGLE (FGPIOA_PTOR = (1<<17))
  #define _LED2_TOGGLE (FGPIOB_PTOR = (1<<8))
  #define _LED3_TOGGLE (FGPIOA_PTOR = (1<<5))
  //OR
  #define F_LED0_TOGGLE (FGPIOA_PTOR = (1<<16))
  #define F_LED1_TOGGLE (FGPIOA_PTOR = (1<<17))
  #define F_LED2_TOGGLE (FGPIOB_PTOR = (1<<8))
  #define F_LED3_TOGGLE (FGPIOA_PTOR = (1<<5))

  #define F_LED0_OFF (FGPIOA_PSOR = (1&lt;&lt;16))
  #define F_LED1_OFF (FGPIOA_PSOR = (1<<17))
  #define F_LED2_OFF (FGPIOB_PSOR = (1<<8))
  #define F_LED3_OFF (FGPIOA_PSOR = (1<<5))
  
  #define F_LED0_ON (FGPIOA_PCOR = (1&lt;&lt;16))
  #define F_LED1_ON (FGPIOA_PCOR = (1<<17))
  #define F_LED2_ON (FGPIOB_PCOR = (1<<8))
  #define F_LED3_ON (FGPIOA_PCOR = (1<<5))


#endif /* __TOWER_H__ */
