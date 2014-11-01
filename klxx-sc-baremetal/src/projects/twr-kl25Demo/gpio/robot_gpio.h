/****************************************************************************************************/
/**
Copyright (c) 2011 Freescale Semiconductor
Freescale Confidential Proprietary
\file       ROBOT_GPIO.h
\brief      
\author     
\author     
\version    1.0
\date       Sep 26, 2011
*/

#include "common.h"




typedef enum
 {
   MOVE_STOP = 0,
   MOVE_FWD  = 1,
   MOVE_BWD  = 2,
   MOVE_CW   = 3,
   MOVE_CCW  = 4,
 } ROBOT_MOVE;

void move_robot(ROBOT_MOVE  direction);
void robot_forward(void);
void robot_stop(void);
void robot_bakcward(void);
void robot_clockwise(void);
void robot_bakcward(void);
void robot_counterclockwise(void);
void robot_GPIO_init(void);
extern char pwm_val;

#define BIT_MASK(x)   (0x1<<x)
// User should only define wich port and the bit 

#define DATA1_PORT   B   // Orange DATA
#define DATA1_BIT    18

#define DATA2_PORT   B    // Yellow DATA
#define DATA2_BIT    19

#define DATA3_PORT   C    // Green
#define DATA3_BIT    5

#define DATA4_PORT   C    // Blue
#define DATA4_BIT    6


// Compilation time Definitions

#define PCR(PTO,BIT)  PORT##PTO##_PCR##BIT
#define PDDR(PTO)     GPIO##PTO##_PDDR
#define PSOR(PORT)    GPIO##PORT##_PSOR
#define PCOR(PORT)    GPIO##PORT##_PCOR
#define PTOR(PORT)    GPIO##PORT##_PTOR
#define PDOR(PORT)    GPIO##PORT##_PDOR
#define PDIR(PORT)    GPIO##PORT##_PDIR



#define  PCR_OUTPUT_CONFIG  (PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK ) //Pin configured as GPIO
#define  PCR_INPUT_CONFIG   (PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK) //Pin configured as GPIO Input with pull up enable


#ifdef  DATA1_PORT
#define DATA1
#define DATA1_PCR_OUTPUT(DATA1_PORT,DATA1_BIT)  PCR(DATA1_PORT,DATA1_BIT)
#define DATA1_DDR_OUTPUT(DATA1_PORT,DATA1_BIT)  PDDR(DATA1_PORT) |= (1<<DATA1_BIT)
#define DATA1_PSOR(DATA1_PORT,DATA1_BIT)        PSOR(DATA1_PORT) |= (1<<DATA1_BIT)
#define DATA1_PCOR(DATA1_PORT,DATA1_BIT)        PCOR(DATA1_PORT) |= (1<<DATA1_BIT)
#define DATA1_PTOR(DATA1_PORT,DATA1_BIT)        PTOR(DATA1_PORT) |= (1<<DATA1_BIT)

#define DATA1_PCR        DATA1_PCR_OUTPUT(DATA1_PORT,DATA1_BIT) 
#define DATA1_OUTPUT_EN  DATA1_DDR_OUTPUT(DATA1_PORT,DATA1_BIT) 

#define DATA1_SET         DATA1_PSOR(DATA1_PORT,DATA1_BIT)
#define DATA1_CLR         DATA1_PCOR(DATA1_PORT,DATA1_BIT)

#define DATA1_ON         DATA1_PCOR(DATA1_PORT,DATA1_BIT)
#define DATA1_OFF        DATA1_PSOR(DATA1_PORT,DATA1_BIT)
#define DATA1_TOGGLE     DATA1_PTOR(DATA1_PORT,DATA1_BIT)
#endif


#ifdef  DATA2_PORT
#define DATA2
#define DATA2_PCR_OUTPUT(DATA2_PORT,DATA2_BIT)  PCR(DATA2_PORT,DATA2_BIT)
#define DATA2_DDR_OUTPUT(DATA2_PORT,DATA2_BIT)  PDDR(DATA2_PORT) |= (1<<DATA2_BIT)
#define DATA2_PSOR(DATA2_PORT,DATA2_BIT)        PSOR(DATA2_PORT) |= (1<<DATA2_BIT)
#define DATA2_PCOR(DATA2_PORT,DATA2_BIT)        PCOR(DATA2_PORT) |= (1<<DATA2_BIT)
#define DATA2_PTOR(DATA2_PORT,DATA2_BIT)        PTOR(DATA2_PORT) |= (1<<DATA2_BIT)

#define DATA2_PCR         DATA2_PCR_OUTPUT (DATA2_PORT,DATA2_BIT)
#define DATA2_OUTPUT_EN   DATA2_DDR_OUTPUT (DATA2_PORT,DATA2_BIT)

#define DATA2_SET      DATA2_PSOR(DATA2_PORT,DATA2_BIT)
#define DATA2_CLR      DATA2_PCOR(DATA2_PORT,DATA2_BIT)

#define DATA2_ON       DATA2_PCOR(DATA2_PORT,DATA2_BIT)
#define DATA2_OFF      DATA2_PSOR(DATA2_PORT,DATA2_BIT)
#define DATA2_TOGGLE   DATA2_PTOR(DATA2_PORT,DATA2_BIT)
#endif

#ifdef  DATA3_PORT
#define DATA3
#define DATA3_PCR_OUTPUT(DATA3_PORT,DATA3_BIT)  PCR(DATA3_PORT,DATA3_BIT)
#define DATA3_DDR_OUTPUT(DATA3_PORT,DATA3_BIT)  PDDR(DATA3_PORT) |= (1<<DATA3_BIT)
#define DATA3_PSOR(DATA3_PORT,DATA3_BIT)        PSOR(DATA3_PORT) |= (1<<DATA3_BIT)
#define DATA3_PCOR(DATA3_PORT,DATA3_BIT)        PCOR(DATA3_PORT) |= (1<<DATA3_BIT)
#define DATA3_PTOR(DATA3_PORT,DATA3_BIT)        PTOR(DATA3_PORT) |= (1<<DATA3_BIT)

#define DATA3_PCR        DATA3_PCR_OUTPUT (DATA3_PORT,DATA3_BIT)
#define DATA3_OUTPUT_EN   DATA3_DDR_OUTPUT (DATA3_PORT,DATA3_BIT)

#define DATA3_CLR       DATA3_PCOR(DATA3_PORT,DATA3_BIT) //DATA turn with positive
#define DATA3_SET       DATA3_PSOR(DATA3_PORT,DATA3_BIT)

#define DATA3_OFF       DATA3_PCOR(DATA3_PORT,DATA3_BIT) //DATA turn with positive
#define DATA3_ON        DATA3_PSOR(DATA3_PORT,DATA3_BIT)
#define DATA3_TOGGLE   DATA3_PTOR(DATA3_PORT,DATA3_BIT)
#endif


#ifdef  DATA4_PORT                               
#define DATA4
#define DATA4_PCR_OUTPUT(DATA4_PORT,DATA4_BIT)  PCR(DATA4_PORT,DATA4_BIT)
#define DATA4_DDR_OUTPUT(DATA4_PORT,DATA4_BIT)  PDDR(DATA4_PORT) |= (1<<DATA4_BIT)
#define DATA4_PSOR(DATA4_PORT,DATA4_BIT)        PSOR(DATA4_PORT) |= (1<<DATA4_BIT)
#define DATA4_PCOR(DATA4_PORT,DATA4_BIT)        PCOR(DATA4_PORT) |= (1<<DATA4_BIT)
#define DATA4_PTOR(DATA4_PORT,DATA4_BIT)        PTOR(DATA4_PORT) |= (1<<DATA4_BIT)
#define DATA4_PDOR(DATA4_PORT,DATA4_BIT)        PDOR(DATA4_PORT) & (1<<DATA4_BIT)


#define DATA4_PCR        DATA4_PCR_OUTPUT (DATA4_PORT,DATA4_BIT)
#define DATA4_OUTPUT_EN   DATA4_DDR_OUTPUT (DATA4_PORT,DATA4_BIT)

#define DATA4_CLR    DATA4_PCOR(DATA4_PORT,DATA4_BIT)    //DATA turn with positive
#define DATA4_SET    DATA4_PSOR(DATA4_PORT,DATA4_BIT)

#define DATA4_OFF      DATA4_PCOR(DATA4_PORT,DATA4_BIT)    //DATA turn with positive
#define DATA4_ON       DATA4_PSOR(DATA4_PORT,DATA4_BIT)
#define DATA4_TOGGLE   DATA4_PTOR(DATA4_PORT,DATA4_BIT)
#endif


