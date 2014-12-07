#ifndef DHT11_H
#define DHT11_H


#define MAXTIMINGS 85

#define DataPin PORTB->PCR[0];

#include "MKL25Z4.h"


typedef enum  
{
	IDLE,
	StartQuery,
	SendLow,
	SendHigh,
	BeginRecDataL,
	BeginRecDataH,
	RecHumi,
	RecHumiW,
} stat ;

typedef struct 
{
	int DataReady;
	int RecHumI;
	int RecHumD;
	int RecTI;
	int RecTD;
} DHT;


extern DHT dht;
extern void Initialize(void);
extern int GetTemp(void);
extern void CtrlRelay(char );
extern void ReleaseRelay (char );
extern void InitialSonic(void);
extern float SonicDistance(void);
extern void DistanceSet(float Threshold);
extern int GetInfrared(void);

#endif
