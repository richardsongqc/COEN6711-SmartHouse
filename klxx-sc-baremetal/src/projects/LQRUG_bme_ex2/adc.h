#ifndef __ADC_H
#define __ADC_H

extern uint8_t cycle_flags;
extern uint32_t result0A;

void ADC_Init(void);
uint8_t  Calibrate_ADC(void);
void ADC_Start(uint8_t channel);
void ADC_Stop(void);

void ADC_BME_Init(void);
uint8_t  Calibrate_BME_ADC(void);

void ADC_Trigger(void);
void ADC_BME_Trigger(void);

#endif

