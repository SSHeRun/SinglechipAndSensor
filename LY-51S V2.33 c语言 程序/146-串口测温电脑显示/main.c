/*-----------------------------------------------
  名称：DS18b20数码管显示 
  论坛：www.doflye.net
  编写：shifang
  日期：2009.5
  修改：无
  内容：显示格式 符号 xxx.x C 
        可以显示负温度
        数码管显示同时通过串口发送到电脑端显示
        使用dofly配套的测温软件 晶振使用11.0592M
------------------------------------------------*/
#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
#include<stdio.h>
#include "18b20.h"

#define DataPort P0 //定义数据端口 程序中遇到DataPort 则用P0 替换

sbit LATCH1=P2^2;//定义锁存使能端口 段锁存
sbit LATCH2=P2^3;//                 位锁存

bit ReadTempFlag;//定义读时间标志

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// 显示段码值0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//分别对应相应的数码管点亮,即位码
unsigned char TempData[8]; //存储显示值的全局变量


void Display(unsigned char FirstBit,unsigned char Num);//数码管显示函数
void Init_Timer0(void);//定时器初始化
/*------------------------------------------------
              串口通讯初始化
------------------------------------------------*/
void UART_Init(void)
{
    SCON  = 0x50;		        // SCON: 模式 1, 8-bit UART, 使能接收  
    TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit 重装
    TH1   = 0xFD;               // TH1:  重装值 9600 波特率 晶振 11.0592MHz  
    TR1   = 1;                  // TR1:  timer 1 打开                         
    //EA    = 1;                  //打开总中断
    //ES    = 1;                  //打开串口中断
	TI =1;
}
/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main (void)
{                  
unsigned int TempH,TempL,temp;
Init_Timer0();
UART_Init();
while (1)         //主循环
  {


if(ReadTempFlag==1)
{
  ReadTempFlag=0;
  temp=ReadTemperature();
  if(temp&0x8000)
     {
     TempData[0]=0x40;//负号标志
     temp=~temp;  // 取反加1
	 temp +=1;
	 }
  else
     TempData[0]=0;

     TempH=temp>>4;
     TempL=temp&0x0F;
     TempL=TempL*6/10;//小数近似处理
     printf("%d.%d\n",TempH,TempL);	
    if(TempH/100==0)
	  TempData[1]=0;
	else
      TempData[1]=dofly_DuanMa[TempH/100]; //十位温度
    if((TempH/100==0)&&((TempH%100)/10==0))//消隐
	  TempData[2]=0;
	else
     TempData[2]=dofly_DuanMa[(TempH%100)/10]; //十位温度
     TempData[3]=dofly_DuanMa[(TempH%100)%10]|0x80; //个位温度,带小数点
     TempData[4]=dofly_DuanMa[TempL];
	 TempData[6]=0x39;         //显示C符号
  }	
 }
}

/*------------------------------------------------
 显示函数，用于动态扫描数码管
 输入参数 FirstBit 表示需要显示的第一位，如赋值2表示从第三个数码管开始显示
 如输入0表示从第一个显示。
 Num表示需要显示的位数，如需要显示99两位数值则该值输入2
------------------------------------------------*/
void Display(unsigned char FirstBit,unsigned char Num)
{
      static unsigned char i=0;
	  

	   DataPort=0;   //清空数据，防止有交替重影
       LATCH1=1;     //段锁存
       LATCH1=0;

       DataPort=dofly_WeiMa[i+FirstBit]; //取位码 
       LATCH2=1;     //位锁存
       LATCH2=0;

       DataPort=TempData[i]; //取显示数据，段码
       LATCH1=1;     //段锁存
       LATCH1=0;
       
	   i++;
       if(i==Num)
	      i=0;


}
/*------------------------------------------------
                    定时器初始化子程序
------------------------------------------------*/
void Init_Timer0(void)
{
 TMOD |= 0x01;	  //使用模式1，16位定时器，使用"|"符号可以在使用多个定时器时不受影响		     
 //TH0=0x00;	      //给定初值
 //TL0=0x00;
 EA=1;            //总中断打开
 ET0=1;           //定时器中断打开
 TR0=1;           //定时器开关打开
}
/*------------------------------------------------
                 定时器中断子程序
------------------------------------------------*/
void Timer0_isr(void) interrupt 1 
{
 static unsigned int num;
 TH0=(65536-2000)/256;		  //重新赋值 2ms
 TL0=(65536-2000)%256;
 
 Display(0,8);       // 调用数码管扫描
 num++;
 if(num==300)        //
   {
    num=0;
    ReadTempFlag=1; //读标志位置1
	}
}


