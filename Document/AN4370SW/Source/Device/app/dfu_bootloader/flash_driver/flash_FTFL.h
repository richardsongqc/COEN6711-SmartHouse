/********************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2004-2010 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
**************************************************************************//*!
 *
 * @file flash_FTFL.h
 *
 * @author
 *
 * @version
 *
 * @date
 *
 * @brief flash driver header file
 *
 *****************************************************************************/

/*********************************** Includes ***********************************/

/*********************************** Macros ************************************/

/*********************************** Defines ***********************************/
#ifndef _FLASH_FTFL_H_
#define _FLASH_FTFL_H_
#include "types.h"

/*********************************** Defines ***********************************/
#if defined(FTFE_SUPPORT)
/*FSL: new Kinetis/ColdFire+ devices might use this new FTFE implementation instead of FTFL*/

#define     FTFL_FSTAT_ACCERR_MASK       FTFE_FSTAT_ACCERR_MASK
#define     FTFL_FSTAT_FPVIOL_MASK       FTFE_FSTAT_FPVIOL_MASK
#define     FTFL_FSTAT_RDCOLERR_MASK     FTFE_FSTAT_RDCOLERR_MASK
#define     FTFL_FSTAT_CCIF_MASK         FTFE_FSTAT_CCIF_MASK
#define     FTFL_FSTAT_MGSTAT0_MASK      FTFE_FSTAT_MGSTAT0_MASK
#define     FTFL_FSTAT                   FTFE_FSTAT
#define     FTFL_FCCOB0                  FTFE_FCCOB0
#define     FTFL_FCCOB1                  FTFE_FCCOB1
#define     FTFL_FCCOB2                  FTFE_FCCOB2
#define     FTFL_FCCOB3                  FTFE_FCCOB3
#define     FTFL_FCCOB4                  FTFE_FCCOB4
#define     FTFL_FCCOB5                  FTFE_FCCOB5
#define     FTFL_FCCOB6                  FTFE_FCCOB6
#define     FTFL_FCCOB7                  FTFE_FCCOB7

#elif defined(FTFA_SUPPORT)

#define     FTFL_FSTAT_ACCERR_MASK       FTFA_FSTAT_ACCERR_MASK
#define     FTFL_FSTAT_FPVIOL_MASK       FTFA_FSTAT_FPVIOL_MASK
#define     FTFL_FSTAT_RDCOLERR_MASK     FTFA_FSTAT_RDCOLERR_MASK
#define     FTFL_FSTAT_CCIF_MASK         FTFA_FSTAT_CCIF_MASK
#define     FTFL_FSTAT_MGSTAT0_MASK      FTFA_FSTAT_MGSTAT0_MASK
#define     FTFL_FSTAT                   FTFA_FSTAT
#define     FTFL_FCCOB0                  FTFA_FCCOB0
#define     FTFL_FCCOB1                  FTFA_FCCOB1
#define     FTFL_FCCOB2                  FTFA_FCCOB2
#define     FTFL_FCCOB3                  FTFA_FCCOB3
#define     FTFL_FCCOB4                  FTFA_FCCOB4
#define     FTFL_FCCOB5                  FTFA_FCCOB5
#define     FTFL_FCCOB6                  FTFA_FCCOB6
#define     FTFL_FCCOB7                  FTFA_FCCOB7

#endif

/* error code */
#define Flash_OK          0x00
#define Flash_FACCERR     0x01
#define Flash_FPVIOL      0x02
#define Flash_MGSTAT0	  0x04
#define Flash_RDCOLERR	  0x08
#define Flash_NOT_ERASED   0x10
#define Flash_CONTENTERR   0x11

/* flash commands */
#if defined(FTFE_SUPPORT)
#define FlashCmd_ProgramLongWord  	0x07/*Phrase write*/
#else /*Including FTFA_SUPPORT and FTFL_SUPPORT (default) */
#define FlashCmd_ProgramLongWord  	0x06/*longword write*/
#endif
#define FlashCmd_SectorErase    	0x09


/********************************** Constant ***********************************/

/*********************************** Variables *********************************/

/*********************************** Prototype *********************************/
void Flash_Init(unsigned char FlashClockDiv);
unsigned char Flash_SectorErase(uint_32 *FlashPtr);
unsigned char Flash_ByteProgram(uint_32 *FlashStartAddress,uint_32 *DataSrcPtr,uint_32 NumberOfBytes);

#if defined(__IAR_SYSTEMS_ICC__)
__ramfunc
#elif defined(__CWCC__)
__relocate_code__
#endif
void SpSub(void);
void SpSubEnd(void);
#endif /*_FLASH_FTFL_H_*/
