/**HEADER********************************************************************
*
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2004-2008 Embedded Access Inc.;
* All Rights Reserved
*
***************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************
*
* $FileName: Loader$
* $Version : 1.0.0.0$
* $Date    : apr-20-2011$
*
* Comments:
*
*   This file is the main file for usb loader demo.
*
*END************************************************************************/
#include <string.h>
#include <stdlib.h>         /* ANSI memory controls */
#include <stdio.h>
#include "types.h"          /* User Defined Data Types */
#include "Bootloader.h"
#include "Boot_loader_task.h"
#include "derivative.h"
#if (defined __MCF52xxx_H__)
#include "flash_cfv2.h"
#elif defined(_MCF51_H)
#include "flash.h"
#elif ((defined __MK_xxx_H__)||(defined _MCF51_PLUS_H))
#include "flash_FTFL.h"
#elif (defined _MC9S08_H)
#include "flash_hcs.h"
#endif
#include "hidef.h"

/*****************************************************************************
 * Function predefinitions.
 *****************************************************************************/
 uint_8           FlashApplication(uint_8* arr,uint_32 length);
        void      Loader_init     (void);
 static uint_8    FlashArrayS19   (uint_8* Array,uint_32 size_of_array,uint_8 *Line);
 static uint_8    FlashLineS19    (uint_8 *Line);
 static uint_8    FlashArrayCW    (uint_8 *Array,uint_32 size_of_array,uint_8 *Line);
 static uint_8    FlashLineCW     (uint_8 *Line);
 static uint_8    GetHexValue     (uint_8 text);
 static uint_8    GetSpair        (uint_8 *arr,uint_8 point);
 static uint_8    CheckAddressValid(uint_32 Address);
 static uint_32   get_uint32      (uint_8* arr, uint_32 index);
 /*****************************************************************************
 * Global variables.
 *****************************************************************************/

uint_8 S19FileDone;         /* parsing S19 flag */
uint_32 S19Address;         /* address to flash */
uint_8 BootloaderStatus;    /* status of loading process */
uint_8 filetype;            /* type of image file */
uint_8 line[260];           /* line buffer */
uint_32 SizeOfFirmware;     /* firmware size */
uint_32 curL,totalL;             /* current Length , taltal Length */
uint_8 newline;
/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : Loader_init
* Returned Value : none
* Comments       : init loader variable
*
*END*--------------------------------------------------------------------*/
void
Loader_init
    (
       void
    )
{
    S19FileDone = 0;
    S19Address = 0;
    BootloaderStatus = 0;
    filetype = 0;
    SizeOfFirmware = 0;
    curL = 0;
    totalL = 6;
    newline = 0;
}
/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : FlashApplication
* Returned Value : 0 if successful, other value if error
* Comments       : parse and flash an array to flash memory
*
*END*--------------------------------------------------------------------*/
uint_8
FlashApplication
    (
        /* [IN] the array to parse */
        uint_8*     arr,
        /* [IN] data length of the array */
        uint_32     length
    )
{
    /* Body */
    uint_8 result;
    uint_32 write_addr;             /* address to write to flash */
    static  uint_32 bytes_written;  /* number of bytes was written to flash */
    #ifdef _MC9S08_H
    uint_16 header;
    #else
    uint_32 header;
    #endif
    #ifdef _MC9S08_H
    header = (uint_16)*(uint_16*)(arr);
    #else
    header = get_uint32(arr,0);
    #endif

    /* Get image type */
    if(filetype == UNKNOWN)
    {
        bytes_written = 0;
        SizeOfFirmware = 0;
        /*  first four bytes is SP */
        if( (MIN_RAM1_ADDRESS <=header ) && (header<= MAX_RAM1_ADDRESS))
        {
#ifndef NO_PRINTF_SUPPORT
            printf("\nRaw binary file found\n\r");
            printf("\nFLASHING....\n\rPlease do not remove your device\n\r");
#endif
            filetype = RAW_BINARY;
        }
        /*  first four bytes is user application address */
        if( (MIN_FLASH1_ADDRESS <=header ) && (header<= MAX_FLASH1_ADDRESS))
        {
        #ifdef _MC9S08_H         /* HCS08 don't have code warrior binary file */
            filetype = RAW_BINARY;
        #else
#ifndef NO_PRINTF_SUPPORT
                printf("\n\rCodeWarrior binary file found\n\r");
                printf("\n\rFLASHING....\n\rPlease do not remove your device\n\r");
#endif
            filetype = CODE_WARRIOR_BINARY;
        #endif
        }
        /*  first four bytes is S-Record header */
#ifdef LITTLE_ENDIAN
        header = BYTESWAP32(header);
#endif
#ifdef _MC9S08_H /*FSL: restrection for 8 bit architecture only*/
        if(header ==(uint_32)(S19_RECORD_HEADER))
#else
        if((header&0xFFFF0000) ==(uint_32)(S19_RECORD_HEADER))
#endif
        {
#ifndef NO_PRINTF_SUPPORT
                    printf("\n\rS-Record file found\n\r");
                    printf("\n\rFLASHING....\n\rPlease do not remove your device\n\r");
#endif
            filetype = S19_RECORD;
        }
        if(filetype ==  UNKNOWN)
        {
#if (!(defined _MC9S08_H))
#ifndef NO_PRINTF_SUPPORT
            printf("\n\r\t\tERROR......\n\rUnknown file type");
#endif
#endif
            return (uint_8)-1;/*FSL: error*/
        } /* EndIf */
    } /* EndIf */

    /* Flash image */
    switch (filetype)
    {
        case RAW_BINARY:
            /* Raw binary file found*/
            /* the address to write to flash */
            write_addr =(uint_32) IMAGE_ADDR + bytes_written;
            /* if flash error , break the loop */
            DisableInterrupts;
            (void)erase_sectors((uint_32*)(write_addr),length);
            result = Flash_ByteProgram((uint_32*)(write_addr),(uint_32*)arr,length);
            EnableInterrupts;
            bytes_written += length;
            SizeOfFirmware = bytes_written;
            break;
        case CODE_WARRIOR_BINARY:
            /* CodeWarrior binary file found */
            result = FlashArrayCW(arr,length,line);  /* DES parse and flash array */
            break;
        case S19_RECORD:
            /* S19 file found */
            result = FlashArrayS19(arr,length,line);    /* DES parse and flash array */
            break;
    } /* EndSwitch */
    /* DES Should add programming verification code ...minimal code
    to see if "#Flash Programming Signature" present from linker script */
#ifndef NO_PRINTF_SUPPORT
    printf("&"); /* flashing indicator */
#endif
    return result;
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : FlashArrayS19
* Returned Value : 0 if successful, other value if error
* Comments       : Get full lines from an array and flash them
*
*END*--------------------------------------------------------------------*/
static uint_8
FlashArrayS19
    (
        /* [IN] the array to parse */
        uint_8 *Array,
        /* [IN] data length of the array */
        uint_32 size_of_array,
        /* [IN] a allocated buffer */
        uint_8 *Line
    )
{
    /* Body */
    uint_16 i,j;
    uint_8 c,result;
    static uint_8 t=0;

    for (i=0 ; i<size_of_array;i++)
    {
        c = Array[i];
        if(curL == 4)
        {
            /* Get total length of current line */
            t = GetSpair(Line,2);
            totalL = (uint_8)((t+2)*2);
        } /* EndIf */
        if (curL>=totalL)                   /* Get full line */
        {
            result = FlashLineS19(Line);    /* Parse and flash current line */
            for (j=0 ; j<totalL;j++)        /* Reset line and other variables */
            {
                Line[j]=0xFF;
            } /* EndFor */
            curL=0;
            totalL=6;
            newline=0;
        } /* EndIf */
        if (newline==0)                     /* Check for newline */
        {
            if (c=='S')
            {
                newline=1;
                Line[curL]=c;
                curL++;
            } /* EndIf */
        }
        else
        {
            Line[curL] = c;
            curL++;
        } /* EndIf */
    } /* EndFor */
    return result;
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : FlashLineS19
* Returned Value : 0 if successful, other value if error
* Comments       : Parse and flash a full line
*
*END*--------------------------------------------------------------------*/
static uint_8
FlashLineS19
    (
        /* [IN] the Line to parse */
        uint_8 *Line
    )
{
    /* Body */
    static uint_8 length;
    static uint_8 checksum;
    static uint_8 i, offset,temp,c_temp,j;
    static uint_8 type;
    static uint_8 data;
    static uint_8 cur_point; /* current pointer */
    uint_8 buffer_to_flash[256];

    c_temp=Line[0];
    if (c_temp!='S')
    {
        BootloaderStatus = BootloaderS19Error;
        return FLASH_IMAGE_ERROR;
    } /* EndIf */
    /* Get record length */
    cur_point = 2;
    length = GetSpair(Line,cur_point);
    if(S19FileDone)
    {
        /* not a valid S19 file */
        BootloaderStatus = BootloaderS19Error;
        return FLASH_IMAGE_ERROR;
    } /* EndIf */
    cur_point--;
    checksum = length;
    type=Line[1];

     /* Take appropriate action */
     switch (type)
        {
            case '1':
            case '2':
            case '3':
                S19Address = (uint_32) NULL;
                type -= '0';
                cur_point=4;
                for (i = 0; i <= type ; i++)
                {
                    /* Formulate address */
                    /* Address needs to be word aligned for successful flash program */
                    data = GetSpair(Line,cur_point);
                    if(S19FileDone)
                    {
                        /* not a valid S19 file */
                        BootloaderStatus = BootloaderS19Error;
                        return FLASH_IMAGE_ERROR;
                    } /* EndIf */
                    S19Address = (S19Address << 8) | data;
                    /* Maintain 8-bit checksum */
                    checksum = (unsigned char)((data + checksum) & 0x00FF);
                    cur_point+=2;
                } /* EndFor */

                if (CheckAddressValid(S19Address))
                {
                    /* 32-bit cores program flash in 32-bit words */
                    /* Therefore S19 address needs to be adjusted to be word aligned */
                    /* Pad beginning of buffer if address not word aligned */
                    #ifndef _MC9S08_H
                    offset = (uint_8) (S19Address & 0x0003);
                    S19Address = (uint_32) (S19Address & 0xFFFFFFFC);
                    length += offset;
                    for (i = 0; i < offset; i++)
                    {
                        buffer_to_flash[i] = 0xFF;
                    } /* EndFor */
                    #endif

                    /* Get data and put into buffer */
                    for (i = offset; i < (length - (type+2)); i++)
                    {
                        data=GetSpair(Line,cur_point);
                        buffer_to_flash[i] =data;
                        cur_point+=2;
                        if(S19FileDone)
                        {
                            /* not a valid S19 file */
                            BootloaderStatus = BootloaderS19Error;
                            return FLASH_IMAGE_ERROR;
                        } /* EndIf */
                    } /* EndFor */

                    /* Calculate checksum */
                    for (i = offset; i < (length - (type+2)); i++)
                    {
                        checksum = (unsigned char)((buffer_to_flash[i] + checksum) & 0x00FF);
                    } /* EndFor */
                    /* Get checksum byte */
                    data = GetSpair(Line,cur_point);
                    cur_point+=2;
                    if(S19FileDone)
                    {
                        /* not a valid S19 file */
                        BootloaderStatus = BootloaderS19Error;
                        return FLASH_IMAGE_ERROR;
                    } /* EndIf */

                    if (((data - ~checksum) & 0x00FF) != 0)
                    {
                        BootloaderStatus = BootloaderS19Error;
                        S19FileDone = TRUE;
                        return FLASH_IMAGE_ERROR;
                    } /* EndIf */

                    #ifndef _MC9S08_H
                    /* For 32-bit cores Flash_Prog writes 32-bit words, not bytes */
                    /* if last 32-bit word in s-record is not complete, finish word */
                    if((i & 0x0003) != 0x0000)
                    {
                        /* 32-bit word not complete */
                        buffer_to_flash[i++] = 0xFF;         /* pad end of word */
                        buffer_to_flash[i++] = 0xFF;         /* pad end of word */
                        buffer_to_flash[i++] = 0xFF;         /* pad end of word */
                    } /* EndIf */
                    /* NOTE: 8-bit core does not need to pad the end */
                    #endif

                    /* Write buffered data to Flash */
#ifdef _MC9S08_H
                    if((S19Address <= (uint_32)FLASH_PROTECTED_ADDRESS) && (S19Address >= MIN_FLASH1_ADDRESS))
#else
                    if((S19Address >= (uint_32)FLASH_PROTECTED_ADDRESS) && (S19Address <= MAX_FLASH1_ADDRESS))
#endif
                    {
                        /* call flash program */
                        DisableInterrupts;
                        (void)erase_sectors((uint_32*)S19Address,i);
                        temp =  Flash_ByteProgram((uint_32*)S19Address,(uint_32*)buffer_to_flash,i);
                        SizeOfFirmware = S19Address - (uint_32) IMAGE_ADDR +i;
                        EnableInterrupts;

                        if(Flash_OK  != temp)
                        {
                            BootloaderStatus = BootloaderFlashError;
                            return FLASH_IMAGE_ERROR;
                        } /* EndIf */
                    } /* EndIf */
                }
                else    /* S-Record points to invalid address */
                {
                    BootloaderStatus = BootloaderS19Error;
                    return FLASH_IMAGE_ERROR;
                } /* EndIf */
                break;
            case '7':
            case '8':
            case '9':
                S19Address = (uint_32) NULL;
                type = (unsigned char)(type - '0');
                type = (unsigned char)(10 - type);
                cur_point=4;
                checksum = length;
                /* Get Address */
                for (i = 0; i <= type ; i++)
                {
                    for(j=0;j<length-1;j++)
                    {
                        data = GetSpair(Line,cur_point);
                        if(S19FileDone)
                        {
                            /* not a valid S19 file */
                            BootloaderStatus = BootloaderS19Error;
                            return FLASH_IMAGE_ERROR;
                        } /* EndIf */
                        checksum = (unsigned char)((data + checksum) & 0x00FF);
                        cur_point+=2;
                    } /* EndFor */

                    /* Read checksum value */
                    data=GetSpair(Line,cur_point);
                    if(S19FileDone)
                    {
                        /* not a valid S19 file */
                        BootloaderStatus = BootloaderS19Error;
                        return FLASH_IMAGE_ERROR;
                    } /* EndIf */

                    /* Check checksum */
                    if (((data - ~checksum) & 0x00FF) != 0)
                    {
                        BootloaderStatus = BootloaderS19Error;
                        S19FileDone = TRUE;
                        return FLASH_IMAGE_ERROR;
                    }
                    else
                    {
                        /* File completely read successfully */
                        BootloaderStatus = BootloaderSuccess;
                        S19FileDone = TRUE;
                        return FLASH_IMAGE_SUCCESS;
                    } /* EndIf */
                } /* EndFor */
                break;
            case '0':
            case '4':
            case '5':
            case '6':
            default:
                break;
        } /* EndSwitch */
    return FLASH_IMAGE_SUCCESS;
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : GetHexValue
* Returned Value : unsigned char, hex value of character
* Comments       : Converts ASCII character to hex value
*
*END*--------------------------------------------------------------------*/
static uint_8
GetHexValue
    (
        /* [IN] the text to parse */
        uint_8 text
    )
{
    /* Body */
    switch (text)
    {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return (uint_8)(text - '0');
        case 'A':
        case 'a':
            return 10;
        case 'B':
        case 'b':
            return 11;
        case 'C':
        case 'c':
            return 12;
        case 'D':
        case 'd':
            return 13;
        case 'E':
        case 'e':
            return 14;
        case 'F':
        case 'f':
            return 15;
        default:
            return 0xFF;
    } /* EndSwitch */
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : GetSpair
* Returned Value : unsigned char, converted hex byte
* Comments       : Gets pair of characters, and converts to hex byte
*
*END*--------------------------------------------------------------------*/
static uint_8
GetSpair
    (
        /* [IN] the array to parse */
        uint_8 *arr,
        /* [IN] point to array data */
        uint_8 point
    )
{
    /* Body */
    uint_8 ch;
    uint_8 upper,lower;
    ch = arr[point];
    upper = (uint_8)(GetHexValue(ch));
    if(upper == 0xFF)
    {
        /* Not a proper S19 file */
        S19FileDone = TRUE;
    } /* EndIf */
    upper = (uint_8)(upper << 4);
    ch=arr[point+1];
    lower= (uint_8)(GetHexValue(ch));
    if(lower == 0xFF)
    {
        /* Not a proper S19 file */
        S19FileDone = TRUE;
    } /* EndIf */
    return (uint_8)(upper | lower);
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : CheckAddressValid
* Returned Value : unsigned char, TRUE or FALSE if valid
* Comments       : Checks if Address of S-Record is valid for device
*
*END*--------------------------------------------------------------------*/
static uint_8
CheckAddressValid
    (
        /* [IN] the address to check */
        uint_32 Address
    )
{
    /* Body */
    if((Address >= MIN_FLASH1_ADDRESS) && (Address <= MAX_FLASH1_ADDRESS))
        return TRUE;
    else if ((Address >= MIN_RAM1_ADDRESS) && (Address <= MAX_RAM1_ADDRESS))
        return TRUE;
    else
        return FALSE;
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : FlashArrayCW
* Returned Value : 0 if successful, other value if error
* Comments       : Get full lines from an array and flash them
*
*END*--------------------------------------------------------------------*/

static uint_8
FlashArrayCW
    (
        /* [IN] the array to parse */
        uint_8 *Array,
        /* [IN] data length of the array */
        uint_32 size_of_array,
        /* [IN] a allocated buffer */
        uint_8 *Line
    )
{
    /* Body */
    uint_8 result;
    uint_16 i,j;
    uint_32 data_length;
    uint_8 c;

    for (i = 0 ; i<size_of_array;i++)
    {
        c = Array[i];
        Line[curL] = c;
        curL++;
        if(curL == 8)
        {
            data_length = get_uint32(Line,4);
            /* Get total length of current line */
            totalL = data_length + 8;
            /* start a new line with address and data length */
            newline = 1;
        } /* EndIf */
        if(newline)
        {
            if (curL>=totalL)                   /* Get full line */
            {
                /* Parse and flash current line */
                result = FlashLineCW(Line);
                /* Reset line and other variables */
                for (j=0 ; j<totalL;j++)
                {
                    Line[j]=0xFF;
                } /* EndFor */
                curL=0;
                totalL=0;
                newline=0;
            } /* EndIf */
        } /* EndIf */
    } /* EndFor */
    return result;
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : FlashLineCW
* Returned Value : 0 if successful, other value if error
* Comments       : Parse and flash a full line
*
*END*--------------------------------------------------------------------*/
static uint_8
FlashLineCW
    (
        /* [IN] the Line to parse */
        uint_8 *Line
    )
{
    /* Body */
    uint_8  result = FLASH_IMAGE_SUCCESS;
    uint_32 write_addr;
    uint_32 data_length;
    write_addr = get_uint32(Line,0);    /* address to flash */
    data_length = get_uint32(Line,4);   /* length of data */
#ifdef _MC9S08_H
    if((write_addr >= (uint_32)MIN_FLASH1_ADDRESS) && (write_addr <= FLASH_PROTECTED_ADDRESS))
#else
    if((write_addr >= (uint_32)FLASH_PROTECTED_ADDRESS) && (write_addr <= MAX_FLASH1_ADDRESS))
#endif
    {
        DisableInterrupts;
        (void)erase_sectors((uint_32*)(write_addr),data_length);
        if (Flash_ByteProgram((uint_32*)(write_addr),(uint_32 *)(Line + 8),data_length))
            result = FLASH_IMAGE_ERROR;
        SizeOfFirmware = write_addr - (uint_32) IMAGE_ADDR + data_length;
        EnableInterrupts;
    } /* EndIf */
    return result;
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : get_uint32
* Returned Value : result
* Comments       : get a unsign long number from an array
*
*END*--------------------------------------------------------------------*/
static uint_32
get_uint32
    (
        /* [IN] the array */
        uint_8* arr,
        /* [IN] the index of array */
        uint_32 index
    )
{
    uint_32 result;
    result = *(uint_32*)(arr + index);
    return result ;
} /* EndBody */

/* EOF */
