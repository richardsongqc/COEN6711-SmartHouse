/*
 * File:		usb_host.c
 * Purpose:		Main process
 *
 */

#include "common.h"

/********************************************************************/
int main (void)
{
	char ch;
        
  	printf("\nRunning the usb_host project.\n");

	while(1)
	{
		ch = in_char();
		out_char(ch);
	} 
}
/********************************************************************/