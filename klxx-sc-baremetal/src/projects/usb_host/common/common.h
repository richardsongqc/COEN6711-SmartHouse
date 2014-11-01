/*
 * File:        common.h
 * Purpose:     File to be included by all project files
 *
 * Notes:
 */

#ifndef _COMMON_H_
#define _COMMON_H_

/********************************************************************/

/*
 * Debug prints ON (#define) or OFF (#undef)
 */

#define DEBUG
#define DEBUG_PRINT

/* 
 * Include the generic CPU header file 
 */
#include "arm_cm0.h"

/* 
 * Include the platform specific header file 
 */
#if (defined(FIREBIRD))
  #include "firebird.h"
#elif (defined(TOWER))
  #include "tower.h"
#elif (defined(FREEDOM))
  #include "freedom.h"
#elif (defined(BACES))
  #include "baces.h"
#else
  #error "No valid platform defined"
#endif

/* 
 * Include the cpu specific header file 
*/
#if (defined (CPU_MKL05Z32LF4))
  #include "MKL05Z32LF4.h"
#elif (defined(CPU_MKL25Z128LK4))
//  #include "MKL25Z128LK4.h"
  #include "MKL25Z4.h"
#elif (defined(CPU_MKM34Z128LL5))
  #include "MKM34Z128LL5.h"
#else
  #error "No valid CPU defined"
#endif


/* 
 * Include any toolchain specfic header files 
 */
#if (defined(__MWERKS__))
  #include "mwerks.h"
#elif (defined(__DCC__))
  #include "build/wrs/diab.h"
#elif (defined(__ghs__))
  #include "build/ghs/ghs.h"
#elif (defined(__GNUC__))
  #include "build/gnu/gnu.h"
#elif (defined(IAR))
  #include "iar.h"
#elif (defined(KEIL))
  
#else
#warning "No toolchain specific header included"
#endif

/* 
 * Include common utilities
 */
#include "assert.h"
#include "io.h"
#include "startup.h"
#include "stdlib.h"
#include "intrinsics.h"

/* 
 * Global Variable
 */


/********************************************************************/

#endif /* _COMMON_H_ */
