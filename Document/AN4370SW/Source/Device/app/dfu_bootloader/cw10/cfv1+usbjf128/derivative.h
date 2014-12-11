/*
 * Note: This file is recreated by the project wizard whenever the MCU is
 *       changed and should not be edited by hand
 */

/* Include the derivative-specific header file */
#include <MCF51JF128.h>

#define _Stop asm ( mov3q #4,d0; bclr.b d0,SOPT1; stop #0x2000; )
#define _Wait asm ( mov3q #4,d0; bset.b d0,SOPT1; nop; stop #0x2000; )

#define _MCF51_PLUS_H

#define BSP_CLOCK_SRC                   8000000ul
//#define EXTERNAL_OSC

#define TERM_PORT                       UART0_BASE_PTR
#define MTIM0_SUPPORT

#define NO_PRINTF_SUPPORT                    /*new!*/

#pragma  define_section relocate_code ".relocate_code" far_absolute RX
#pragma  define_section relocate_data ".relocate_data" far_abssolute RW
#pragma  define_section relocate_const ".relocate_const" far_abssolute R

#define __relocate_code__   __declspec(relocate_code)
#define __relocate_data__   __declspec(relocate_data)
#define __relocate_const__  __declspec(relocate_const)

/*extra definitions*/

/* PTAPF3 Bit Fields */
#define MXC_PTAPF3_A0_MASK                       0xFu
#define MXC_PTAPF3_A0_SHIFT                      0
#define MXC_PTAPF3_A0(x)                         (((uint8_t)(((uint8_t)(x))<<MXC_PTAPF3_A0_SHIFT))&MXC_PTAPF3_A0_MASK)
#define MXC_PTAPF3_A1_MASK                       0xF0u
#define MXC_PTAPF3_A1_SHIFT                      4
#define MXC_PTAPF3_A1(x)                         (((uint8_t)(((uint8_t)(x))<<MXC_PTAPF3_A1_SHIFT))&MXC_PTAPF3_A1_MASK)

/* PTCPF1 Bit Fields */
#define MXC_PTCPF1_C4_MASK                       0xFu
#define MXC_PTCPF1_C4_SHIFT                      0
#define MXC_PTCPF1_C4(x)                         (((uint8_t)(((uint8_t)(x))<<MXC_PTCPF1_C4_SHIFT))&MXC_PTCPF1_C4_MASK)
#define MXC_PTCPF1_C5_MASK                       0xF0u
#define MXC_PTCPF1_C5_SHIFT                      4
#define MXC_PTCPF1_C5(x)                         (((uint8_t)(((uint8_t)(x))<<MXC_PTCPF1_C5_SHIFT))&MXC_PTCPF1_C5_MASK)

/* PTBPF3 Bit Fields */
#define MXC_PTBPF3_B0_MASK                       0xFu
#define MXC_PTBPF3_B0_SHIFT                      0
#define MXC_PTBPF3_B0(x)                         (((uint8_t)(((uint8_t)(x))<<MXC_PTBPF3_B0_SHIFT))&MXC_PTBPF3_B0_MASK)
#define MXC_PTBPF3_B1_MASK                       0xF0u
#define MXC_PTBPF3_B1_SHIFT                      4
#define MXC_PTBPF3_B1(x)                         (((uint8_t)(((uint8_t)(x))<<MXC_PTBPF3_B1_SHIFT))&MXC_PTBPF3_B1_MASK)

/* PTBPF1 Bit Fields */
#define MXC_PTBPF1_B4_MASK                       0xFu
#define MXC_PTBPF1_B4_SHIFT                      0
#define MXC_PTBPF1_B4(x)                         (((uint8_t)(((uint8_t)(x))<<MXC_PTBPF1_B4_SHIFT))&MXC_PTBPF1_B4_MASK)
#define MXC_PTBPF1_B5_MASK                       0xF0u
#define MXC_PTBPF1_B5_SHIFT                      4
#define MXC_PTBPF1_B5(x)                         (((uint8_t)(((uint8_t)(x))<<MXC_PTBPF1_B5_SHIFT))&MXC_PTBPF1_B5_MASK)
