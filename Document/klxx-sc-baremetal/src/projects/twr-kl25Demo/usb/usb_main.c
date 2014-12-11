/*
 * File:		usb_main.c
 * Purpose:		Main process
 *
 */

/* Includes */
#include "common.h"
#include "usb_cdc.h"
#include "usb_reg.h"


/* Testing Suite */
#define USB_DEVICE_XCVR_TEST
//#define USBREG_TEST
//#define USB_CDC_DCD

uint8 gu8USB_Buffer[64];

extern uint8 gu8USB_Flags; 
extern uint8 gu8EP3_OUT_ODD_Buffer[];
extern tBDT tBDTtable[];



char str1[] = "hola";


void Debug_Init(void);
/********************************************************************/
int main (void)
{
    uint8 u8ChargerResult = 0;

    #ifdef USB_DEVICE_XCVR_TEST
      printf("\n********** USB XCVR Module Testing **********\n");     
      USB_REG_SET_ENABLE;
      CDC_Init();
      // Pull up enable
      FLAG_SET(USB_CONTROL_DPPULLUPNONOTG_SHIFT,USB0_CONTROL);
    #endif
        
    #ifdef USBREG_TEST
      printf("\n********** USB Voltage Regulator Testing **********\n");
      USBReg_Testing();
    #endif
    
      for(;;)
    {    
        // Main Loop for USB Host Mode
        #ifdef USB_HOST_XCVR_TEST
          //CDC_Engine();
        #endif
        
        // Main Loop for USB Device Mode
        #ifdef USB_DEVICE_XCVR_TEST 
        // USB CDC service routine
          CDC_Engine();

        // If data transfer arrives
        if(FLAG_CHK(EP_OUT,gu8USB_Flags))
        {
            (void)USB_EP_OUT_SizeCheck(EP_OUT);         
            usbEP_Reset(EP_OUT);
            usbSIE_CONTROL(EP_OUT);
            FLAG_CLR(EP_OUT,gu8USB_Flags);

            // Send it back to the PC
          //  EP_IN_Transfer(EP2,CDC_OUTPointer,1);
            
            EP_IN_Transfer(EP2,&str1[0],sizeof(str1));
            

        }        
        #endif        
    } 
}

/*******************************************************************/

