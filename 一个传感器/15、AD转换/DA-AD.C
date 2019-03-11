/**********************************************************************************
*   光敏电阻和热敏电阻试验                                                        *
*   1.通过本例程了解并掌握AD-DA转换芯片和热敏光敏电阻的基本原理和使用 			  *
*   2.了解掌握I2C总线接口的工作原理及一般编程方法。
*   
* 连接方法：参照接线图							                                  *
* 通过改变学习板上光敏和热敏电阻实现模拟输入，AD输入通道在第3和第4通道上
* 观察数码管的数字变化情况
* 用遮挡光敏电阻和有触摸热敏电阻可以看到AD的值发生变化		                      *
***********************************************************************************/

#include<reg52.h>
#include <I2C.H>

#define  PCF8591 0x90    //PCF8591 地址


//7段数码管工作选择 IO
sbit    LS138A=P2^0;  
sbit    LS138B=P2^1;
sbit    LS138C=P2^2;  

//此表为 LED 的字模, 共阴数码管 0-9  - 
unsigned char code Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f}; 

unsigned char AD_CHANNEL;
unsigned long xdata  LedOut[8];
unsigned int  D[32];
		 

/*******************************************************************
ADC发送字节[命令]数据函数               
*******************************************************************/
bit ISendByte(unsigned char sla,unsigned char c)
{
   Start_I2c();              //启动总线
   SendByte(sla);            //发送器件地址
   if(ack==0)return(0);
   SendByte(c);              //发送数据
   if(ack==0)return(0);
   Stop_I2c();               //结束总线
   return(1);
}

/*******************************************************************
ADC读字节数据函数               
*******************************************************************/
unsigned char IRcvByte(unsigned char sla)
{  unsigned char c;

   Start_I2c();          //启动总线
   SendByte(sla+1);      //发送器件地址
   if(ack==0)return(0);
   c=RcvByte();          //读取数据0

   Ack_I2c(1);           //发送非就答位
   Stop_I2c();           //结束总线
   return(c);
}

//******************************************************************/
main()
{  char i,j;

 while(1)
 {/********以下AD-DA处理*************/  
   switch(AD_CHANNEL)
   {
     case 0: ISendByte(PCF8591,0x41);
             D[0]=IRcvByte(PCF8591)*2;  //ADC0 模数转换1  放大2倍显示
			 break;  
 
	 case 1: ISendByte(PCF8591,0x42);
             D[1]=IRcvByte(PCF8591)*2;  //ADC1  模数转换2
			 break;  

	 case 2: ISendByte(PCF8591,0x43);
             D[2]=IRcvByte(PCF8591);  //ADC2	模数转换3
			 break;  

	 case 3: ISendByte(PCF8591,0x40);
             D[3]=IRcvByte(PCF8591);  //ADC3   模数转换4
			 break;  
	   }

  if(++AD_CHANNEL>3) AD_CHANNEL=0;

 /********以下将AD3 AD4采集的热名电阻和光敏电阻值送到LED数码管显示*************/
	   		
	 LedOut[0]=Disp_Tab[D[2]%10000/1000];
     LedOut[1]=Disp_Tab[D[2]%1000/100];
     LedOut[2]=Disp_Tab[D[2]%100/10]|0x80;
     LedOut[3]=Disp_Tab[D[2]%10];
	 
	 LedOut[4]=Disp_Tab[D[3]%10000/1000];
     LedOut[5]=Disp_Tab[D[3]%1000/100];
     LedOut[6]=Disp_Tab[D[3]%100/10]|0x80;
     LedOut[7]=Disp_Tab[D[3]%10];  
	  
	
	 for( i=0; i<8; i++) 
	 {	 P0 = LedOut[i];
			
	  switch(i)	  //使用switch 语句控制138译码器  也可以是用查表的方式 学员可以试着自己修改				  
	     {	    
			case 0:LS138A=0; LS138B=0; LS138C=0; break;         
	        case 1:LS138A=1; LS138B=0; LS138C=0; break;             	
	        case 2:LS138A=0; LS138B=1; LS138C=0; break; 
	        case 3:LS138A=1; LS138B=1; LS138C=0; break; 
			case 4:LS138A=0; LS138B=0; LS138C=1; break;
			case 5:LS138A=1; LS138B=0; LS138C=1; break;
			case 6:LS138A=0; LS138B=1; LS138C=1; break;
			case 7:LS138A=1; LS138B=1; LS138C=1; break;
			
	     }
	 
	     for (j = 0 ; j<90 ;j++) { ;}	   //扫描间隔时间
	  }

	    P0 = 0; 

 }  
}



