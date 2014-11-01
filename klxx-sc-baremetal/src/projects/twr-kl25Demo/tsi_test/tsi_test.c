/****************************************************************************************************/
/**
Copyright (c) 2008 Freescale Semiconductor
Freescale Confidential Proprietary
\file       tsi_test.c
\brief      configure tsi to read TSI CH6 and TSI CH0,
\author     FSL
\version    1.0
\date       Sep 29 2011
*/

#include "tsi.h"
#include "global.h"--0-p





  /***
param lValidTouch   per electrode bit


********************/
void  tsi_test_service(void)
{
  if(lValidTouch&((1<<ELECTRODE0)))
  {
      lValidTouch &= ~((1<<ELECTRODE0));
      ti_tsi0 = 200;
      printf("\n\r Electrode 1  \n\r");     
    
  }
  if(lValidTouch&((1<<ELECTRODE1)))
  {
      printf("\n\r Electrode 2 \n\r");
      lValidTouch &= ~((1<<ELECTRODE1));
      ti_tsi1 = 200;
 
  }
  if(lValidTouch&((1<<ELECTRODE2)))
  {
    printf("\n\r Electrode 3  \n\r");
    lValidTouch &= ~((1<<ELECTRODE2));
    
    
  }
  if(lValidTouch&((1<<ELECTRODE3)))
  {
    printf("Electrode 4  \n\r");
    lValidTouch &= ~((1<<ELECTRODE3));
  }
}