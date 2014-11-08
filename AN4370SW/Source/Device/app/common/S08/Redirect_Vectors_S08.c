/******************************************************************************
*                                                  
*  (c) copyright Freescale Semiconductor 2008
*  ALL RIGHTS RESERVED
*
*  File Name: Redirect_Vectors_S08.C
*                                                                          
*  Purpose: This file redirects the interrupt vectors from protected flash in 
*           the bootloader, to unprotected flash in the application
*                                                                          
*  Assembler:  Codewarrior for Microcontrollers V6.2
*                                            
*  Version:  1.0
*                                                                          
*                                                                          
*  Author: Derek Snell                             
*                                                                                       
*  Location: Indianapolis, IN. USA                                            
*                                                                                  
* UPDATED HISTORY:
*
* REV   YYYY.MM.DD  AUTHOR        DESCRIPTION OF CHANGE
* ---   ----------  ------        --------------------- 
* 1.0   2008.06.10  Derek Snell   Initial version
* 
*
******************************************************************************/                                                                        
/* Freescale  is  not  obligated  to  provide  any  support, upgrades or new */
/* releases  of  the Software. Freescale may make changes to the Software at */
/* any time, without any obligation to notify or provide updated versions of */
/* the  Software  to you. Freescale expressly disclaims any warranty for the */
/* Software.  The  Software is provided as is, without warranty of any kind, */
/* either  express  or  implied,  including, without limitation, the implied */
/* warranties  of  merchantability,  fitness  for  a  particular purpose, or */
/* non-infringement.  You  assume  the entire risk arising out of the use or */
/* performance of the Software, or any systems you design using the software */
/* (if  any).  Nothing  may  be construed as a warranty or representation by */
/* Freescale  that  the  Software  or  any derivative work developed with or */
/* incorporating  the  Software  will  be  free  from  infringement  of  the */
/* intellectual property rights of third parties. In no event will Freescale */
/* be  liable,  whether in contract, tort, or otherwise, for any incidental, */
/* special,  indirect, consequential or punitive damages, including, but not */
/* limited  to,  damages  for  any loss of use, loss of time, inconvenience, */
/* commercial loss, or lost profits, savings, or revenues to the full extent */
/* such  may be disclaimed by law. The Software is not fault tolerant and is */
/* not  designed,  manufactured  or  intended by Freescale for incorporation */
/* into  products intended for use or resale in on-line control equipment in */
/* hazardous, dangerous to life or potentially life-threatening environments */
/* requiring  fail-safe  performance,  such  as  in the operation of nuclear */
/* facilities,  aircraft  navigation  or  communication systems, air traffic */
/* control,  direct  life  support machines or weapons systems, in which the */
/* failure  of  products  could  lead  directly to death, personal injury or */
/* severe  physical  or  environmental  damage  (High  Risk Activities). You */
/* specifically  represent and warrant that you will not use the Software or */
/* any  derivative  work of the Software for High Risk Activities.           */
/* Freescale  and the Freescale logos are registered trademarks of Freescale */
/* Semiconductor Inc.                                                        */ 
/*****************************************************************************/

#include "Bootloader.h"

 typedef void (* Addr)(void);
 
void Dummy_ISR(void) {
  
}

extern   void  Timer_ISR(void);
extern   void  USB_ISR(void);

/* application interrrupt vector table */

volatile const Addr AppISRTable[InterruptVectorsNum]@ VectorAddressTableAddress;
/* bootloader interrupt vector table */
volatile const Addr BootISRTable[InterruptVectorsNum] = { 
    Dummy_ISR,                 //  0 - Reset
    Dummy_ISR,                 //  1 - SWI
    Dummy_ISR,                 //  2 - IRQ
    Dummy_ISR,                 //  3 - Low Voltage Detect
    Dummy_ISR,                 //  4 - MCG Loss of Lock
    Dummy_ISR,                 //  5 - SPI1
    Dummy_ISR,                 //  6 - SPI2
    USB_ISR,                   //  7 - USB Status
    Dummy_ISR,                 //  8 - Reserved
    Dummy_ISR,                 //  9 - TPM1 Channel0
    Dummy_ISR,                 //  10 - TPM1 Channel1
    Dummy_ISR,                 //  11 - TPM1 Channel2
    Dummy_ISR,                 //  12 - TPM1 Channel3
    Dummy_ISR,                 //  13 - TPM1 Channel4
    Dummy_ISR,                 //  14 - TPM1 Channel5
    Dummy_ISR,                 //  15 - TPM1 Overflow
    Dummy_ISR,                 //  16 - TPM2 Channel0
    Dummy_ISR,                 //  17 - TPM2 Channel1
    Dummy_ISR,                 //  18 - TPM2 Overflow
    Dummy_ISR,                 //  19 - TPM1 SCI1 Error
    Dummy_ISR,                 //  20 - TPM1 SCI1 Receive
    Dummy_ISR,                 //  21 - TPM1 SCI1 Transmit
    Dummy_ISR,                 //  22 - TPM1 SCI2 Error
    Dummy_ISR,                 //  23 - TPM1 SCI2 Receive
    Dummy_ISR,                 //  24 - TPM1 SCI2 Transmit
    Dummy_ISR,                 //  25 - TPM1 KBI
    Dummy_ISR,                 //  26 - TPM1 ADC Conversion
    Dummy_ISR,                 //  27 - TPM1 ACMP
    Dummy_ISR,                 //  28 - IIC
    Timer_ISR,                 //  29 - RTC
};

 /* application interrrupt vector table */
 extern uint_8 boot_mode;
/******************************************************************************
 *
 *   @name        vector1
 *
 *   @brief       Interrtupt service routine of vector 1 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vswi void vector1(void)
 {
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vswi]();
    }
    else
    {
         AppISRTable[VectorNumber_Vswi]();
    }
 }
/******************************************************************************
 *
 *   @name        vector2
 *
 *   @brief       Interrtupt service routine of vector 2 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Virq void vector2(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Virq]();
    }
    else
    {
         AppISRTable[VectorNumber_Virq]();
    }
}
/******************************************************************************
 *
 *   @name        vector3
 *
 *   @brief       Interrtupt service routine of vector 3 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vlvd void Vector3(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vlvd]();
    }
    else
    {
         AppISRTable[VectorNumber_Vlvd]();
    }
}
/******************************************************************************
 *
 *   @name        vector4
 *
 *   @brief       Interrtupt service routine of vector 4 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vlol void Vector4(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vlol]();
    }
    else
    {
         AppISRTable[VectorNumber_Vlol]();
    }
}
/******************************************************************************
 *
 *   @name        vector5
 *
 *   @brief       Interrtupt service routine of vector 5 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vspi1 void Vector5(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vspi1]();
    }
    else
    {
         AppISRTable[VectorNumber_Vspi1]();
    }
}

/******************************************************************************
 *
 *   @name        vector6
 *
 *   @brief       Interrtupt service routine of vector 6 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vspi2 void Vector6(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vspi2]();
    }
    else
    {
         AppISRTable[VectorNumber_Vspi2]();
    }
}

/******************************************************************************
 *
 *   @name        vector7
 *
 *   @brief       Interrtupt service routine of vector 7 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vusb void Vector7(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vusb]();
    }
    else
    {
         AppISRTable[VectorNumber_Vusb]();
    }
}

/******************************************************************************
 *
 *   @name        vector8
 *
 *   @brief       Interrtupt service routine of vector 8 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_VReserved void Vector8(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_VReserved]();
    }
    else
    {
         AppISRTable[VectorNumber_VReserved]();
    }
}

/******************************************************************************
 *
 *   @name        vector9
 *
 *   @brief       Interrtupt service routine of vector 9 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vtpm1ch0 void Vector9(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vtpm1ch0]();
    }
    else
    {
         AppISRTable[VectorNumber_Vtpm1ch0]();
    }
}

/******************************************************************************
 *
 *   @name        vector10
 *
 *   @brief       Interrtupt service routine of vector 10 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vtpm1ch1 void Vector10(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vtpm1ch1]();
    }
    else
    {
         AppISRTable[VectorNumber_Vtpm1ch1]();
    }
}

/******************************************************************************
 *
 *   @name        vector11
 *
 *   @brief       Interrtupt service routine of vector 11 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vtpm1ch2 void Vector11(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vtpm1ch2]();
    }
    else
    {
         AppISRTable[VectorNumber_Vtpm1ch2]();
    }
}

/******************************************************************************
 *
 *   @name        vector12
 *
 *   @brief       Interrtupt service routine of vector 12 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vtpm1ch3 void Vector12(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vtpm1ch3]();
    }
    else
    {
         AppISRTable[VectorNumber_Vtpm1ch3]();
    }
}

/******************************************************************************
 *
 *   @name        vector13
 *
 *   @brief       Interrtupt service routine of vector 13 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vtpm1ch4 void Vector13(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vtpm1ch4]();
    }
    else
    {
         AppISRTable[VectorNumber_Vtpm1ch4]();
    }
}

/******************************************************************************
 *
 *   @name        vector14
 *
 *   @brief       Interrtupt service routine of vector 14 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vtpm1ch5 void Vector14(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vtpm1ch5]();
    }
    else
    {
         AppISRTable[VectorNumber_Vtpm1ch5]();
    }
}

/******************************************************************************
 *
 *   @name        vector15
 *
 *   @brief       Interrtupt service routine of vector 15 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vtpm1ovf void Vector15(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vtpm1ovf]();
    }
    else
    {
         AppISRTable[VectorNumber_Vtpm1ovf]();
    }
}

/******************************************************************************
 *
 *   @name        vector16
 *
 *   @brief       Interrtupt service routine of vector 16 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vtpm2ch0 void Vector16(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vtpm2ch0]();
    }
    else
    {
         AppISRTable[VectorNumber_Vtpm2ch0]();
    }
}

/******************************************************************************
 *
 *   @name        vector17
 *
 *   @brief       Interrtupt service routine of vector 17 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vtpm2ch1 void Vector17(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vtpm2ch1]();
    }
    else
    {
         AppISRTable[VectorNumber_Vtpm2ch1]();
    }
}

/******************************************************************************
 *
 *   @name        vector18
 *
 *   @brief       Interrtupt service routine of vector 18 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vtpm2ovf void Vector18(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vtpm2ovf]();
    }
    else
    {
         AppISRTable[VectorNumber_Vtpm2ovf]();
    }
}
/******************************************************************************
 *
 *   @name        vector19
 *
 *   @brief       Interrtupt service routine of vector 19 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vsci1err void Vector19(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vsci1err]();
    }
    else
    {
         AppISRTable[VectorNumber_Vsci1err]();
    }
}

/******************************************************************************
 *
 *   @name        vector20
 *
 *   @brief       Interrtupt service routine of vector 20 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vsci1rx void Vector20(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vsci1rx]();
    }
    else
    {
         AppISRTable[VectorNumber_Vsci1rx]();
    }
}

/******************************************************************************
 *
 *   @name        vector21
 *
 *   @brief       Interrtupt service routine of vector 21 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vsci1tx void Vector21(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vsci1tx]();
    }
    else
    {
         AppISRTable[VectorNumber_Vsci1tx]();
    }
}

/******************************************************************************
 *
 *   @name        vector22
 *
 *   @brief       Interrtupt service routine of vector 22 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vsci2err void Vector22(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vsci2err]();
    }
    else
    {
         AppISRTable[VectorNumber_Vsci2err]();
    }
}

/******************************************************************************
 *
 *   @name        vector23
 *
 *   @brief       Interrtupt service routine of vector 23 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vsci2rx void Vector23(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vsci2rx]();
    }
    else
    {
         AppISRTable[VectorNumber_Vsci2rx]();
    }
}

/******************************************************************************
 *
 *   @name        vector24
 *
 *   @brief       Interrtupt service routine of vector 24 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vsci2tx void Vector24(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vsci2tx]();
    }
    else
    {
         AppISRTable[VectorNumber_Vsci2tx]();
    }
}

/******************************************************************************
 *
 *   @name        vector25
 *
 *   @brief       Interrtupt service routine of vector 25 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vkeyboard void Vector25(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vkeyboard]();
    }
    else
    {
         AppISRTable[VectorNumber_Vkeyboard]();
    }
}

/******************************************************************************
 *
 *   @name        vector26
 *
 *   @brief       Interrtupt service routine of vector 26 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vadc void Vector26(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vadc]();
    }
    else
    {
         AppISRTable[VectorNumber_Vadc]();
    }
}

/******************************************************************************
 *
 *   @name        vector27
 *
 *   @brief       Interrtupt service routine of vector 27 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vacmp void Vector27(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vacmp]();
    }
    else
    {
         AppISRTable[VectorNumber_Vacmp]();
    }
}

/******************************************************************************
 *
 *   @name        vector28
 *
 *   @brief       Interrtupt service routine of vector 28 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Viic void Vector28(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Viic]();
    }
    else
    {
         AppISRTable[VectorNumber_Viic]();
    }
}

/******************************************************************************
 *
 *   @name        vector29
 *
 *   @brief       Interrtupt service routine of vector 29 which is used to 
 *                redirect vector table between bootloader and application.
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************/
interrupt VectorNumber_Vrtc void Vector29(void)
{
    if(boot_mode == BOOT_MODE)
    {
         BootISRTable[VectorNumber_Vrtc]();
    }
    else
    {
         AppISRTable[VectorNumber_Vrtc]();
    }
}
