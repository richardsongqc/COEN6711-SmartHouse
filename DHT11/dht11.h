#ifndef DHT11_H
#define DHT11_H

/* DHT11 library 

written by Pingyang
*/
//

#define MAXTIMINGS 85

#define DataPin PORTB->PCR[0];



enum stat {
	IDLE,
	StartQuery,
	SendLow,
	SendHigh,
	BeginRecDataL,
	BeginRecDataH,
	RecHumi,
	RecHumiW,

}LineStat;

struct DHT{
	int DataReady;
	int RecHumI;
	int RecHumD;
	int RecTI;
	int RecTD;
}dht;

//void GetTemp();

#endif
