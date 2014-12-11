#include <stdint.h>
#include "MKL25Z4.h"

void delay(int n){
int i;
	for(i=1;i<n;i++){}
	}//delay

int main(void){
  SystemCoreClockUpdate();  //core clock setup
	SIM->SCGC5|=(1<<10);//enable clock to portb 
	PORTB->PCR[10]|=(1<<8);//make portb10 gpio
	PORTB->PCR[11]|=(1<<8);//MAKE PORTB11 GPIO
	PORTB->PCR[1]|=(1<<8);//MAKE PORT1 GPIO
	PTB->PDDR|=(1<<10);//make portB10 output, the input pin of the sensor
	PTB->PDDR&=~(1<<11);//make portb11 input, the output pin of the sensor
	PTB->PDDR|=(1<<1);//MAKE PORBT1 OUTPUT, the pin to enable the sensor or the process of the next state
	
	while(1){
		if(PTB->PDIR&(1<<11)){
		PTB->PDOR|=(1<<1);//sensor's output equals to 1 when a person's coming, then enable the next sensor
		delay(1000000000);}
	}
}