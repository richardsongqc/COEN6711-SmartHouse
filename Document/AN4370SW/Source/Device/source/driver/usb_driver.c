/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2004-2010 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
******************************************************************************
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
**************************************************************************//*!
*
* @file usb_driver.c
*
* @author
*
* @version
*
* @date
*
* @brief The file contains S08 USB stack device layer implementation.
*
*****************************************************************************/
/******************************************************************************
* Includes
*****************************************************************************/
#include "usb_devapi.h" /* USB Device Layer API Header File */
#include "usb_dciapi.h" /* USB Controller API Header File */
/*****************************************************************************
* Constant and Macro's
*****************************************************************************/
/****************************************************************************
* Global Variables
****************************************************************************/
/* Array of USB Service pointers */
static USB_SERVICE_CALLBACK g_usb_CB[USB_SERVICE_MAX];  
/* Array of USB Component Status */
/* Test mode is the last service */
static uint_16 g_usb_component_status[USB_STATUS_TEST_MODE]; 
/* Array of USB Endpoint Status */
static uint_16 g_usb_ep_status[MAX_SUPPORTED_ENDPOINTS];
/* Current un-initialized non CONTROL Endpoint */
static uint_8 g_EPn=0;     
/* Maximum number of Non CONTROL Endpoint required by upper layer */
static uint_8 g_EPn_max=0; 
/*****************************************************************************
* Local Types - None
*****************************************************************************/
/*****************************************************************************
* Local Functions Prototypes - None
*****************************************************************************/
static void USB_Device_Init_Params(void);
/*****************************************************************************
* Local Variables - None
*****************************************************************************/
extern uint_8 USB_DCI_DeInit(void);/*FSL: new!*/
/*****************************************************************************
* Local Functions 
*****************************************************************************/
#if (defined(_MC9S08MM128_H) || defined(_MC9S08JE128_H))
#pragma CODE_SEG DEFAULT
#endif
/**************************************************************************//*!
*
* @name  USB_Device_Init_Params
*
* @brief The funtion initializes the Device Layer Structures
*
* @param  None
*
* @return None
*
******************************************************************************
* Initializes USB Device Layer Structures
*****************************************************************************/
static void USB_Device_Init_Params(void)
{
    /* Body */
    uint_8 loop_index=0;
    g_EPn= g_EPn_max; /* 1 control endpoint */
    /* 
    Initialize USB_STATUS_DEVICE_STATE, USB_STATUS_INTERFACE,
    USB_STATUS_ADDRESS, USB_STATUS_CURRENT_CONFIG, USB_STATUS_SOF_COUNT
    and USB_STATUS_DEVICE to USB_STATUS_UNKNOWN 
    */
    for(loop_index = 0; loop_index < USB_STATUS_TEST_MODE; loop_index++)
    {
        #ifdef OTG_BUILD
        if(loop_index != (USB_STATUS_OTG-1)) /* Do not initialize the OTG status with 0xFFFF */         
        #endif
        {          
            g_usb_component_status[loop_index] = USB_STATUS_UNKNOWN;
        } /* EndIf */
    } /* EndFor */
    /* Initialize status of All Endpoints to USB_STATUS_DISABLED */
    for(loop_index = 0; loop_index < MAX_SUPPORTED_ENDPOINTS; loop_index++)
    {
        g_usb_ep_status[loop_index] = USB_STATUS_DISABLED;
    } /* EndFor */
} /* EndBody */
/*****************************************************************************
* Global Functions
*****************************************************************************/
/**************************************************************************//*!
*
* @name  USB_Devie_Init
*
* @brief The funtion initializes the Device and Controller layer
*
* @param controller_ID : Controller ID
* @param endpoints     : Endpoint count of the application
*
* @return status
*         USB_OK                              : When Successfull
*         USBERR_INVALID_NUM_OF_ENDPOINTS     : When endpoints > max Supported
******************************************************************************
* This function initializes the Device layer and the Controller layer of the
* S08 USB stack. It initialised the variables used for this layer and then
* calls the controller layer initialize function
*****************************************************************************/
uint_8 USB_Device_Init 
    (
        uint_8    controller_ID, /* [IN] Controller ID */
        uint_8    endpoints      /* [IN] Endpoint count of the application */
    )
{
    /* Body */
    /* validate endpoints param */
    if((endpoints > MAX_SUPPORTED_ENDPOINTS) ||
            (endpoints < MIN_SUPPORTED_ENDPOINTS))
    {
        return USBERR_INVALID_NUM_OF_ENDPOINTS;
    } /* EndIf */
    /*init variables */
    g_EPn_max = (uint_8)(endpoints - 1);
    USB_Device_Init_Params();
    /* Initialize all services to null value */
    Clear_Mem((uint_8_ptr)g_usb_CB, 
    (sizeof(USB_SERVICE_CALLBACK) * USB_SERVICE_MAX), (uint_8)NULL);
    /* Call controller layer initialization function */
    return USB_DCI_Init(controller_ID);
} /* EndBody */
/**************************************************************************//*!
*
* @name  USB_Devie_DeInit
*
* @brief The funtion de-initializes the Device and Controller layer
*
* @param controller_ID : Controller ID
*
* @return status
*         USB_OK                              : When Successfull
*         USBERR_INVALID_NUM_OF_ENDPOINTS     : When endpoints > max Supported
******************************************************************************
* This function de-initializes the Device layer and the Controller layer
*****************************************************************************/
uint_8 USB_Device_DeInit(void)
{
    /* Body */
    g_EPn_max = 0;
    /* Call controller layer de-initialization function */
    return USB_DCI_DeInit();
} /* EndBody */
/**************************************************************************//*!
*
* @name  USB_Device_Init_EndPoint
*
* @brief The funtion initializes the endpoint
*
* @param controller_ID : Controller ID
* @param ep_ptr        : Pointer to endpoint detail structure
* @param flag          : Zero termination flag
*
* @return status
*         USB_OK                              : When Successfull
*         USBERR_EP_INIT_FAILED     : When endpoints > max Supported
******************************************************************************
*
* This function initializes an endpoint the Device layer and the Controller
* layer in the S08 USB stack. It validate whether all endpoints have already
* been initialized or not and then calls the controller layer endpoint
*  initialize function
*
*****************************************************************************/
uint_8 USB_Device_Init_EndPoint 
    (
        uint_8               controller_ID, /* [IN] Controller ID */
        USB_EP_STRUCT_PTR    ep_ptr,        /* [IN] Pointer to endpoint detail
                                                    structure */
        uint_8               flag           /* [IN] Zero termination flag */
    )
{
    /* Body */
    uint_8 status=USB_OK;
    /* check if all endpoint have already been initialised */
    if((g_EPn == 0) && (ep_ptr->ep_num != CONTROL_ENDPOINT))
    {
        return USBERR_EP_INIT_FAILED;
    } /* EndIf */
    /* call controller layer for initiazation */
    status = USB_DCI_Init_EndPoint(controller_ID, ep_ptr, flag);
    /* if endpoint successfully initialised update counter */
    if ((ep_ptr->ep_num != CONTROL_ENDPOINT) && (status == USB_OK))
    {
        g_EPn--;
    } /* EndIf */
    return status;
} /* EndBody */
/**************************************************************************//*!
*
* @name  USB_Device_Deinit_EndPoint
*
* @brief The funtion De-initializes the endpoint
*
* @param controller_ID : Controller ID
* @param ep_ptr        : Pointer to endpoint detail structure
* @param flag          : Zero termination flag
*
* @return status
*         USB_OK                              : When Successfull
*         USBERR_EP_DEINIT_FAILED     : When endpoints > max Supported
******************************************************************************
*
* This function deinitializes an endpoint the Device layer and the Controller
* layer in the S08 USB stack. It validate whether all endpoints have already
* been deinitialized or not and then calls the controller layer endpoint
* deinitialize function
*
*****************************************************************************/
uint_8 USB_Device_Deinit_EndPoint 
    (
        uint_8    controller_ID,  /* [IN] Controller ID */
        uint_8    ep_num,         /* [IN] Endpoint number */
        uint_8    direction       /* [IN] Direction */
    )
{
    /* Body */
    uint_8 status=USB_OK;
    /* check if all endpoint have already been initialised */
    if((g_EPn == g_EPn_max) && (ep_num != CONTROL_ENDPOINT))
    {
        return USBERR_EP_DEINIT_FAILED;
    } /* EndIf */
    /* call controller layer for initiazation */
    status = USB_DCI_Deinit_EndPoint(controller_ID, ep_num, direction);
    /* if endpoint successfully deinitialised update counter */
    if ((ep_num != CONTROL_ENDPOINT) && (status == USB_OK))
    {
        g_EPn++;
    } /* EndIf */
    return status;
} /* EndBody */
/**************************************************************************//*!
*
* @name  USB_Device_Get_Status
*
* @brief The funtion retrieves various endpoint as well as USB component status
*
* @param controller_ID : Controller ID
* @param component     : USB component
* @param status        : Pointer to 16 bit return value
*
* @return status
*         USB_OK                : When Successfull
*         USBERR_BAD_STATUS     : When error
*
******************************************************************************
* This function retrieves the endpoint as well USB component status which is
* stored by calling USB_Device_Set_Status. This function can be called by Ap-
* plication as well as the DCI layer.
*****************************************************************************/
uint_8 USB_Device_Get_Status 
    (
        uint_8        controller_ID,  /* [IN]  Controller ID  */
        uint_8        component,      /* [IN]  USB component */
        uint_16_ptr   status          /* [OUT] Pointer to 16 bit return value */
    )
{
    /* Body */
    /* get the endpoint number from component input variable */
    uint_8 ep_num = (uint_8)(component & USB_STATUS_ENDPOINT_NUMBER_MASK);
    UNUSED (controller_ID)
    if((component <= USB_STATUS_TEST_MODE) &&
            (component >= USB_STATUS_DEVICE_STATE))
    {
        /* Get the corresponding component status
        -1 as components start from 1 */
        *status = g_usb_component_status[component-1];
    }
    else if ((component & USB_STATUS_ENDPOINT) &&
            (ep_num < MAX_SUPPORTED_ENDPOINTS))
    {
        /* Get the corresponding endpoint status */
        *status = g_usb_ep_status[ep_num];
    }
    else
    {
        return USBERR_BAD_STATUS;
    } /* EndIf */
    return USB_OK;
} /* EndBody */
/**************************************************************************//*!
*
* @name  USB_Device_Set_Status
*
* @brief The funtion saves status of endpoints as well as USB components.
*
* @param controller_ID : Controller ID
* @param component     : USB component
* @param setting       : Value to be set
*
* @return status
*         USB_OK                : When Successfull
*         USBERR_BAD_STATUS     : When error
*
******************************************************************************
* This function sets the endpoint as well USB component status which can be
* retrieved by calling USB_Device_Get_Status. This function can be called by
* Application as well as the DCI layer.
*****************************************************************************/
uint_8 USB_Device_Set_Status
    (
        uint_8    controller_ID,  /* [IN] Controller ID */
        uint_8    component,      /* [IN] USB component */
        uint_16   setting         /* [IN] Value to be set */
    )
{
    /* Body */
    /* get the endpoint number from component input variable */
    uint_8 ep_num = (uint_8)(component & USB_STATUS_ENDPOINT_NUMBER_MASK);
    UNUSED (controller_ID)
    if((component <= USB_STATUS_TEST_MODE) &&
            (component >= USB_STATUS_DEVICE_STATE))
    {
        /* 
        Set the corresponding component setting
        -1 as components start from 1 
        */
        g_usb_component_status[component-1] = setting;
    }
    else if ((component & USB_STATUS_ENDPOINT) &&
            (ep_num < MAX_SUPPORTED_ENDPOINTS))
    {
        uint_8 direction =
        (uint_8)((component >> USB_COMPONENT_DIRECTION_SHIFT) &
        USB_COMPONENT_DIRECTION_MASK);
        /* HALT Endpoint */
        if(setting == USB_STATUS_STALLED)
        {
            USB_Device_Stall_EndPoint(controller_ID, ep_num, direction);
        }
        else if((setting == USB_STATUS_IDLE) &&
                (g_usb_ep_status[ep_num] == USB_STATUS_STALLED))
        {
            USB_Device_Unstall_EndPoint(controller_ID, ep_num, direction);
            if(ep_num == CONTROL_ENDPOINT)
            {
                direction = (uint_8)((direction == USB_SEND)?
                (USB_RECV):(USB_SEND));
                USB_Device_Unstall_EndPoint(controller_ID, ep_num, direction);
            } /* EndIf */
        } /* EndIf */
        /* Set the corresponding endpoint setting */
        g_usb_ep_status[ep_num] = setting;
    }
    else
    {
        return USBERR_BAD_STATUS;
    } /* EndIf */
    return USB_OK;
} /* EndBody */
/**************************************************************************//*!
*
* @name  USB_Device_Register_Service
*
* @brief The funtion registers a callback function from the Application layer
*
* @param controller_ID : Controller ID
* @param type          : event type or endpoint number
* @param service       : callback function pointer
*
* @return status
*         USB_OK                   : When Successfull
*         USBERR_ALLOC_SERVICE     : When invalid type or already registered
*
******************************************************************************
* This function registers a callback function from the application if it is
* called not already registered so that the regitered callback function can
* be if the event of that type occurs
*****************************************************************************/
uint_8 USB_Device_Register_Service
(
        uint_8                    controller_ID, /* [IN] Controller ID           */
        uint_8                    type,          /* [IN] type of event or endpoint
                                                            number to service       */
        USB_SERVICE_CALLBACK      service        /* [IN] pointer to callback
                                                            function                */
    )
{
    /* Body */
    UNUSED (controller_ID)
    /* check if the type is valid and callback for the type
    is not already registered */
    if(((type  <= USB_SERVICE_MAX_EP) ||
       ((type < USB_SERVICE_MAX) && (type >= USB_SERVICE_BUS_RESET))) &&
        (g_usb_CB[type] == NULL))
    {
        /* register the callback function */
        g_usb_CB[type] = service;
        return USB_OK;
    }
    else
    {
        return USBERR_ALLOC_SERVICE;
    } /* EndIf */
} /* EndBody */
/**************************************************************************//*!
*
* @name  USB_Device_Unregister_Service
*
* @brief The funtion unregisters an event or endpoint callback function
*
* @param controller_ID : Controller ID
* @param type          : event type or endpoint number
*
* @return status
*         USB_OK                   : When Successfull
*         USBERR_UNKNOWN_ERROR     : When invalid type or not registered
*
*****************************************************************************
* This function un registers a callback function which has been previously
* registered by the application layer
*****************************************************************************/
uint_8 USB_Device_Unregister_Service
    (
        uint_8 controller_ID, /* [IN] Controller ID */
        uint_8 type           /* [IN] type of event or endpoint number
                                        to service */
    )
{
    /* Body */
    UNUSED (controller_ID)
    /* check if the type is valid and callback for the type
    is already registered */
    if(((type  <= USB_SERVICE_MAX_EP) ||
                ((type < USB_SERVICE_MAX) && (type >= USB_SERVICE_BUS_RESET))) &&
            (g_usb_CB[type] != NULL))
    {
        /* unregister the callback */
        g_usb_CB[type] = NULL;
        return USB_OK;
    }
    else
    {
        return USBERR_UNKNOWN_ERROR;
    } /* EndIf */
} /* EndBody */
/**************************************************************************//*!
*
* @name  USB_Device_Call_Service
*
* @brief The funtion is a device layer event handler
*
* @param type  : Type of service or endpoint
* @param event : Pointer to event structure
*
* @return status
*         USB_OK      : Always
*
*****************************************************************************
*
* This function calls the registered service callback function of the applic-
* ation layer based on the type of event received. This function is called
* from the DCI layer.
*
*****************************************************************************/
uint_8 USB_Device_Call_Service
    (
        uint_8                    type,    /* [IN] Type of service or endpoint */
        PTR_USB_DEV_EVENT_STRUCT  event    /* [IN] Pointer to event structure  */
    )
{
    /* Body */
    if(type == USB_SERVICE_BUS_RESET)
    {    /* if it is an reset interrupt then reset all status structures */
        USB_Device_Init_Params();
    } /* EndIf */
    /* check if the callback is registered or not */
    if(g_usb_CB[type] != NULL)
    {
        /* call the callback function */
        g_usb_CB[type](event);
    } /* EndIf */
    return USB_OK;
} /* EndBody */

/* EOF */
