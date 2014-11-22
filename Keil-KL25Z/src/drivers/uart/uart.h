/*
 * File:		uart.h
 * Purpose:     Provide common ColdFire uart routines for polled serial IO
 *
 * Notes:
 */

#ifndef __uart_H__
#define __uart_H__

#include "stdio.h"
//#include "MKL25Z4.H"
#include "MemMapPtr_KL25Z4.h"
/********************************************************************/

void uart0_init (UART0_MemMapPtr uartch, int sysclk, int baud);
char uart0_getchar (UART0_MemMapPtr channel);
void uart0_putchar (UART0_MemMapPtr channel, char ch);
int uart0_getchar_present (UART0_MemMapPtr channel);
unsigned int len(const char *str);
void print(const char *str);
void print1(uint8_t *str);
void PutChar(char ch);
/********************************************************************/

#endif /* __uart_H__ */
