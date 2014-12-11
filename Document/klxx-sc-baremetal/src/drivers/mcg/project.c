#include <MKL25Z4.h>
#include "mcg.h"
#include "freedom.h"
#include "common.h"

int main( void) 
{
	int BusClock;
	uint8 i;
	
	SystemCoreClockUpdate(); // Get Core Clock Frequent
	i = pll_init(8000000, LOW_POWER, CRYSTAL, PLL0_PRDIV, PLL0_VDIV, MCGOUT);
	SystemCoreClockUpdate(); // Update the system Core Clock with the new MCG mode
	
	BusClock=(SystemCoreClock/(1u+((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV4_MASK) >> SIM_CLKDIV1_OUTDIV4_SHIFT)));
	return 0;
}