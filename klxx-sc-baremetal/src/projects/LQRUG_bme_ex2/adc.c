#include "common.h"
#include "adc.h"
#include "LQRUG_bme_ex2.h"
#include "BME.h"

uint8_t cycle_flags;
uint32_t result0A;

void ADC_Init(void)
{
  	//Turn on ADC and LPTMR clock gate
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;  
	
	ADC0_CFG1 &= ~ADC_CFG1_ADICLK_MASK;
    ADC0_CFG1 |= ADC_CFG1_ADICLK(0);	//bus clock
	
	ADC0_CFG1 &= ~ADC_CFG1_MODE_MASK;
	ADC0_CFG1 |= ADC_CFG1_MODE(0);	//singled-ended 8bit mode
	
	ADC0_CFG1 &= ~ADC_CFG1_ADIV_MASK;
	ADC0_CFG1 |= ADC_CFG1_ADIV(3); //div by 8
	
	ADC0_CFG2 |= ADC_CFG2_MUXSEL_MASK  //select channel B
			| ADC_CFG2_ADHSC_MASK	//high speed mode
			;
	
	ADC0_SC2 |= ADC_SC2_ADTRG_MASK;	//hardware trigger
	
	ADC0_SC2 &= ~ADC_SC2_REFSEL_MASK;
	ADC0_SC2 |= ADC_SC2_REFSEL(0);		//VREFH/VREFL
	 
	ADC0_SC3 &= ~ADC_SC3_ADCO_MASK;  //single shot mode
	
	ADC0_SC3 |= ADC_SC3_AVGE_MASK; 	//hardware average
	
	ADC0_SC3 &= ~ADC_SC3_AVGS_MASK;
	ADC0_SC3 |= ADC_SC3_AVGS(3); 		//32 sample  				 

}

void ADC_BME_Init(void)
{
  	//Turn on ADC and LPTMR clock gate 
	BME_OR_W(&SIM_SCGC6, SIM_SCGC6_ADC0_MASK);

	//BME_BFI_W(&ADC0_CFG1, 0<<ADC_CFG1_ADICLK_SHIFT, ADC_CFG1_ADICLK_SHIFT, 2);	//ADICLK=0, bus clock
	//BME_BFI_W(&ADC0_CFG1, 0<<ADC_CFG1_MODE_SHIFT, ADC_CFG1_MODE_SHIFT, 2);	//MODE=0, 8 bit mode
	//BME_BFI_W(&ADC0_CFG1, 3<<ADC_CFG1_ADIV_SHIFT, ADC_CFG1_ADIV_SHIFT, 2);	//ADIV=3, div by 8
	
	BME_BFI_W(&ADC0_CFG1, (0<<ADC_CFG1_ADICLK_SHIFT | 0<<ADC_CFG1_MODE_SHIFT | 3<<ADC_CFG1_ADIV_SHIFT), 
			  0<<ADC_CFG1_ADICLK_SHIFT, 7);
	
	BME_OR_W(&ADC0_CFG2, ADC_CFG2_MUXSEL_MASK | ADC_CFG2_ADHSC_MASK);	
	
	BME_OR_W(&ADC0_SC2, ADC_SC2_ADTRG_MASK);	//hareware trigger
	BME_BFI_W(&ADC0_SC2, 0<<ADC_SC2_REFSEL_SHIFT, ADC_SC2_REFSEL_SHIFT, 2);	//VREFH/VREFL	
	
	BME_AND_W(&ADC0_SC3, ~ADC_SC3_ADCO_MASK);	//single shot mode
	
	BME_OR_W(&ADC0_SC3, ADC_SC3_AVGE_MASK);	//hardware average
	BME_BFI_W(&ADC0_SC3, 3<<ADC_SC3_AVGS_SHIFT, ADC_SC3_AVGS_SHIFT, 2);		//32 sample
}

uint8_t  Calibrate_ADC(void) {
    uint16_t cal_temp;
    
	ADC_Init();
	
	ADC0_SC3 |= ADC_SC3_CALF_MASK;		// clear any existing calibration failed flag
    ADC0_SC3 |= ADC_SC3_CAL_MASK;		// Start Calibration
	while(ADC0_SC3 & ADC_SC3_CAL_MASK){	// wait for calibration to finish
	};
	if(ADC0_SC3 & ADC_SC3_CALF_MASK) 		// There was a fault during calibration
		return 1;

	// Initialize (clear) a 16b variable in RAM.
	cal_temp = 0;
	
    //Add the following plus-side calibration results CLP0, CLP1, CLP2, CLP3, CLP4, CLPS and CLPD to the variable.
	cal_temp += ADC0_CLP0;
	cal_temp += ADC0_CLP1;
	cal_temp += ADC0_CLP2;
	cal_temp += ADC0_CLP3;
	cal_temp += ADC0_CLP4;
	cal_temp += ADC0_CLPS;

	// Divide the variable by two.
	cal_temp = cal_temp >> 1;
	// Set the MSB of the variable.
	cal_temp |= 0x8000;
	
	// Store the value in the plus-side gain calibration registers ADCPG
	ADC0_PG = cal_temp;
	
	// Repeat procedure for the minus-side gain calibration value.
	// Initialize (clear) a 16b variable in RAM.
	cal_temp = 0;

    //Add the following minus-side calibration results CLM0, CLM1, CLM2, CLM3, CLM4, CLMS and CLMD to the variable.
	cal_temp += ADC0_CLM0;
	cal_temp += ADC0_CLM1;
	cal_temp += ADC0_CLM2;
	cal_temp += ADC0_CLM3;
	cal_temp += ADC0_CLM4;
	cal_temp += ADC0_CLMS;
	
	// Divide the variable by two.
	cal_temp = cal_temp >> 1;
	// Set the MSB of the variable.
	cal_temp |= 0x8000;
	
	// Store the value in the minus-side gain calibration registers ADCMG
	ADC0_MG = cal_temp;

	return 0;
}

uint8_t  Calibrate_BME_ADC(void) {
    uint16_t cal_temp;

	//ADC0_SC3 |= ADC_SC3_CALF_MASK;		// clear any existing calibration failed flag
	BME_OR_W(&ADC0_SC3, ADC_SC3_CALF_MASK);
    //ADC0_SC3 |= ADC_SC3_CAL_MASK;		// Start Calibration
	BME_OR_W(&ADC0_SC3, ADC_SC3_CAL_MASK);
	while(BME_UBFX_W(&ADC0_SC3, ADC_SC3_CAL_SHIFT, 1)){	// wait for calibration to finish
	};
	if(BME_UBFX_W(&ADC0_SC3, ADC_SC3_CALF_SHIFT, 1)) 		// There was a fault during calibration
		return 1;

	// Initialize (clear) a 16b variable in RAM.
	cal_temp = 0;
	
    //Add the following plus-side calibration results CLP0, CLP1, CLP2, CLP3, CLP4, CLPS and CLPD to the variable.
	cal_temp += ADC0_CLP0;
	cal_temp += ADC0_CLP1;
	cal_temp += ADC0_CLP2;
	cal_temp += ADC0_CLP3;
	cal_temp += ADC0_CLP4;
	cal_temp += ADC0_CLPS;

	// Divide the variable by two.
	cal_temp = cal_temp >> 1;
	// Set the MSB of the variable.
	cal_temp |= 0x8000;
	
	// Store the value in the plus-side gain calibration registers ADCPG
	ADC0_PG = cal_temp;
	
	// Repeat procedure for the minus-side gain calibration value.
	// Initialize (clear) a 16b variable in RAM.
	cal_temp = 0;

    //Add the following minus-side calibration results CLM0, CLM1, CLM2, CLM3, CLM4, CLMS and CLMD to the variable.
	cal_temp += ADC0_CLM0;
	cal_temp += ADC0_CLM1;
	cal_temp += ADC0_CLM2;
	cal_temp += ADC0_CLM3;
	cal_temp += ADC0_CLM4;
	cal_temp += ADC0_CLMS;
	
	// Divide the variable by two.
	cal_temp = cal_temp >> 1;
	// Set the MSB of the variable.
	cal_temp |= 0x8000;
	
	// Store the value in the minus-side gain calibration registers ADCMG
	ADC0_MG = cal_temp;

	return 0;
}

void ADC_Start(uint8_t channel)
{
    ADC0_SC1A = (ADC_SC1_AIEN_MASK |channel); 	
}

void ADC_Stop(void)
{
    ADC0_SC1A = ADC_SC1_ADCH_MASK; //stop conversion
}

void adc0_isr(void)
{
	if (( ADC0_SC1A & ADC_SC1_COCO_MASK ) == ADC_SC1_COCO_MASK) 
	{  // check which of the two conversions just triggered 
		result0A = ADC0_RA;           // this will clear the COCO bit that is also the interrupt flag
		
		cycle_flags |= ADC0A_DONE ;   // mark this step done 
		GPIOA_PDOR ^= (1<<5);		//toggle orange LED
	}   
}

void ADC_Trigger(void)
{
	//Configure LPTMR as ADC trigger
	SIM_SOPT7 |= (SIM_SOPT7_ADC0ALTTRGEN_MASK 
				  | !SIM_SOPT7_ADC0PRETRGSEL_MASK);
	SIM_SOPT7 &= ~SIM_SOPT7_ADC0TRGSEL_MASK;	
	SIM_SOPT7 |= SIM_SOPT7_ADC0TRGSEL(LPTMR0_TRG);   
}


void ADC_BME_Trigger(void)
{
	//Configure LPTMR as ADC trigger
	BME_OR_W(&SIM_SOPT7, SIM_SOPT7_ADC0ALTTRGEN_MASK 
					  | !SIM_SOPT7_ADC0PRETRGSEL_MASK );
	BME_BFI_W(&SIM_SOPT7, LPTMR0_TRG << SIM_SOPT7_ADC0TRGSEL_SHIFT, SIM_SOPT7_ADC0TRGSEL_SHIFT, 4);
	  
  
}
