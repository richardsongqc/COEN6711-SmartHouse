*******************************************************************************************
* Project Name:    low_power_demo
*
* Freescale Semiconductor
*
* Sample Code Only
*
* NOTE: This code comes with no guarantees, expressed or implied, and is meant to
* only show examples to get you started with your projects.
*******************************************************************************************

DESCRIPTION:

	Demonstrates the low power modes of the KL25Z128. 

TESTED ON:

	- FRDM-KL25Z

	- TWR-KL25Z48M

	- IAR EWARM 6.50.6

	- PE Micro DEBUG-APP_Pemicro_v106.SDA

	- Memory Targets:

		FLASH_128KB
		FLASH_64KB
		FLASH_32KB

REQUIREMENTS:

	- FRDM KL26Z development board running PEmicro Debug Application (See OPENSDAUG.pdf for instructions to install debug app.)

	- Terminal application. e.g., Hercules, PuTTY, etc.

	- Mini USB cable for OpenSDA debugging & powering the FRDM KL26Z


SETUP:

	Terminal:
		- Open up Device Manager to check COM port name of 'OpenSDA - CDC Serial port' & 'Freescale CDC Device'. e.g., COM1, COM2, etc..
		- Baudrate: 	19200 (Check 'freedom.h' for 'TERMINAL_BAUD' for baudrate setting.)
		- Data size: 	8 bits
		- Parity:	none
		- Handshake:	OFF	(Called 'Flow control' in PuTTY, set to 'None'.)

	USB CDC driver:
		- Download project to FRDM board, or begin a debug session.
		- Device Manager and find 'USB CDC' device
		- Right click on 'USB CDC' device and click 'Update Driver Software...'
		- Choose 'Browse my computer for driver software'
		- Browse to '..\baremetal\src\projects\usb_device' folder and check 'Include subfolders'
		- Click next, and driver should install
		- Check Device Manager for 'Freescale CDC Device' and make note of the COM port.


BEHAVIOR:

		
	After downloading code to target, or running a debug session, the following should appear in the terminal application window (connected to OpenSDA - CDC port)
	after the target description:
	

LLWU configured pins PTC3/SCI1_RX/FTM0_CH2 is LLWU wakeup source
 LLWU configured modules as LLWU wakeup sources = 0x01,
*--------------D E B U G    D I S A B L E D------------------*
*------------------------------------------------------------*
*                    KL Low Power DEMO                       *
*                    May 23 2013 14:23:41                    *
*------------------------------------------------------------*
  in Run Mode  !   in PEE mode now at 48000000 Hz

Select the desired operation
0 for CASE 0: Enter VLLS0 with POR disabled (Very Low Leakage STOP 0) NO POR
1 for CASE 1: Enter VLLS0 with POR enabled (Very Low Leakage STOP 0) with POR
2 for CASE 2: Enter VLLS1 (Very Low Leakage STOP 1)
3 for CASE 3: Enter LLS with LPTMR 1 second wakeup loop (Low Leakage Stop)
4 for CASE 4: Enter VLLS3 (Very Low Leakage STOP 3)
5 for CASE 5: Enter LLS(Low Leakage Stop)
6 for CASE 6: Enter VLPS(Very Low Power Stop)
7 for CASE 7: Enter VLPR(Very Low Power RUN) in BLPE (8 MHz Crystal)
8 for CASE 8: Exit VLPR(Very Low Power RUN)
9 for CASE 9: Enter VLPW(Very Low Power WAIT)
A for CASE 10: Enter WAIT from RUN or VLPW from VLPR
B for CASE 11: Enter Normal STOP from RUN or VLPS from VLPR
C for CASE 12: Enter PARTIAL STOP 1 with both system and bus clocks disabled
D for CASE 13: Enter PARTIAL STOP 2 with system clock disabled and bus clock enabled
E for CASE 14: Running coremark 2 times in RUN  with CPO then without CPO
F for CASE 15: Running coremark 2 times in VLPR with CPO then without CPO
G for CASE 16: Enable LPTMR to wakeup every 5 seconds from any mode except VLLS0
H for CASE 17: Disable LPTMR wakeup
I for CASE 18: Enter VLPR in BLPI at Core Frequency of 4 MHz
J for CASE 19: Enter VLPR in BLPI at Core Frequency of 2 MHz
K for CASE 20: Enter Compute Mode run for(i=0;i<wait_count;i++)
L for CASE 21: To enable DEBUG
 >

	Select the desired power mode from the menu displayed above.  Some selections may require additional steps for that example.  If this
        is the case, directions will be displayed on the terminal window.  The part will enter the selected power mode and allow you time to 
	measure the current draw in this mode.  	
		
ADDITIONAL NOTES:

	- Some selections may require EEMBC benchmark code, CoreMark, to run.  If you would like to use the CoreMark code in your demonstration,
	navigate to the CoreMark website, www.coremark.org, to dowload the software.  Once downloaded, simply add the CoreMark files to your 
	low power demo project to use these functions.  The specific CoreMark benchmark software that you will need to add are as follows:

		- core_list_join.c
		- core_main.c
		- core_matrix.c
		- core_state.c
		- core_util.c
		- coremark.h

	These files should be placed in the BenchMark folder that is already include in your sample code (baremetal\src\projects\low_power_demo\BenchMark).
	The file search paths for this folder are already configured in the project for each supported toolchain. The user must also #include "coremark.h"
        in the 'low_power_demo.c' file. Also, the following files should be included in the build:

		- core_portme.c
		- core_portme.h

	These can be found in the Kinetis folder of the project source folder. (..\baremetal\src\projects\low_power_demo\Kinetis).  
