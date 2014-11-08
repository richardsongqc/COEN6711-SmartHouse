/*
 * Note: This file is recreated by the project wizard whenever the MCU is
 *       changed and should not be edited by hand
 */

/* Include the derivative-specific header file */
#include <MC9S08JM60.h>

  /*!< Macro to enter stop modes, STOPE bit in SOPT1 register must be set prior to executing this macro */
#define _Stop asm ( stop; )
#define _Wait asm ( wait; )

#define _MC9S08_H

#define NO_PRINTF_SUPPORT




