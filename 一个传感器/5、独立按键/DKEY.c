/******************************************************************/
/* 	      按键状态显示试验										  *
/*	             												  *
/*  连接方法：连接JP10（P0)与JP5（按钮接口)          			  *
/*			      JP11 (P2)与JP1(LED接口) 						  *
																  *
/*按键则点亮LED灯，8路指示灯接p0口         						  *
/******************************************************************/
#include <reg51.h>
#define uchar unsigned char
#define uint  unsigned int

sbit  K1 = P1^0; 
sbit  K2 = P1^1;
sbit  K3 = P1^2; 
sbit  K4 = P1^3;
sbit  K5 = P1^4; 
sbit  K6 = P1^5;
sbit  K7 = P1^6; 
sbit  K8 = P1^7; 

void delay(uint ms) 
{
   uchar t;
   while(ms--)
   { 
     for(t = 0; t < 120; t++);
   }
}
/*********************************************************/
main()
{
   P2 = 0x07;	
   P0 = 0x3f;
   while(1)
   {

   	  if(K1==0)
   	  P0 = 0x06;


   	  if(K2 == 0)
   	  P0 = 0x5b;


   	  if(K3==0)
   	  P0 = 0x4f;


   	  if(K4==0)
   	  P0 = 0x66;

	  	  
	  if (K5==0 )
	    P0 = 0x6d;


	  if (K6==0)
	   P0 = 0x7d;

	   if (K7==0)
	   P0 = 0x07;

	   if (K8==0)
	   P0 = 0x7f;

	   delay(15);	//按键消抖
   }
}