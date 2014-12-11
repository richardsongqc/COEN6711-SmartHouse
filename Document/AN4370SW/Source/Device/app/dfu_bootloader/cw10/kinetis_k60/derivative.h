/*
 * Note: This file is recreated by the project wizard whenever the MCU is
 *       changed and should not be edited by hand
 */

/* Include the derivative-specific header file */

#include <MK60N512VMD100.h>

#define __MK_xxx_H__

#define LITTLE_ENDIAN

#define printf printf_kinetis
#define sprintf sprintf_kinetis

#define SYSCLK   48000000/*FSL: fixed for 48MHz clk, then cpu and bus have the same freq*/
#define BSP_CLOCK_SRC                   (50000000ul)       // 50MHz osc, oscillator freq
#define EXTERNAL_OSC /*FSL: only if external osc is used*/

#define TERM_PORT                       UART3_BASE_PTR
#define USED_PIT0

#define NO_PRINTF_SUPPORT

#pragma define_section relocate_code ".relocate_code" ".relocate_code" ".relocate_code" far_abs RX
#pragma define_section relocate_data ".relocate_data" ".relocate_data" ".relocate_data" RW
#pragma define_section relocate_const ".relocate_const" ".relocate_const" ".relocate_const" far_abs R

#define __relocate_code__   __declspec(relocate_code)
#define __relocate_data__   __declspec(relocate_data)
#define __relocate_const__  __declspec(relocate_const)

