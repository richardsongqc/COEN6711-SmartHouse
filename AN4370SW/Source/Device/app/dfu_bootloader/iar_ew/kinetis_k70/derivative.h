/*
 * Note: This file is recreated by the project wizard whenever the MCU is
 *       changed and should not be edited by hand
 */

/* Include the derivative-specific header file */

#include <MK70F15.h>
 
#define __MK_xxx_H__

#define LITTLE_ENDIAN

#define printf printf_kinetis
#define sprintf sprintf_kinetis

#define FTFE_SUPPORT

#define SYSCLK   60000000/*FSL: fixed for 48MHz clk, then cpu and bus have the same freq*/
/*FSL: both definitions are not used for this port*/
#define BSP_CLOCK_SRC                   (50000000ul)       // 50MHz osc, oscillator freq
#define EXTERNAL_OSC /*FSL: only if external osc is used*/

#define TERM_PORT                       UART2_BASE_PTR
#define USED_PIT0

#define NO_PRINTF_SUPPORT
