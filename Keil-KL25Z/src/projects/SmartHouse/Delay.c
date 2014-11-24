#include "Delay.h"

volatile uint32_t msTicks;                            /* counts 1ms timeTicks */
int32_t IntervalBegin;

/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
//---------------------------------------
void SysTick_Handler(void) 
{
	msTicks++;                       
}
/*-----------------------------------------*/

/*------------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 ms)
 *------------------------------------------------------------------------------*/
__INLINE static void Delay (uint32_t dlyTicks) 
{
	uint32_t curTicks;

	curTicks = msTicks;
	while ((msTicks - curTicks) < dlyTicks);
}