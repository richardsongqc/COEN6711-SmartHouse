/******************************************************************************
 *
 * Freescale Semiconductor Inc.
 * (c) Copyright 2004-2010 Freescale Semiconductor, Inc.
 * ALL RIGHTS RESERVED.
 *
 **************************************************************************//*!
 *
 * @file dfu_mouse.c
 *
 * @author
 *
 * @version
 *
 * @date
 *
 * @brief  The file emulates a mouse with buttons
 *         4 buttons are used on the demo board for the emulation
 *         PTG0--------- for left click(For JS16 PTG0 is inactive)
 *         PTG1--------- for right click
 *         PTG2--------- if macro "UP_LEFT" is defined mouse moves "left"
 *                       else mouse moves "right"
 *         PTG3--------- if macro "UP_LEFT" is defined mouse moves "up"
 *                       else mouse moves "down"
 *
 *   NOTE: key pressed  == LOGIC 0
 *         key released == LOGIC 1
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <stdlib.h>
#include <string.h>
/* skip the inclusion in dependency state */
#ifndef __NO_SETJMP
#include <stdio.h>
#endif
#include "hidef.h"            /* for EnableInterrupts macro */
#include "derivative.h"       /* include peripheral declarations */
#include "types.h"            /* User Defined Data Types */
#include "usb_hid.h"          /* USB HID Class Header File */
#include "dfu_mouse.h"        /* Mouse Application Header File */
#include "usb_dfu.h"          /* USB DFU Class Header File */
#include "RealTimerCounter.h" /* RTC header file */
#include "rtc.h"
#if defined (_MCF51_H)
#include "exceptions.h"
#endif
#if (defined __MCF52xxx_H__)
#include "flash_cfv2.h"
#elif defined(_MCF51_H)
#include "flash.h"
#elif ((defined __MK_xxx_H__)||(defined _MCF51_PLUS_H))
#include "flash_FTFL.h"
#elif (defined _MC9S08_H)
#include "flash_hcs.h"
#endif
#include "Bootloader.h"
#if (!(defined   _MC9S08_H))
#include "sci.h"
#endif
#include "Boot_loader_task.h"
/*****************************************************************************
 * Constant and Macro's - None
 *****************************************************************************/

/*****************************************************************************
 * Global Functions Prototypes
 *****************************************************************************/
void TestApp_Init(void);
void TestApp_Task(void);
/****************************************************************************
 * Global Variables - None
 ****************************************************************************/
uint_8 usb_device_state;  /* Device state */
boolean Flag_Manifest;    /* Flag to check manifest phase complete or not */
TIMER_OBJECT dfu_timeout;
uint_8 dfu_flash_result = FLASH_IMAGE_SUCCESS;
/*****************************************************************************
 * Local Types - None
 *****************************************************************************/

/*****************************************************************************
 * Local Functions Prototypes
 *****************************************************************************/
static void USB_App_HID_Callback
(
    uint_8 controller_ID,
    uint_8 event_type,
    void* val
);
static void USB_App_DFU_Callback
(
    uint_8 controller_ID,
    uint_8 event_type,
    void* val
);
static uint_8 USB_App_Param_Callback
(
    uint_8 request,
    uint_16 value,
    uint_8_ptr* data,
    USB_PACKET_SIZE* size
);
static void USB_App_Process_Detach_TimeOut(void);

/*****************************************************************************
 * Local Variables
 *****************************************************************************/
static boolean mouse_init=FALSE;            /* Application Init Flag */
static uint_8 rpt_buf[MOUSE_BUFF_SIZE];     /* Mouse Event Report Buffer
                                               Key press */
static uint_8 null_buf[MOUSE_BUFF_SIZE];    /* Mouse Event Report Buffer
                                               Key Release */
static uint_8 g_app_request_params[2];      /* for get/set idle and protocol
                                               requests*/
/*****************************************************************************
 * Local Functions
 *****************************************************************************/

/******************************************************************************
 *
 *    @name        USB_App_HID_Callback
 *
 *    @brief       This function handles the callback
 *
 *    @param       controller_ID    : Controller ID
 *    @param       event_type       : value of the event
 *    @param       val              : gives the configuration value
 *
 *    @return      None
 *
 *****************************************************************************
 * This function is called from the class layer whenever reset occurs or enum
 * is complete. after the enum is complete this function sets a variable so
 * that the application can start
 *****************************************************************************/
static void 
USB_App_HID_Callback
(
    uint_8 controller_ID,/* [IN] Controller ID */
    uint_8 event_type,   /* [IN] value of the event*/
    void* val            /* [IN] gives the configuration value*/
)
{
    /* Body */
    UNUSED (controller_ID)
    UNUSED (val)
    if ((event_type == USB_APP_BUS_RESET) || (event_type == USB_APP_CONFIG_CHANGED))
    {
        mouse_init=FALSE;
    }
    else if (event_type == USB_APP_ENUM_COMPLETE)
    {   /* if enumeration is complete set mouse_init so that application can start */
        mouse_init=TRUE;
    } /* EndIf */
    return;
} /* EndBody */

/******************************************************************************
 *
 *    @name        USB_App_Process_Detach_TimeOut
 *
 *    @brief       This function is called to processed detach timeout
 *
 *
 *    @return      None
 *
 *****************************************************************************
 *
 *****************************************************************************/
static void 
USB_App_Process_Detach_TimeOut()
{
    /* Body */
    usb_device_state = USB_Class_DFU_App_Getstate();
    if (usb_device_state == USB_APP_DETACH)
    {
        USB_Class_DFU_App_SetIdle();
        USB_Device_Shutdown(0);
        mode = RUNTIME_MODE;
        (void)USB_Class_HID_Init(CONTROLLER_ID, USB_App_HID_Callback, NULL,
                                   USB_App_Param_Callback);
    } /* EndIf */
} /* EndBody */

/******************************************************************************
 *
 *    @name        USB_App_Param_Callback
 *
 *    @brief       This function handles callbacks for USB HID Class request
 *
 *    @param       request  : request type
 *    @param       value    : give report type and id
 *    @param       data     : pointer to the data
 *    @param       size     : size of the transfer
 *
 *    @return      status
 *                 USB_OK  :  if successful
 *                 else return error
 *
 *****************************************************************************
 * This function is called whenever a HID class request is received. This
 * function handles these class requests
 *****************************************************************************/
static uint_8 
USB_App_Param_Callback
(
    uint_8 request,        /* [IN] request type */
    uint_16 value,         /* [IN] report type and ID */
    uint_8_ptr* data,      /* [OUT] pointer to the data */
    USB_PACKET_SIZE* size  /* [OUT] size of the transfer */
)
{
    uint_8 status = USB_OK;
    uint_8 index = (uint_8)((request - 2) & USB_HID_REQUEST_TYPE_MASK);

    *size = 0;
    /* handle the class request */
    switch (request)
    {
    case USB_HID_GET_REPORT_REQUEST :
        *data = &rpt_buf[0]; /* point to the report to send */
        *size = MOUSE_BUFF_SIZE; /* report size */
        break;

    case USB_HID_SET_REPORT_REQUEST :
        for (index = 0; index < MOUSE_BUFF_SIZE ; index++)
        {   /* copy the report sent by the host */
            rpt_buf[index] = *(*data + index);
        }
        break;

    case USB_HID_GET_IDLE_REQUEST :
        /* point to the current idle rate */
        *data = &g_app_request_params[index];
        *size = REQ_DATA_SIZE;
        break;

    case USB_HID_SET_IDLE_REQUEST :
        /* set the idle rate sent by the host */
        g_app_request_params[index] =(uint_8)((value & MSB_MASK) >>
                                              HIGH_BYTE_SHIFT);
        break;

    case USB_HID_GET_PROTOCOL_REQUEST :
        /* point to the current protocol code
           0 = Boot Protocol
           1 = Report Protocol*/
        *data = &g_app_request_params[index];
        *size = REQ_DATA_SIZE;
        break;

    case USB_HID_SET_PROTOCOL_REQUEST :
        /* set the protocol sent by the host
           0 = Boot Protocol
           1 = Report Protocol*/
        g_app_request_params[index] = (uint_8)(value);
        break;
    default:
        if (request == USB_DFU_DETACH)
        {
#if !(DETACH)
            (void)RemoveTimerQ(0);
            dfu_timeout.msCount = (uint_16)value;
            dfu_timeout.pfnTimerCallback = USB_App_Process_Detach_TimeOut;
            (void)AddTimerQ(&dfu_timeout);
#endif
            USB_Class_DFU_App_SetDetach();
            USB_Device_Shutdown(0);
            mode = DFU_MODE;
            (void)USB_Class_DFU_Init(CONTROLLER_ID, USB_App_DFU_Callback, NULL,
                                       NULL);
        }
        break;
    }
    return status;
}

/******************************************************************************
 *
 *    @name        USB_App_DFU_Callback
 *
 *    @brief       This function handles USB Class callback
 *
 *    @param       controller_ID    : Controller ID
 *    @param       event_type       : value of the event
 *    @param       val              : gives the configuration value
 *
 *    @return      None
 *
 *****************************************************************************
 * This function is called from the class layer whenever reset occurs or enum
 * is complete. After the enum is complete this function sets a variable so
 * that the application can start
 *****************************************************************************/
static void 
USB_App_DFU_Callback
(
    uint_8 controller_ID, /* [IN] Controller ID */
    uint_8 event_type,    /* [IN] value of the event */
    void* val             /* [IN] gives the configuration value */
)
{
    /* Body */
    UNUSED (controller_ID)
    UNUSED (val)
    usb_device_state = USB_Class_DFU_App_Getstate();
    Flag_Manifest = USB_Class_DFU_App_Get_Flag_Manifest();
    if (event_type == USB_APP_BUS_RESET)
    {
        switch (usb_device_state)
        {
         case USB_APP_DETACH:
            USB_Class_DFU_App_Set_Usbreset();
            break;
         default:
            USB_Class_DFU_App_Set_Usbreset();
            if (Flag_Manifest)
            {
                /* Change to Runtime mode when manifest phase completed */
                mode = RUNTIME_MODE;
                (void)USB_Class_HID_Init(CONTROLLER_ID, USB_App_HID_Callback, NULL,
                                           USB_App_Param_Callback);
                (void)USB_Class_DFU_App_Reset_Flag_Manifest();
            } /* EndIf */
            break;
        } /* EndSwitch */
    } 
    else if (event_type == USB_APP_ENUM_COMPLETE)
    {

    } /* EndIf */
    return;
} /* EndBody */

/******************************************************************************
 *
 *   @name        TestApp_Init
 *
 *   @brief       This function is the entry for Keyboard Application
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************
 * This function starts the keyboard application
 *****************************************************************************/
void 
TestApp_Init(void)
{
    /* Body */
    uint_8   error;
    
    EnableInterrupts;
    rpt_buf[3] = 0x00;  /* always zero */
    *((uint_32_ptr)rpt_buf) = 0;
    *((uint_32_ptr)null_buf) = 0;
    /* Initialize the flash */
#if (defined(_MCF51_PLUS_H) || defined(__MK_xxx_H__)) 
    Flash_Init(0);       // no clk related
#elif defined(_MCF51_H)
    Flash_Init(0x4E);    // for bus clock=24MHz, 200KHz
#elif defined(_MC9S08_H)
    Flash_Init(0x49);    // for bus clock=~12MHz, 162KHz
#elif defined(__MCF52xxx_H__)
    Flash_Init(0x50);	// prescaler on, freq = 187.5KHz
#else
#error "Configure MCU correctly"
#endif

#ifndef NO_PRINTF_SUPPORT
    sci_init();
    printf("UART enabled\n\r");
#endif
    
#if defined(_MCF51_H)
    usb_int_dis();
#endif

    DisableInterrupts;
    
    /* Initialize the USB interface */
    error = USB_Class_HID_Init(CONTROLLER_ID, USB_App_HID_Callback, NULL,
                               USB_App_Param_Callback);
    EnableInterrupts;
    if (error != USB_OK)
    {
        /* USB HID Class Initialization Failed */
        return;
    } /* EndIf */

#if defined(_MCF51_H)
    usb_int_en();
#endif
} /* EndBody */

/******************************************************************************
 *
 *   @name        TestApp_Task
 *
 *   @brief       This function implement application task
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************
 * This function implement application task
 *****************************************************************************/
void 
TestApp_Task(void)
{
    /* Body */
    /* call the periodic task function */
    USB_Class_HID_Periodic_Task();

    if (mouse_init) /*check whether enumeration is complete or not */
    {
        /*FSL: empty declaration -- no mouse functionality at all*/
    } /* EndIf */
    USB_Class_DFU_Periodic_Task();
    
    /* check flash progress */
    if(FLASH_IMAGE_ERROR == dfu_flash_result)
    {
#ifndef NO_PRINTF_SUPPORT
            printf("\n\nFlash fail. Re-erase memory ...");
#endif
            dfu_flash_result =  FLASH_IMAGE_SUCCESS;
            USB_Class_DFU_App_SetIdle();
    }
    
} /* EndBody */
/* EOF */
