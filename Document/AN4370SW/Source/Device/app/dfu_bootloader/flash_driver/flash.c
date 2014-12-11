/********************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2004-2010 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
**************************************************************************//*!
 *
 * @file flash.c
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
#include "flash.h" /* include flash driver header file */
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
 * Returns:         never return
 *
 * Notes:
 *
 *******************************************************************************/
void 
Flash_Init(unsigned char FlashClockDiv)
{
    /* Flash clock between 150-200kHz - > 24MHz/(15*8)=200kHz*/
    FCDIV = FlashClockDiv;
}

/**
 * Execute Flash command at runtime: must run at another FLASH block 
 * that is being modified. Usually from RAM
 * @param flash address to modify
 * @param flash data pointer
 * @param flash number of longwords to modify
 * @param flash command  
 * @return none
 */
#if defined(__IAR_SYSTEMS_ICC__)
__ramfunc
#elif defined(__CWCC__)
__relocate_code__
#endif
unsigned char 
Flash_Cmd(unsigned long *FlashAddress, 
          unsigned short FlashDataCounter, 
          unsigned long *pFlashDataPtr, 
          unsigned char FlashCommand)
{
	unsigned short counter;
	
	/* Check to see if FACCERR or PVIOL is set */
    if (FSTAT&0x30)  
    {         
        /* Clear Flags if set*/
        FSTAT = 0x30;  
    }
    
    /*FSL: count in longwords instead of bytes*/
    counter = (unsigned short)((FlashDataCounter - 1)/4 + 1);

    if (counter)
    {
      do
      {
          /* Wait for the Last Burst Command to complete */
          while(!(FSTAT&FSTAT_FCBEF_MASK))
          {
            SRS = 0x55;
            SRS = 0xAA;
          }/*wait until termination*/
          
          /* Write Data into Flash*/
          *FlashAddress = *pFlashDataPtr;
          FlashAddress++;
          pFlashDataPtr++;
          
          /* Write Command */
          FCMD = FlashCommand;
          
          /* Put FCBEF at 1 */
          FSTAT = FSTAT_FCBEF_MASK;
          
          asm(nop);
          asm(nop);
          asm(nop);
          asm(nop);
          
          /* Check if Flash Access Error or Protection Violation Error are Set */
          if (FSTAT&0x30)
          {     
            /* If so, finish the function returning 1 to indicate error */
            return (1);
          }
          
      }while (--counter);
    }
    /* wait for the last command to complete */
    while ((FSTAT&FSTAT_FCCF_MASK)==0)
    {;}/*wait until termination*/
    
    /* Return zero to indicate that the function executed OK */
    return (0);
}

