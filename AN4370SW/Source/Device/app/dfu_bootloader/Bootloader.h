/******************************************************************************
 *
 * Freescale Semiconductor Inc.
 * (c) Copyright 2004-2009 Freescale Semiconductor, Inc.
 * ALL RIGHTS RESERVED.
 *
 **************************************************************************//*!
 *
 * @file Bootloader.h
 *
 * @author
 *
 * @version
 *
 * @date
 *
 * @brief This file is for a USB Mass-Storage Device bootloader.  This file
 *           has the structures and definitions for the bootloader
 *
 *****************************************************************************/
#ifndef _BOOTLOADER_H_
#define _BOOTLOADER_H_

#include "types.h"
#include "derivative.h"

#if (defined __MCF52259_H__)
#define MIN_RAM1_ADDRESS        0x20000000
#define MAX_RAM1_ADDRESS        0x2000FFFF
#define MIN_FLASH1_ADDRESS      0x00000000
#define MAX_FLASH1_ADDRESS      0x0007FFFF
#define IMAGE_ADDR              ((uint_32_ptr)0x9000)
#define ERASE_SECTOR_SIZE       0x1000  /* 4K bytes*/
#define FIRMWARE_SIZE_ADD       0x0007FFF0
#elif (defined _MCF51JM128_H)
#define MIN_RAM1_ADDRESS        0x00800000
#define MAX_RAM1_ADDRESS        0x00803FFF
#define MIN_FLASH1_ADDRESS      0x00000000
#define MAX_FLASH1_ADDRESS      0x0001FFFF
#define IMAGE_ADDR              ((uint_32_ptr)0x0A000)
#define ERASE_SECTOR_SIZE       0x0400  /* 1K bytes*/
#define FIRMWARE_SIZE_ADD       0x0001FFF0
#elif (defined MCU_mcf51jf128)
#define MIN_RAM1_ADDRESS        0x00800000
#define MAX_RAM1_ADDRESS        0x00807FFF
#define MIN_FLASH1_ADDRESS      0x00000000
#define MAX_FLASH1_ADDRESS      0x0001FFFF
#define IMAGE_ADDR              ((uint_32_ptr)0x0A000)
#define ERASE_SECTOR_SIZE       0x0400  /* 1K bytes*/
#define FIRMWARE_SIZE_ADD       0x0001FFF0
#elif (defined MCU_MK70F15)
#define MIN_RAM1_ADDRESS        0x1FFF0000
#define MAX_RAM1_ADDRESS        0x20010000
#define MIN_FLASH1_ADDRESS      0x00000000
#define MAX_FLASH1_ADDRESS      0x000FFFFF
#define IMAGE_ADDR              ((uint_32_ptr)0xA000)
#define ERASE_SECTOR_SIZE       0x1000  /* 4K bytes*/
#define FIRMWARE_SIZE_ADD       0x000FFFF0
#elif (defined MCU_MK60N512VMD100)
#define MIN_RAM1_ADDRESS        0x1FFF0000
#define MAX_RAM1_ADDRESS        0x20010000
#define MIN_FLASH1_ADDRESS      0x00000000
#define MAX_FLASH1_ADDRESS      0x0003FFFF//0x0007FFFF
#define IMAGE_ADDR              ((uint_32_ptr)0xA000)
#define ERASE_SECTOR_SIZE       0x800  /* 2K bytes*/
#define FIRMWARE_SIZE_ADD       0x0003FFF0
#elif (defined MCU_MK40N512VMD100)/*new!*/
#define MIN_RAM1_ADDRESS        0x1FFF0000
#define MAX_RAM1_ADDRESS        0x20010000
#define MIN_FLASH1_ADDRESS      0x00000000
#define MAX_FLASH1_ADDRESS      0x0003FFFF
#define IMAGE_ADDR              ((uint_32_ptr)0xA000)
#define ERASE_SECTOR_SIZE       0x800  /* 2K bytes*/
#define FIRMWARE_SIZE_ADD       0x0003FFF0
#elif (defined MCU_MK20D5)/*new!*/
#define MIN_RAM1_ADDRESS        0x1FFF0000
#define MAX_RAM1_ADDRESS        0x20010000
#define MIN_FLASH1_ADDRESS      0x00000000
#define MAX_FLASH1_ADDRESS      0x0001FFFF
#define IMAGE_ADDR              ((uint_32_ptr)0xA000)
#define ERASE_SECTOR_SIZE       0x400  /* 1K bytes*/
#define FIRMWARE_SIZE_ADD       0x0001FFF0
#elif (defined MCU_MKL25Z4)/*new!*/
#define MIN_RAM1_ADDRESS        0x1FFFF000
#define MAX_RAM1_ADDRESS        0x20003000
#define MIN_FLASH1_ADDRESS      0x00000000
#define MAX_FLASH1_ADDRESS      0x0001FFFF
#define IMAGE_ADDR              ((uint_32_ptr)0xA000)
#define ERASE_SECTOR_SIZE       0x400  /* 1K bytes*/
#define FIRMWARE_SIZE_ADD       0x0001FFF0
#elif (defined _MC9S08JM60_H)
#define MIN_RAM1_ADDRESS        0x00B0
#define MAX_RAM1_ADDRESS        0x10AF
#define MIN_FLASH2_ADDRESS      0x10B0
#define MAX_FLASH2_ADDRESS      0x17FF
#define MIN_FLASH1_ADDRESS      0x1960
#define MAX_FLASH1_ADDRESS      0xFFFF   /* NOTE: EraseFlash() assumes this address, only works with S08s with 64K memory space */
#define IMAGE_ADDR              ((uint_32_ptr)0x1960)
#define USER_ENTRY_ADDRESS      0xABFD
#define ERASE_SECTOR_SIZE       0x200
#define FIRMWARE_SIZE_ADD       (VectorAddressTableAddress - 0x10)
/* Redirected Interrupt Vector Table */
#define FLASH_PROTECTED_ADDRESS 0xAC00
#define InterruptVectorsNum     30     /* Number of interrupt vectors to relocate */
#define VectorAddressTableAddress (FLASH_PROTECTED_ADDRESS - 3*(InterruptVectorsNum))
#define BootVectorTableAddress  0xFFC4   /* Lowest interrupt vector address to relocate */
#else
#error "Define MCU settings for the DFU bootloader"
#endif

#ifdef _MC9S08_H
#define S19_RECORD_HEADER       (uint_16)0x5330
#else
#define S19_RECORD_HEADER       (uint_32)0x53300000 //0x53303033
#endif
/* DES space less than this is protected */
#ifndef FLASH_PROTECTED_ADDRESS
#define FLASH_PROTECTED_ADDRESS (int)&IMAGE_ADDR[0]
#endif

#define First4Bytes             4

/* File types */
#define UNKNOWN                 0
#define RAW_BINARY              1
#define CODE_WARRIOR_BINARY     2
#define S19_RECORD              3

void _Entry(void) ;
extern long __SP_INIT;

/* Bootloader Status */
#define BootloaderReady         0
#define BootloaderS19Error      1
#define BootloaderFlashError    2
#define BootloaderSuccess       3
#define BootloaderStarted       4
/* running modes */
#define BOOT_MODE               0
#define APP_MODE                1

#define  FLASH_IMAGE_SUCCESS    0
#define  FLASH_IMAGE_ERROR      1

uint_8           FlashApplication(uint_8* arr,uint_32 length);
void             Loader_init     (void);
extern           int _startup(void);
#endif
/* EOF */
