/*
 * File:        freedom.h
 * Purpose:     Kinetis Freedom hardware definitions
 *
 * Notes:
 */

#ifndef __FREEDOM_H__
#define __FREEDOM_H__

#include "mcg.h"

/********************************************************************/

/* Global defines to use for all Tower cards */
#define DEBUG_PRINT

// Define which CPU  you are using.
#define CPU_MKL25Z128LK4

/*
* Input Clock Info
*/
#define CLK0_FREQ_HZ        8000000
#define CLK0_TYPE           CRYSTAL

/*
 * PLL Configuration Info
 */
//#define NO_PLL_INIT  // define this to skip PLL initilaization and stay in default FEI mode

/* The expected PLL output frequency is:
 * PLL out = (((CLKIN/PRDIV) x VDIV) / 2)
 * where the CLKIN is CLK0_FREQ_HZ.
 *
 * For more info on PLL initialization refer to the mcg driver files.
 */

#define PLL0_PRDIV      4       // divider eference by 4 = 2 MHz
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

/* Uses UART0 for both Open SDA and TWR-SER Tower card */

#define TERM_PORT_NUM       0

#define TERMINAL_BAUD       19200
#undef  HW_FLOW_CONTROL

  #define LED0_EN (PORTB_PCR19 = PORT_PCR_MUX(1))
  #define LED1_EN (PORTB_PCR18 = PORT_PCR_MUX(1))
  #define LED2_EN (PORTD_PCR0 = PORT_PCR_MUX(1))

  #define LED0_TOGGLE (GPIOB_PTOR = (1<<19))
  #define LED1_TOGGLE (GPIOB_PTOR = (1<<18))
  #define LED2_TOGGLE (GPIOD_PTOR = 1)

  #define LED0_OFF (GPIOB_PSOR = (1<<19))
  #define LED1_OFF (GPIOB_PSOR = (1<<18))
  #define LED2_OFF (GPIOD_PSOR = 1)

  #define LED0_ON (GPIOB_PCOR = (1<<19))
  #define LED1_ON (GPIOB_PCOR = (1<<18))
  #define LED2_ON (GPIOD_PCOR = 1)
  /* fast GPIO through IOPORT */
  #define _LED0_TOGGLE (FGPIOB_PTOR = (1<<19))
  #define _LED1_TOGGLE (FGPIOB_PTOR = (1<<18))
  #define _LED2_TOGGLE (FGPIOD_PTOR = 1)
  //OR
  #define F_LED0_TOGGLE (FGPIOB_PTOR = (1<<19))
  #define F_LED1_TOGGLE (FGPIOB_PTOR = (1<<18))
  #define F_LED2_TOGGLE (FGPIOD_PTOR = 1)

  #define F_LED0_OFF (FGPIOB_PSOR = (1<<19))
  #define F_LED1_OFF (FGPIOB_PSOR = (1<<18))
  #define F_LED2_OFF (FGPIOD_PSOR = 1)
  
  #define F_LED0_ON (FGPIOB_PCOR = (1<<19))
  #define F_LED1_ON (FGPIOB_PCOR = (1<<18))
  #define F_LED2_ON (FGPIOD_PCOR = 1)


#endif /* __FREEDOM_H__ */
