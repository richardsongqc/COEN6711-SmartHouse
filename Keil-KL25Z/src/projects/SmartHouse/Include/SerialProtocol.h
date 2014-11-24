#pragma once

#ifndef __SERIAL_PROTOCOL_H__
#define __SERIAL_PROTOCOL_H__

typedef unsigned char U8;
typedef signed char   S8;
typedef unsigned int  U16;
typedef signed int	  S16;
typedef unsigned long U32;
typedef signed long   S32;

// Command ( KL25Z -> PC )
#define CMD_PIR					0x01		// PIR
#define CMD_ULTRASONIC_RANGE	0x02		// Ultrasonic Ranging Module HC - SR04
#define CMD_RELAY_1				0x03		// Relay 1
#define CMD_RELAY_2				0x04		// Relay 2
#define CMD_DHT					0x05		// Digital Humid & Temperature

// Response ( PC -> KL25Z )
#define RSP_PIR					0x81		// PIR
#define RSP_ULTRASONIC_RANGE	0x82		// Ultrasonic Ranging Module HC - SR04
#define RSP_RELAY_1				0x83		// Relay 1
#define RSP_RELAY_2				0x84		// Relay 2
#define RSP_DHT					0x85		// Digital Humid & Temperature

// Response Code
#define CMD_OK				 	0x8000
#define ERR_CMD				 	0x8001
#define ERR_CMD_FORMAT			0x8002
#define ERR_NO_RESPONSE			0x8003

// Send Message from KL25Z board to PC via Serial Port
extern U16 SendPIR(U8 uOn);						// ON, OFF of PIR
extern U16 SendUltrasonicRange(U16 usDistance);	// Distance - cm (2-400)
extern U16 SendRelay1(U8 uOn);					// ON, OFF of Relay1
extern U16 SendRelay2(U8 uOn);					// ON, OFF of Relay2
extern U16 SendDHT(							 
	U8 uHiHumid,								// High Humid
	U8 uLoHumid,								// Low Humid
	U8 uHiTemp,									// High Humid
	U8 uLoTemp);								// Low Humid

// Get Message from PC to KL25Z 
extern U16 RecvBuffer( U8* uCmd );
extern U16 GetUltrasonicRange(U16* usDistance);
extern U16 GetRelay1( U8* uOn );
extern U16 GetRelay2( U8* uOn );
extern U16 GetDHT( 
	U8* uHiHumid,
	U8* uLoHumid,
	U8* uHiTemp,	
	U8* uLoTemp);

	
	
#endif //__SMART_HOUSE_H__
