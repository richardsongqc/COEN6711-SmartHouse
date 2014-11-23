#include <string.h>
#include "uart.h"
#include "SerialProtocol.h"


U8 szCmdBuf[256] = {0};

int SendBuffer()
{
	int nRet = 0;
	U8 len = szCmdBuf[1] + sizeof(U8)* 2;
	U8 i = 0;
	for( ; i < len; i ++ )
	{
		uart0_putchar(UART0_BASE_PTR,(char)szCmdBuf[i]);
		nRet++;
	}
	
	return nRet;
}

int SendPIR(U8 uOn)						// ON, OFF of PIR
{
	memset( szCmdBuf, 0, sizeof(szCmdBuf));
	szCmdBuf[0] = SENSOR_PIR;
	szCmdBuf[1] = sizeof(U8);
	szCmdBuf[2] = uOn;
	
	return SendBuffer();
}

int SendUltrasonicRange(U16 usDistance) 	// Distance - cm (2-400)
{
	memset( szCmdBuf, 0, sizeof(szCmdBuf));
	szCmdBuf[0] = SENSOR_ULTRASONIC_RANGE;
	szCmdBuf[1] = sizeof(U16);
	szCmdBuf[2] = (usDistance>>8) & 0xFF;
	szCmdBuf[3] = usDistance & 0xFF;
	
	return SendBuffer();
}


int SendRelay1(U8 uOn) 					// ON, OFF of Relay1
{
	memset( szCmdBuf, 0, sizeof(szCmdBuf));
	szCmdBuf[0] = SENSOR_RELAY_1;
	szCmdBuf[1] = sizeof(U8);
	szCmdBuf[2] = uOn;
	
	return SendBuffer();
}

int SendRelay2(U8 uOn) 					// ON, OFF of Relay2
{
	memset( szCmdBuf, 0, sizeof(szCmdBuf));
	szCmdBuf[0] = SENSOR_RELAY_2;
	szCmdBuf[1] = sizeof(U8);
	szCmdBuf[2] = uOn;
	
	return SendBuffer();
}

int SendDHT(							 
	U8 uHiHumid,								// High Humid
	U8 uLoHumid,								// Low Humid
	U8 uHiTemp ,								// High Humid
	U8 uLoTemp ) 								// Low Humid
{
	memset( szCmdBuf, 0, sizeof(szCmdBuf));
	szCmdBuf[0] = SENSOR_DHT;
	szCmdBuf[1] = sizeof(U8)*4; 
	szCmdBuf[2] = uHiHumid;
	szCmdBuf[3] = uLoHumid;
	szCmdBuf[4] = uHiTemp ;
	szCmdBuf[5] = uLoTemp ;
	
	return SendBuffer();
}
