#ifndef DHT11_H
#define DHT11_H


#define MAXTIMINGS 85

#define DataPin PORTB->PCR[0];



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

extern void GetTemp(void);
extern void CtrlRelay(char Port1,char Port2);

#endif
