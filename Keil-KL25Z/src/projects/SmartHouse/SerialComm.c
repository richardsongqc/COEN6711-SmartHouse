#include <string.h>
#include "uart.h"
#include "SerialProtocol.h"


U8 szCmdBuf[256] = {0};
U8 szRepBuf[256] = {0};

U16 SendBuffer()
{
	U16 usRet = 0;
	U8 len = szCmdBuf[1] + sizeof(U8)* 2;
	U8 i = 0;
	for( ; i < len; i ++ )
	{
		uart0_putchar(UART0_BASE_PTR,(char)szCmdBuf[i]);
	}
	
	if ( uart0_getchar_present(UART0_BASE_PTR) == 0 )
	{
		return ERR_NO_RESPONSE;
	}
	
	memset( szRepBuf, 0, sizeof(szRepBuf) );
	szRepBuf[0] = (U8)uart0_getchar(UART0_BASE_PTR);
	if( szRepBuf[0] != (szCmdBuf[0] | 0x80 ) )
	{
		return ERR_CMD;
	}
	
	for( i = 2; i < szRepBuf[1]; i ++ )
	{
		szRepBuf[i] = (U8)uart0_getchar(UART0_BASE_PTR);
	}
	
	usRet  = szRepBuf[2];
	usRet <<= 8;
	usRet |= szRepBuf[3];
	
	return usRet;
}

U16 RecvBuffer( U8* uCmd )
{
	U8 i = 0;
	
	if( uart0_getchar_present(UART0_BASE_PTR) == 0 )
	{
		*uCmd = 0;
		return 0;
	}
	
	memset( szRepBuf, 0, sizeof(szRepBuf) );
	
	szRepBuf[0]  = (U8)uart0_getchar(UART0_BASE_PTR);
	*uCmd = szRepBuf[0];
	if( szRepBuf[0] > 10 || szRepBuf[0] == 0 )
	{
		return ERR_CMD_FORMAT;
	}
	
	szRepBuf[1]  = (U8)uart0_getchar(UART0_BASE_PTR);
	
	for( i = 2; i < szRepBuf[1]; i ++ )
	{
		szRepBuf[i] = (U8)uart0_getchar(UART0_BASE_PTR);
	}
	
	return 0;
}



U16 SendPIR(U8 uOn)							// ON, OFF of PIR
{
	memset( szCmdBuf, 0, sizeof(szCmdBuf));
	szCmdBuf[0] = CMD_PIR;
	szCmdBuf[1] = sizeof(U8);
	szCmdBuf[2] = uOn;
	
	return SendBuffer();
}

U16 SendUltrasonicRange(U16 usDistance) 	// Distance - cm (2-400)
{
	memset( szCmdBuf, 0, sizeof(szCmdBuf));
	szCmdBuf[0] = CMD_ULTRASONIC_RANGE;
	szCmdBuf[1] = sizeof(U16);
	szCmdBuf[2] = (usDistance>>8) & 0xFF;
	szCmdBuf[3] = usDistance & 0xFF;
	
	return SendBuffer();
}


U16 SendRelay1(U8 uOn) 						// ON, OFF of Relay1
{
	memset( szCmdBuf, 0, sizeof(szCmdBuf));
	szCmdBuf[0] = CMD_RELAY_1;
	szCmdBuf[1] = sizeof(U8);
	szCmdBuf[2] = uOn;
	
	return SendBuffer();
}

U16 SendRelay2(U8 uOn) 						// ON, OFF of Relay2
{
	memset( szCmdBuf, 0, sizeof(szCmdBuf));
	szCmdBuf[0] = CMD_RELAY_2;
	szCmdBuf[1] = sizeof(U8);
	szCmdBuf[2] = uOn;
	
	return SendBuffer();
}

U16 SendDHT(							 
	U8 uHiHumid,							// High Humid
	U8 uLoHumid,							// Low Humid
	U8 uHiTemp ,							// High Humid
	U8 uLoTemp ) 							// Low Humid
{
	memset( szCmdBuf, 0, sizeof(szCmdBuf));
	szCmdBuf[0] = CMD_DHT;
	szCmdBuf[1] = sizeof(U8)*4; 
	szCmdBuf[2] = uHiHumid;
	szCmdBuf[3] = uLoHumid;
	szCmdBuf[4] = uHiTemp ;
	szCmdBuf[5] = uLoTemp ;
	
	return SendBuffer();
}


U16 GetDHT( 
	U8* uHiHumid,
	U8* uLoHumid,
	U8* uHiTemp,	
	U8* uLoTemp)
{
	*uHiHumid = szRepBuf[2] ;
	*uLoHumid = szRepBuf[3] ;
	*uHiTemp  = szRepBuf[4] ;
	*uLoTemp  = szRepBuf[5] ;
	
	return 0;
}
	
U16 GetRelay1( U8* uOn )
{
	*uOn = szRepBuf[2];
	
	return 0;
}

U16 GetRelay2( U8* uOn )
{
	*uOn = szRepBuf[2];
	
	return 0;
}

U16 GetUltrasonicRange(U16* usDistance)
{
	*usDistance = szRepBuf[2];
	*usDistance <<= 8;
	*usDistance |= szRepBuf[3];
	
	return 0;
}

