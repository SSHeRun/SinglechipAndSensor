#include<reg51.h>
#define uint unsigned int
void delay(uint xms);
sbit led1=P0^0;
void main(){
	 led1=0; //灯灭
	 IT0=0;//低电平有效
	 EA=1;	
	 EX0=1;
	 while(1){
	 if(led1==1)
			led1=0;
	 }
}

void externalInterruption() interrupt 0
{
	  led1 = ~led1;
		delay(100);
}

void delay(uint xms){
uint i,j;
	for(i=xms;i>0;i--)
		for(j=110;j>0;j--);
}


