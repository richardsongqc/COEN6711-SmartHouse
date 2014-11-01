#ifndef __LPTMR_H
#define __LPTMR_H

void lptmr_isr(void);
void Lptmr_Init(int count, int clock_source);
void Lptmr_Start(void);

void Lptmr_BME_Init(int count, int clock_source);

#endif
