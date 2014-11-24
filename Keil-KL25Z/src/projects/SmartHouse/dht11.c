#include "dht11.h"
#include "MKL25Z4.h" 
#include "stdint.h"

#define LED_NUM     3                   /* Number of user LEDs                */
const uint32_t led_mask[] = {1UL << 18, 1UL << 19, 1UL << 1};

#define LED_RED    0
#define LED_GREEN  1
#define LED_BLUE     2
//#define LED_CLK    4

DHT dht;
stat LineStat;



volatile uint32_t msTicks;                            /* counts 1ms timeTicks */
int32_t IntervalBegin;

/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
//---------------------------------------
void SysTick_Handler(void) {
  msTicks++;                       
}
/*-----------------------------------------*/

/*------------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 ms)
 *------------------------------------------------------------------------------*/
__INLINE static void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}





void InitializeLED(void) 
{
    SIM->SCGC5    |= (1UL <<  10) | (1UL <<  12);       /* Enable Clock to Port B & D */ 
    PORTB->PCR[18] = (1UL <<  8);                       /* Pin PTB18 is GPIO */
    PORTB->PCR[19] = (1UL <<  8);                       /* Pin PTB19 is GPIO */
    PORTD->PCR[1]  = (1UL <<  8);                       /* Pin PTD1  is GPIO */
    
    PORTB->PCR[0]=(1UL<<8);                             /*Pin PTB0 is Data*/
    PORTB->PCR[1]=(1UL<<8);                             /*Pin PTB1 is relay1*/
    PORTB->PCR[2]=(1UL<<8);                             /*Pin PTB2 is relay2*/
    
    FPTB->PDOR = (led_mask[0] | led_mask[1] );          /* switch Red/Green LED off  */
    FPTB->PDDR = (led_mask[0] | led_mask[1]|1UL<<1|1UL<<2 );          /* enable PTB18/19 as Output */

    FPTD->PDOR = led_mask[2];                           /* switch Blue LED off  */
    FPTD->PDDR = led_mask[2];                           /* enable PTD1 as Output */
    
    
}

/*----------------------------------------------------------------------------
  Function that turns on Red LED
 *----------------------------------------------------------------------------*/
void LEDRed_On (void) 
{
    FPTD->PSOR   = led_mask[LED_BLUE];   /* Blue LED Off*/
    FPTB->PSOR   = led_mask[LED_GREEN];  /* Green LED Off*/
    FPTB->PCOR   = led_mask[LED_RED];    /* Red LED On*/
}

/*----------------------------------------------------------------------------
  Function that turns on Green LED
 *----------                 ------------------------------------------------------------------*/
void LEDGreen_On (void) 
{
    FPTB->PSOR   = led_mask[LED_RED];     /* Red LED Off*/
    FPTD->PSOR   = led_mask[LED_BLUE];    /* Blue LED Off*/
    FPTB->PCOR   = led_mask[LED_GREEN];   /* Green LED On*/
}

/*----------------------------------------------------------------------------
  Function that turns on Blue LED
 *----------------------------------------------------------------------------*/
void LEDBlue_On (void) 
{
    FPTB->PSOR   = led_mask[LED_GREEN];   /* Green LED Off*/
    FPTB->PSOR   = led_mask[LED_RED];     /* Red LED Off*/
    FPTD->PCOR   = led_mask[LED_BLUE];    /* Blue LED On*/
}

/*----------------------------------------------------------------------------
  Function that turns all LEDs off
 *----------------------------------------------------------------------------*/
void LED_Off (void) 
{
    FPTB->PSOR   = led_mask[LED_GREEN];   /* Green LED Off*/
    FPTB->PSOR   = led_mask[LED_RED];     /* Red LED Off*/
    FPTD->PSOR   = led_mask[LED_BLUE];    /* Blue LED Off*/
}

/*----------------------------------------------------------------------------
 *        Thread 1 'phaseA': Phase A output
 *---------------------------------------------------------------------------*/
void phaseA (void) 
{
    LEDRed_On();
}

void phaseB (void) 
{
    LEDBlue_On();
}

/*----------------------------------------------------------------------------
 *        Thread 3 'phaseC': Phase C output
 *---------------------------------------------------------------------------*/
void phaseC (void) 
{
    LEDGreen_On();
}

/*----------------------------------------------------------------------------
 *        Thread 4 'phaseD': Phase D output
 *---------------------------------------------------------------------------*/
void phaseD (void) 
{
    LED_Off();
}


long TimeOver;
long SendCount;
int LowCount;
int HighCount;
int BitCount;
int Rec;

void clearCount()
{
    TimeOver=0;
    LowCount=0;
    HighCount=0;
    BitCount=0;
    dht.DataReady=0;
    Rec=0;
}


int BitRec()
{
	int i; int j;
	int a;int ret=0;int sum;
	int32_t inter;
	//if((a&0x01)==0)
	for (i=1;i<=5;i++)
	{
		ret=0;			//HighCount=0;
		Rec = 0;
		for(j=0;j<=7;j++)
		{
			a=FPTB->PDIR;
			while((a&0x01)==0 )a=FPTB->PDIR;
			IntervalBegin=msTicks;
			while((a&0x01)==1)a=FPTB->PDIR;
			inter=msTicks-IntervalBegin;
			if (inter<=0x05)
			{
				Rec=Rec<<1;
//				BitCount++;
			}
			else
			{
				Rec=Rec<<1;
				Rec=Rec|1;
//				BitCount++;
			}
		}
		switch (i)
		{
		case 1:
			dht.RecHumI=Rec;
			break;
		case 2:
			dht.RecHumD=Rec;
			break;
		case 3:
			dht.RecTI=Rec;
			break;
		case 4:
			dht.RecTD=Rec;
			break;
		case 5:
			ret=Rec;        //check sum 
			sum=dht.RecHumI+dht.RecHumD+dht.RecTD+dht.RecTI;
			sum=sum & 0xFF;
			if (sum==ret) 
			{
				return 1;       
			}   
			else
			{
				return 0;
			}
			//break;
		}
	}
}

int GetTemp( void )
{   
	int a=0;
	//int32_t inter;
	clearCount();
	dht.DataReady = 0;
	FPTB->PCOR   = 1UL<<18;    /* Red LED On*/
	Delay(100000);
	FPTB->PSOR = 1UL<<18;     //--------------------
			
	FPTB->PDDR = (1UL<<18| 1UL<<19|1UL<<1|1UL<<2|1 ); //pin setup output
	FPTB->PCOR=1;       //-----------------send low
	//SendCount++;
	Delay(2000);
	FPTB->PSOR=1;       //------------------send high 
	Delay(2);

	FPTB->PDDR = (1UL<<18| 1UL<<19|1UL<<1|1UL<<2); //pin setup input

	a=FPTB->PDIR;
	while((a&0x01)!=0) a=FPTB->PDIR;
	while((a&0x01)==0) a=FPTB->PDIR;
	//IntervalBegin=msTicks;
	while((a&0x01)==1) a=FPTB->PDIR;
	//inter=msTicks-IntervalBegin;
	//if(inter>15 && inter<65)return0;
	FPTB->PCOR   = 1UL<<18;    /* Red LED On*/
	return BitRec();
    
}



void CtrlRelay(char Port1,char Port2)
{
    
    if(Port1==1)
    {
        FPTB->PSOR=1UL<<1;
    }
    else
    {
        FPTB->PCOR=1UL<<1;
    }
    if(Port2==1)
    {
        FPTB->PSOR=1UL<<2;
    }
    else
    {
        FPTB->PCOR=1UL<<2;
    }
}

/*----------------------------------------------------------------------------
 *      Main: Initialize
 *---------------------------------------------------------------------------*/


