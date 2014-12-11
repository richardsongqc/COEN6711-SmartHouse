/*
 * File:		rtc.h
 * Purpose:     Provide common rtc functions
 *
 * Notes:
 */

#ifndef __RTC_H__
#define __RTC_H__

/********************************************************************/

void rtc_init(unsigned int seconds, unsigned int alarm, unsigned int c_interval, unsigned char c_value, unsigned char interrupt);      
void rtc_isr(void);
void rtc_reg_report (void);

/********************************************************************/

#endif /* __RTC_H__ */
