/*
 * Note: This file is recreated by the project wizard whenever the MCU is
 *       changed and should not be edited by hand
 */

/* Include the derivative-specific header file */
#include <MCF51JM128.h>

#define _MCF51_H

#define _Stop asm ( mov3q #4,d0; bclr.b d0,SOPT1; stop #0x2000; )
  /*!< Macro to enter stop modes, STOPE bit in SOPT1 register must be set prior to executing this macro */

#define _Wait asm ( mov3q #4,d0; bset.b d0,SOPT1; nop; stop #0x2000; )
  /*!< Macro to enter wait mode */


#define SYSCLK   24000000/*FSL: fixed for bus clk*/
#define NO_PRINTF_SUPPORT                    /*new!*/

#pragma  define_section relocate_code ".relocate_code" far_absolute RX

#pragma  define_section relocate_data ".relocate_data" far_abssolute RW

#pragma  define_section relocate_const ".relocate_const" far_abssolute R

#define __relocate_code__   __declspec(relocate_code)
#define __relocate_data__   __declspec(relocate_data)
#define __relocate_const__  __declspec(relocate_const)