#include<reg52.h>  	 
#define uchar unsigned char
#define uint unsigned int  

sbit LED=P1^0;	 
sbit DOUT=P2^0;	 

void delay()
{
uchar m,n,s;
for(m=20;m>0;m--)
for(n=20;n>0;n--)
for(s=248;s>0;s--);
}

void main()
{
		DOUT=1;
	LED=0;
	while(1) 
	{
	 
	if(DOUT==0)
	 {
		 delay();

		 if(DOUT==0)
	    {
		   LED=1;	   
			 delay();
			 DOUT=1;
			 LED=0;
		 }
 }
		
	}
}	