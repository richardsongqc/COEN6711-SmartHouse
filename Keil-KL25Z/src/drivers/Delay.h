#ifndef __DELAY_H__
#define __DELAY_H__
volatile uint32_t msTicks;                            /* counts 1ms timeTicks */
volatile static int delay=0;
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
	msTicks++;                        /* increment counter necessary in Delay() */		
	delay++;
}

/*------------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 ms)
 *------------------------------------------------------------------------------*/
__INLINE static void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}
#endif //DELAY
