/*
 */

/* Include the derivative-specific header file */

#include <MK20D5.h>

#define __MK_xxx_H__

#define LITTLE_ENDIAN

#define printf printf_kinetis
#define sprintf sprintf_kinetis

#define SYSCLK   48000000/*FSL: fixed for 48MHz clk, then cpu and bus have the same freq*/
#define BSP_CLOCK_SRC                   (8000000ul)       // 8MHz crystal, oscillator freq
//#define EXTERNAL_OSC /*FSL: only if external osc is used*/

#define TERM_PORT                       UART1_BASE_PTR
#define USED_PIT0

#define NO_PRINTF_SUPPORT

