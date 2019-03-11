/*******************************************************************************
 标题：                  EEPROM读写试验(按钮输入)							   *
********************************************************************************
																			   *
此例熟悉并掌握（存储芯片AT2402）的操作，有保存和读取操作 	                   *			   
了解掌握I2C总线接口的工作原理及一般编程方法。								   *
                                                                               *		  
跳线操作：P0（JP10）连 数码管（JP12）										   *
          P1（JP8 ）连 独立按键（JP5）										   *
																			   *
具体操作：	第1个按钮保存      第2个按钮读取							       *
            第3个按钮减减      第4个按钮加数							       *
																			   *		  	   							  
********************************************************************************/

#include<Function.h>
#include<reg52.h>

#define  AT24C02 0xa0  //AT24C02 地址

//else IO
sbit    LS138A=P2^0;  	//译码器端
sbit    LS138B=P2^1;
sbit    LS138C=P2^2; 

sbit    K1 = P3^2;	 //保存
sbit    K2 = P3^3;	 //读取
sbit    K3 = P3^4;	 //+数据
sbit    K4 = P3^5;   //-数据

void delay(unsigned int i);


//此表为 LED 的字模, 共阴数码管 0-9  - 
unsigned char code Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40}; 

unsigned char Count1;
unsigned long D[16], LedOut[5],LedNumVal;
unsigned int idata USEC;

  
void  system_ini() 
{    
    TMOD|= 0x11;
    TH1 = 0xfe; //11.0592
	TL1 = 0x33;
	TR1  = 1; 
    IE =0x8A;

}

 main()
 { 	unsigned char i;
	unsigned char pDat[8];

   system_ini();
   while(1)
   {
		 //========================IIC 读取
	if(K2 == 0)	   //第二个按钮读取数据
	{ 
	  IRcvStr(AT24C02, 0 , &pDat[0], 8); 
	  for (i=0; i<4; i++)
	  {
		D[14+i]=pDat[i*2+0]+pDat[i*2+1]*0x100;
		Count1 = D[14];
	   }	     
	 }

	   //========================IIC 保存
	   if(K1 == 0)    //第一个按钮保存数据
	   { 
	      D[14]= Count1;
		 for (i=0; i<4; i++)
	     {
		   pDat[i*2+0]=D[14+i];
		   pDat[i*2+1]=D[14+i]>>8;
		 }
	     ISendStr(AT24C02, 0 , &pDat[0], 8); 
       }


	  /********以下将2402中保存的数据送到LED数码管显示*************/
        LedNumVal=Count1;
	  LedOut[0]=Disp_Tab[LedNumVal%10000/1000];
      LedOut[1]=Disp_Tab[LedNumVal%1000/100];
      LedOut[2]=Disp_Tab[LedNumVal%100/10]|0x80;
      LedOut[3]=Disp_Tab[LedNumVal%10]; 
  	  
    for(i=0; i<4; i++) 
	  {		
		
	    P0 =  LedOut[i];
	   	
		switch(i)					  
         {	    
			case 0:LS138A=0; LS138B=0; LS138C=0; break;         
            case 1:LS138A=1; LS138B=0; LS138C=0; break;             	
            case 2:LS138A=0; LS138B=1; LS138C=0; break; 
            case 3:LS138A=1; LS138B=1; LS138C=0; break; 
			case 4:LS138A=0; LS138B=0; LS138C=1; break;
         }
		 
		delay(100);
	  }

	  //  LS138A=1; LS138B=1; LS138C=1;    

   }
}


void delay(unsigned int i)
{
    char j;
    for(i; i > 0; i--)
        for(j = 200; j > 0; j--);
}


/*************************************
 [ t1 (0.5ms)中断] 中断中做 PWM 输出
  ------------1000/(0.02ms*250)=200Hz
*************************************/
void T1zd(void) interrupt 3   //3 为定时器1的中断号  1 定时器0的中断号 0 外部中断1 2 外部中断2  4 串口中断
{
	 TH1 = 0xfe; //12M	
	 TL1 = 0x33;
   if(USEC++==200)
   {	USEC=0;
	 
    if (K3) Count1++;     //改变数据
    if (K4&Count1!=0) Count1--;
  
    }  

}


