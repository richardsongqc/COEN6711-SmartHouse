#pragma once

#ifndef __SERIAL_PROTOCOL_H__
#define __SERIAL_PROTOCOL_H__

typedef unsigned char U8;
typedef signed char   S8;
typedef unsigned int  U16;
typedef signed int	  S16;
typedef unsigned long U32;
typedef signed long   S32;

// Sensor Type
#define SENSOR_PIR					0x01		// PIR
#define SENSOR_ULTRASONIC_RANGE		0x02		// Ultrasonic Ranging Module HC - SR04
#define SENSOR_RELAY_1				0x03		// Relay 1
#define SENSOR_RELAY_2				0x04		// Relay 2
#define SENSOR_DHT					0x05		// Digital Humid & Temperature

// Send Message from KL25Z board to PC via Serial Port
extern int SendPIR(U8 uOn);						// ON, OFF of PIR
extern int SendUltrasonicRange(U16 usDistance);	// Distance - cm (2-400)
extern int SendRelay1(U8 uOn);					// ON, OFF of Relay1
extern int SendRelay2(U8 uOn);					// ON, OFF of Relay2
extern int SendDHT(							 
	U8 uHiHumid,								// High Humid
	U8 uLoHumid,								// Low Humid
	U8 uHiTemp,									// High Humid
	U8 uLoTemp);								// Low Humid

#endif //__SMART_HOUSE_H__
