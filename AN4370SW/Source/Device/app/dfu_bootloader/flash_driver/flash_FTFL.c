/********************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2004-2010 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
**************************************************************************//*!
 *
 * @file flash_FTFL.c
 *
 * @author
 *
 * @version
 *
 * @date
 *
 * @brief The file includes flash routines
 *
 *****************************************************************************/

/*********************************** Includes ***********************************/
#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "flash_FTFL.h" /* include flash driver header file */
/*********************************** Macros ************************************/

/*********************************** Defines ***********************************/

/********************************** Constant ***********************************/

/*********************************** Variables *********************************/

/*********************************** Prototype *********************************/

/*********************************** Function **********************************/

/*******************************************************************************
 * Function:        Flash_Init
 *
 * Description:     Set the flash clock
 *
 * Returns:         none
 *
 * Notes:
 *
 *******************************************************************************/
void 
Flash_Init(unsigned char FlashClockDiv)
{
    UNUSED(FlashClockDiv)
			
    /* checking access error */
    if (FTFL_FSTAT & (FTFL_FSTAT_ACCERR_MASK|FTFL_FSTAT_FPVIOL_MASK|FTFL_FSTAT_RDCOLERR_MASK))
    {
        /* clear error flag */
    	FTFL_FSTAT |= (FTFL_FSTAT_ACCERR_MASK|FTFL_FSTAT_FPVIOL_MASK|FTFL_FSTAT_RDCOLERR_MASK);
    }
}

/*******************************************************************************
 * Function:        Flash_SectorErase
 *
 * Description:     erase a sector of the flash
 *
 * Returns:         Error Code
 *
 * Notes:
 *
 *******************************************************************************/
unsigned char 
Flash_SectorErase(uint_32 *FlashPointer)
{
    unsigned char Return = Flash_OK;
    uint_32 FlashPtr = (uint_32)FlashPointer;

    /* Allocate space on stack to run flash command out of SRAM */
    /* wait till CCIF is set*/
    while (!(FTFL_FSTAT & FTFL_FSTAT_CCIF_MASK))
    {;}
    
    /* Write command to FCCOB registers */
    FTFL_FCCOB0 = FlashCmd_SectorErase;
    FTFL_FCCOB1 = (uint_8)(FlashPtr >> 16);
    FTFL_FCCOB2 = (uint_8)((FlashPtr >> 8) & 0xFF);
    FTFL_FCCOB3 = (uint_8)(FlashPtr & 0xFF);
    
    /* Launch command */
    SpSub();
    /* checking access error */
    if (FTFL_FSTAT & FTFL_FSTAT_ACCERR_MASK)
    {
        /* clear error flag */
    	FTFL_FSTAT |= FTFL_FSTAT_ACCERR_MASK;

        /* update return value*/
        Return |= Flash_FACCERR;
    }
    /* checking protection error */
    else if (FTFL_FSTAT & FTFL_FSTAT_FPVIOL_MASK)
    {
    	/* clear error flag */
    	FTFL_FSTAT |= FTFL_FSTAT_FPVIOL_MASK;

        /* update return value*/
        Return |= Flash_FPVIOL;
    }
    else if (FTFL_FSTAT & FTFL_FSTAT_RDCOLERR_MASK)
    {
       	/* clear error flag */
       	FTFL_FSTAT |= FTFL_FSTAT_RDCOLERR_MASK;

       	/* update return value*/
       	Return |= Flash_RDCOLERR;
    }
    /* checking MGSTAT0 non-correctable error */
    else if (FTFL_FSTAT & FTFL_FSTAT_MGSTAT0_MASK)
    {
    	Return |= Flash_MGSTAT0;
    }
    /* function return */
    return  Return;
}

/*******************************************************************************
 * Function:        Flash_ByteProgram
 *
 * Description:     byte program the flash
 *
 * Returns:         Error Code
 *
 * Notes:
 *
 *******************************************************************************/
unsigned char 
Flash_ByteProgram(uint_32 *FlashStartAddress,uint_32 *DataSrcPtr,uint_32 NumberOfBytes)
{
    unsigned char Return = Flash_OK;
    uint_32 FlashStartAdd = (uint_32)FlashStartAddress;/*FSL: glue logic*/
    
    /* Allocate space on stack to run flash command out of SRAM */
    uint_32 size_buffer;
    if (NumberOfBytes == 0)
    {
    	return Flash_CONTENTERR;
    }
    else
    {
    	size_buffer = (NumberOfBytes - 1)/4 + 1;	
    }
    /* wait till CCIF is set*/
    while (!(FTFL_FSTAT & FTFL_FSTAT_CCIF_MASK))
    {;}
        
    while ((size_buffer) && (Return == Flash_OK))
    {
      /*FSL: for FTFL, 4-byte aligned, for FTFE, 8 byte aligned!*/
      
       /* Write command to FCCOB registers */
       FTFL_FCCOB0 = FlashCmd_ProgramLongWord;
       FTFL_FCCOB1 = (uint_8)(FlashStartAdd >> 16);
       FTFL_FCCOB2 = (uint_8)((FlashStartAdd >> 8) & 0xFF);
       FTFL_FCCOB3 = (uint_8)(FlashStartAdd & 0xFF);
#ifdef __MK_xxx_H__ /*little endian*/
       FTFL_FCCOB4 = (uint_8)(*((uint_8*)DataSrcPtr+3));
       FTFL_FCCOB5 = (uint_8)(*((uint_8*)DataSrcPtr+2));
       FTFL_FCCOB6 = (uint_8)(*((uint_8*)DataSrcPtr+1));
       FTFL_FCCOB7 = (uint_8)(*((uint_8*)DataSrcPtr+0));
#else /* Big endian */
       FTFL_FCCOB4 = (uint_8)(*((uint_8*)DataSrcPtr+0));
       FTFL_FCCOB5 = (uint_8)(*((uint_8*)DataSrcPtr+1));
       FTFL_FCCOB6 = (uint_8)(*((uint_8*)DataSrcPtr+2));
       FTFL_FCCOB7 = (uint_8)(*((uint_8*)DataSrcPtr+3));
#endif
#if defined(FTFE_SUPPORT)
#ifdef __MK_xxx_H__ /*little endian*/
       FTFE_FCCOB8 = (uint_8)(*((uint_8*)DataSrcPtr+7));
       FTFE_FCCOB9 = (uint_8)(*((uint_8*)DataSrcPtr+6));
       FTFE_FCCOBA = (uint_8)(*((uint_8*)DataSrcPtr+5));
       FTFE_FCCOBB = (uint_8)(*((uint_8*)DataSrcPtr+4));
#else /* Big endian */
       FTFE_FCCOB8 = (uint_8)(*((uint_8*)DataSrcPtr+4));
       FTFE_FCCOB9 = (uint_8)(*((uint_8*)DataSrcPtr+5));
       FTFE_FCCOBA = (uint_8)(*((uint_8*)DataSrcPtr+6));
       FTFE_FCCOBB = (uint_8)(*((uint_8*)DataSrcPtr+7));
#endif
#endif
       
       /* Launch command */
       SpSub();
	
       /* checking access error */
       if (FTFL_FSTAT & FTFL_FSTAT_ACCERR_MASK)
       {
          /* clear error flag */
          FTFL_FSTAT |= FTFL_FSTAT_ACCERR_MASK;

          /* update return value*/
          Return |= Flash_FACCERR;
       }
       /* checking protection error */
       else if (FTFL_FSTAT & FTFL_FSTAT_FPVIOL_MASK)
       {
          /* clear error flag */
          FTFL_FSTAT |= FTFL_FSTAT_FPVIOL_MASK;

          /* update return value*/
          Return |= Flash_FPVIOL;
       }
       else if (FTFL_FSTAT & FTFL_FSTAT_RDCOLERR_MASK)
       {
          /* clear error flag */
          FTFL_FSTAT |= FTFL_FSTAT_RDCOLERR_MASK;

          /* update return value*/
          Return |= Flash_RDCOLERR;
       }
       /* checking MGSTAT0 non-correctable error */
       else if (FTFL_FSTAT & FTFL_FSTAT_MGSTAT0_MASK)
       {
          Return |= Flash_MGSTAT0;
       }
       /* decrement byte count */
#ifndef FTFE_SUPPORT
       size_buffer --;
       (uint_32*)DataSrcPtr++;
       FlashStartAdd +=4;
#else
       size_buffer-=2;
       (uint_32*)DataSrcPtr++;
       (uint_32*)DataSrcPtr++;
       FlashStartAdd +=8;
#endif
    }
    /* function return */
    return  Return;
}

/*******************************************************************************
 * Function:        SpSub
 *
 * Description:     Execute the Flash write while running out of SRAM
 *
 * Returns:         none
 *
 * Notes:
 *
 *******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)
__ramfunc
#elif defined(__CWCC__)
__relocate_code__
#endif
void 
SpSub(void)
{
    /* Launch command */
    FTFL_FSTAT |= FTFL_FSTAT_CCIF_MASK;    
    /* wait for command completion */
    while (!(FTFL_FSTAT & FTFL_FSTAT_CCIF_MASK)) {};
}
/* Leave this immediately after SpSub */
void SpSubEnd(void) {}
