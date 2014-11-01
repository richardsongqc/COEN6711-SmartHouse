/****************************************************************************************************/
/**
Copyright (c) 2011 Freescale Semiconductor
Freescale Confidential Proprietary
\file       TWR-K20D540M_DEF.c
\brief      
\author     
\author     
\version    1.0
\date       Sep 26, 2011
*/


#include "common.h"
#include "ROBOT_GPIO.h"

uint8 pwm_count;
uint8 pwm1_duty;
uint8 pwm2_duty;
uint8 pwm3_duty;
uint8 pwm4_duty;
char pwm_val = 8;


void move_robot(ROBOT_MOVE  direction)
{
 switch (direction)
 {
   case MOVE_STOP: robot_stop(); break;
   case MOVE_FWD: robot_forward(); break;
   case MOVE_BWD: robot_bakcward(); break;
   case MOVE_CW: robot_clockwise(); break;
   case MOVE_CCW: robot_counterclockwise(); break;
  default: 
     robot_stop();
 }
}


#if 0
void robot_forward(void)
 {
  DATA1_ON;
  DATA2_OFF;
  
  DATA3_ON;
  DATA4_OFF;
 }


void robot_stop(void)
 {
  DATA1_OFF;
  DATA2_OFF;
  
  DATA3_OFF;
  DATA4_OFF;
 }


void robot_bakcward(void)
 {
  DATA1_OFF;
  DATA2_ON;
  
  DATA3_OFF;
  DATA4_ON;
 }


void robot_clockwise(void)
 {
  DATA1_ON;
  DATA2_OFF;
  
  DATA3_OFF;
  DATA4_ON;
 }


void robot_counterclockwise(void)
 {
  DATA1_OFF;
  DATA2_ON;
  
  DATA3_ON;
  DATA4_OFF;
 }

#endif 
/**********************************/


void robot_forward(void)
 {
  pwm1_duty = pwm_val;
  pwm2_duty = 0;
  
  pwm3_duty = pwm_val;
  pwm4_duty = 0;
 }


void robot_stop(void)
 {
  pwm1_duty = 0;
  pwm2_duty = 0;
  
  pwm3_duty = 0;
  pwm4_duty = 0;
 }


void robot_bakcward(void)
 {
  pwm2_duty = pwm_val;
  pwm1_duty = 0;
  
  pwm4_duty = pwm_val;
  pwm3_duty = 0;
 }


void robot_clockwise(void)
 {
  pwm1_duty = pwm_val;
  pwm2_duty = 0;
  
  pwm3_duty = 0;
  pwm4_duty = pwm_val;
 }


void robot_counterclockwise(void)
 {
  pwm1_duty = 0;
  pwm2_duty = pwm_val;
  
  pwm3_duty = pwm_val;
  pwm4_duty = 0;
 }


void  robot_GPIO_init(void)
{  
#ifdef DATA1
  DATA1_PCR = PCR_OUTPUT_CONFIG;
  DATA1_OFF;
  DATA1_OUTPUT_EN;
#endif
    
#ifdef DATA2
  DATA2_PCR = PCR_OUTPUT_CONFIG;
  DATA2_OFF;
  DATA2_OUTPUT_EN;
#endif

#ifdef DATA3
  DATA3_PCR = PCR_OUTPUT_CONFIG;
  DATA3_OFF;
  DATA3_OUTPUT_EN;
#endif
  
#ifdef DATA4
  DATA4_PCR = PCR_OUTPUT_CONFIG;
  DATA4_OFF;
  DATA4_OUTPUT_EN;
#endif

}




/*
  This function must be called periodicallr


*/
void Robot_GPIO_Pseudo_PWM(void)
{
   pwm_count++;
   pwm_count &= 0xF;  //module
  
  if (pwm1_duty>= pwm_count) DATA1_OFF ; else DATA1_ON;
  if (pwm2_duty>= pwm_count) DATA2_OFF ; else DATA2_ON;
  if (pwm3_duty>= pwm_count) DATA3_OFF ; else DATA3_ON;
  if (pwm4_duty>= pwm_count) DATA4_OFF ; else DATA4_ON;
   
}
