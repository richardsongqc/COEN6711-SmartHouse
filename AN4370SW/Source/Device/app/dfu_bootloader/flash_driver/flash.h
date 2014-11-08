/********************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2004-2010 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
**************************************************************************//*!
 *
 * @file flash.h
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

/* error code */
#define Flash_OK              0x00

#define FLASH_MASS_ERASE_CMD  0x41
#define FLASH_ERASE_CMD       0x40
#define FLASH_PROGRAM_CMD     0x20
#define FLASH_BURST_CMD       0x25

/********************************** Constant ***********************************/

/*********************************** Variables *********************************/

/*********************************** Prototype *********************************/
extern void Flash_Init(unsigned char FlashClockDiv);
/**
 * Execute Flash command at runtime: must run at another FLASH block 
 * that is being modified. Usually from RAM
 * @param flash address to modify
 * @param flash data pointer
 * @param flash number of longwords to modify
 * @param flash command  
 * @return none
 */
unsigned char Flash_Cmd(unsigned long *FlashAddress, 
          unsigned short FlashDataCounter, 
          unsigned long *pFlashDataPtr, 
          unsigned char FlashCommand);
  
/* Macros to call the function using the different features */
#define Flash_Erase(Address) \
          Flash_Cmd(Address, (unsigned short)1, 0, FLASH_ERASE_CMD)
  
#define Flash_Program(Address, Data) \
          Flash_Cmd(Address, (unsigned short)1, Data, FLASH_PROGRAM_CMD)
  
#define Flash_Burst(Address, DataPtr, Size) \
          Flash_Cmd(Address, (unsigned short)Size, DataPtr, FLASH_BURST_CMD)

/*FSL: glue logic to allow DFU wrapper*/
#define Flash_SectorErase     Flash_Erase
#define Flash_ByteProgram     Flash_Burst

