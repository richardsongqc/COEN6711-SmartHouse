/*
 * File:		twr-KL25Demo.c
 * Purpose:		Main process
 *
 */

#include "common.h"
#include "pit.h"
#include "global.h"
#include "twr_kl25_gpio.h"
#include "hal_i2c.h"
#include "hal_dev_mma8451.h"
#include "angle_cal.h"
#include "task_mgr.h"
#include "adc_test.h"
#include "rtc.h"
#include "TSIDrv.h"
#include "usb_cdc.h"
#include "usb_reg.h"
#include "start.h"
/* Testing Suite */
#define USB_DEVICE_XCVR_TEST
uint8 gu8USB_Buffer[64];

extern uint8 gu8USB_Flags; 
extern uint8 gu8EP3_OUT_ODD_Buffer[];
extern tBDT tBDTtable[];

void adc_init(void);
void tsi_test_init(void);
void tsi_test_service(void);


/*****************************************************************************************************
* Definition of module wide VARIABLEs - NOT for use in other modules
*****************************************************************************************************/
signed short accel_x, accel_y, accel_z;
signed short resultx, resulty, resultz;



void usb_init(void)
{
      printf("\n********** USB XCVR Module Testing **********\n");     
      USB_REG_SET_ENABLE;
      CDC_Init();
      // Pull up enable
      FLAG_SET(USB_CONTROL_DPPULLUPNONOTG_SHIFT,USB0_CONTROL);
}


void usb_service(void)
{
      // If data transfer arrives
        if(FLAG_CHK(EP_OUT,gu8USB_Flags))
        {
            (void)USB_EP_OUT_SizeCheck(EP_OUT);         
            usbEP_Reset(EP_OUT);
            usbSIE_CONTROL(EP_OUT);
            FLAG_CLR(EP_OUT,gu8USB_Flags);

            // Send it back to the PC
            EP_IN_Transfer(EP2,CDC_OUTPointer,1);
            
        }
        
           if (!ti_print)
            {
             ti_print = 300;
             sprintf((char *)gu8USB_Buffer,"\r RTC = %5i s", RTC_TSR);
             EP_IN_Transfer(EP2,gu8USB_Buffer,15);
            }
            
        
}    
       



void accel_init(void)
{
    unsigned char tmp;
    printf("****Accelerometor init ****\n");
   //Configure MMA8451 sensor //MMA8451Q
    hal_dev_mma8451_init();      //Initialize I2C modules
    tmp = hal_dev_mma8451_read_reg(0x2a);
    hal_dev_mma8451_write_reg(0x2a,tmp|0x01);
}

void accel_demo(void)
{
        static int note,notez;
        static int cont=0x1;

        note  = yz_angle/100;  //9-9
        if (note != notez && xz_mag > 20 && !ti_delay)
        {
         if (note <-9) note=-9;
         if (note >9)  note =9;
         if (notez < note) notez++;
         if (notez > note) notez--;

         cont = cont << 0x1;
         if (cont >= 0x10 || cont==0) cont = 0x01;
         ti_delay = 75;
        }
}


const uint8 RAIN_SEQUENCE[] = 
  {
    0x01,
    0x02,
    0x04,   
    0x08,
    0x00,
  };


#define TIME_LED_SEQ   25 // base time for sequence falling

void led_test(void)
 {
   static uint16 period_led;
   static uint16 sw1_count;
   static uint16 sw2_count;
    
   static int rain_sequence_off;
   static int note,notez;
   static char aux_1,aux_SW1, aux_SW2;
   
   note  = yz_angle/100;  //9-9
   if (!ti_led)
        {
         if (note <-9) note=-9;
         if (note >9)  note =9;
         if (notez < note) notez++;
         if (notez > note) notez--;       
         if (notez>0)
         {
           if (++rain_sequence_off >= sizeof(RAIN_SEQUENCE)) rain_sequence_off=0;
           ti_led = (10-notez)*TIME_LED_SEQ; 
          }
         if (notez<0)
         {
           if (--rain_sequence_off < 0) rain_sequence_off=sizeof(RAIN_SEQUENCE);
           ti_led = (10+notez)*TIME_LED_SEQ; 
         }  
         if (RAIN_SEQUENCE[rain_sequence_off] & 0x1) LED1_ON;else LED1_OFF;
         if (RAIN_SEQUENCE[rain_sequence_off] & 0x2) LED2_ON;else LED2_OFF;
         if (RAIN_SEQUENCE[rain_sequence_off] & 0x4) LED3_ON;else LED3_OFF;
         if (RAIN_SEQUENCE[rain_sequence_off] & 0x8) LED4_ON;else LED4_OFF;
        }  
    
       if (notez==0)
       {
         if (RTC_TSR & 0x01)LED1_ON;else LED1_OFF; 
         
         if (SW1_ON || Electrode1 || ti_tsi0) LED3_ON;else LED3_OFF;
         if (SW2_ON || Electrode2 || ti_tsi1) LED4_ON;else LED4_OFF;
                 
         if (input_rise(SW1_ON,&aux_SW1)) printf("\n\r SW1 click %i",sw1_count++);
         if (input_rise(SW2_ON,&aux_SW2)) printf("\n\r SW2 click %i",sw2_count++);
             
        
         if (ti_led<period_led/2) LED2_ON;else LED2_OFF;
         
         if (!ti_led)
         { 
           period_led=adc_sample/10;
           if (period_led<40) period_led=40;
           ti_led=  period_led;    //adc value goes from 0 to 4095      40 to 409
         }
       }
    adc_sample = adc_read(4);
         if (input_rise(base_time125m%0x2,&aux_1))
         { 
 
                printf("\n\r yz=[%03i],  xz[%03i] ,xy[%03i]  POT=%i ",yz_angle/10, xz_angle/10,xy_angle/10, adc_sample);
         }               
    
   
    
 }




void accel_read(void)
{
  
        if((hal_dev_mma8451_read_reg(0x00)&0xf) != 0)
        {

            accel_x   = hal_dev_mma8451_read_reg(0x01)<<8;
            accel_x  |= hal_dev_mma8451_read_reg(0x02);
            accel_x >>= 2;

            accel_y   = hal_dev_mma8451_read_reg(0x03)<<8;
            accel_y  |= hal_dev_mma8451_read_reg(0x04);
            accel_y >>= 2;

            accel_z   = hal_dev_mma8451_read_reg(0x05)<<8;
            accel_z  |= hal_dev_mma8451_read_reg(0x06);
            accel_z >>= 2;

            resultx   = hal_dev_mma8451_read_reg(0x01)<<8;
            resultx  |= hal_dev_mma8451_read_reg(0x02);
            resultx >>= 8;

            resulty   = hal_dev_mma8451_read_reg(0x03)<<8;
            resulty  |= hal_dev_mma8451_read_reg(0x04);
            resulty >>= 8;

            resultz   = hal_dev_mma8451_read_reg(0x05)<<8;
            resultz  |= hal_dev_mma8451_read_reg(0x06);
            resultz >>= 8;
            
            
            angle_calculation(); //-900  to  900            
            detect_fall_detection();
            accel_demo();
        }
}

void vfn_test_accel(void)
{
               if (ti_accel_sampling==0)
              {
                accel_read();              
                ti_accel_sampling = 10;            
               } 
               
               led_test();
              
               

}

/******************************************************************************
 Name	main loop
Parameters		none                         
Returns			none
Notes         		
******************************************************************************/
int main (void)
{
	 #ifdef CMSIS  // If we are conforming to CMSIS, we need to call start here
    start();
		#endif           
  	printf("\n TWR KL25  Demo Code !!\n");
        Twr_gpio_init();
        TSI_Init();    
        usb_init();
        rtc_init();  // driven by 32kHz external oscillator
        Pit_init();      //1msec base time on PIT0 CH0
        Twr_gpio_init();
        TSI_Init();    
        usb_init();
        accel_init();
        adc_init();
        next_task(vfn_test_accel);
	while(1)
	{
          CDC_Engine();
          usb_service();            
          TSI_Electrode_detection();             
          ptr_next_task();  // do the actual function 
         
	} 
}
/********************************************************************/
