/*
 */

/* Include the derivative-specific header file */

#include "MKL25Z4.h"

#define __MK_xxx_H__

#define LITTLE_ENDIAN

#define CORTEX_M0_PLUS

#define FTFA_SUPPORT

#define printf printf_kinetis
#define sprintf sprintf_kinetis

#define MCG_C2_HGO_MASK       MCG_C2_HGO0_MASK
#define MCG_C2_RANGE          MCG_C2_RANGE0
#define MCG_C2_EREFS_MASK     MCG_C2_EREFS0_MASK
#define MCG_C5_PLLCLKEN_MASK  MCG_C5_PLLCLKEN0_MASK
#define MCG_C5_PRDIV          MCG_C5_PRDIV0
#define MCG_C6_VDIV           MCG_C6_VDIV0
#define MCG_S_OSCINIT_MASK    MCG_S_OSCINIT0_MASK
#define MCG_S_LOCK_MASK       MCG_S_LOCK0_MASK

#define SYSCLK   48000000/*FSL: fixed for 48MHz clk, then cpu and bus have the same freq*/
#define BSP_CLOCK_SRC                   (8000000ul)       // 8MHz crystal, oscillator freq
//#define EXTERNAL_OSC /*FSL: only if external osc is used*/

//#define TERM_PORT                       UART1_BASE_PTR
#define UART_PORT                         0
#define USED_PIT0

#define NO_PRINTF_SUPPORT

