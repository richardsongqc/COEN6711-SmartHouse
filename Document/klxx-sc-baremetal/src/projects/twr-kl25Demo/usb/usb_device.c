/*
 * File:		usb_device.c
 * Purpose:		Main process
 *
 */

#include "common.h"

/********************************************************************/
int main (void)
{
	char ch;
        
  	printf("\nRunning the usb_device project.\n");

	while(1)
	{
		ch = in_char();
		out_char(ch);
	} 
}
/********************************************************************/