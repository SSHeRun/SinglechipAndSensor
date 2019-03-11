/*-----------------------------------------------
  名称：DS1302时钟数码管显示 串口更新
  论坛：www.doflye.net
  编写：shifang
  日期：2009.5
  修改：无
  内容：液晶显示时间
		通过dofly自带的串口调试软件，打开串口，波特率默认9600，点击更新时间即可，如果不行，按下开发板复位重新更新
------------------------------------------------*/
#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
#include <stdio.h>
#include "ds1302.h"
#include "delay.h"
#include "1602.h"

bit ReadTimeFlag;//定义读时间标志
bit SetFlag;     //更新时间标志位
unsigned char time_buf2[16];

void Init_Timer0(void);//定时器初始化
void UART_Init(void);
/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main (void)
{
unsigned char i;                  
unsigned char temp[16];//定义显示区域临时存储数组

LCD_Init();           //初始化液晶
DelayMs(20);          //延时有助于稳定
LCD_Clear();          //清屏
Init_Timer0();        //定时器0初始化
Ds1302_Init();        //ds1302初始化
UART_Init();          //串口初始化

Ds1302_Read_Time();   //首次读取时间
if((time_buf1[2]+time_buf1[7])==0) //如果所有参数都为0，写入一个初始值
   Ds1302_Write_Time();    
while (1)         //主循环
  {


if(SetFlag)     //如果接收到串口信息则更新时钟
  {
	for(i=0;i<8;i++)
		{
		time_buf1[i]=time_buf2[2*i]*10+time_buf2[2*i+1];//数据整合，如2个数 1和5整合成15
		}
		Ds1302_Write_Time();//接收更新的时间然后写入ds1302
		SetFlag=0;          //时钟信息更新后标志位清零
   }


if(ReadTimeFlag==1) //定时读取ds1302 定时时间到 则标志位置1，处理过时间参数标志位清零
{
  ReadTimeFlag=0;  //标志位清零
  Ds1302_Read_Time();//读取时间参数
  sprintf(temp,"DATE %02d-%02d-%02d %d",(int)time_buf1[1],(int)time_buf1[2],(int)time_buf1[3],(int)time_buf1[7]);//年月日周
  LCD_Write_String(0,0,temp);//显示第一行
  sprintf(temp,"TIME %02d:%02d:%02d",(int)time_buf1[4],(int)time_buf1[5],(int)time_buf1[6]);//时分秒
  LCD_Write_String(0,1,temp);//显示第二行
  }
 }
}
/*------------------------------------------------
              串口通讯初始化
------------------------------------------------*/
void UART_Init(void)
{
    SCON  = 0x50;		        // SCON: 模式 1, 8-bit UART, 使能接收  
    TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit 重装
    TH1   = 0xFD;               // TH1:  重装值 9600 波特率 晶振 11.0592MHz  
    TR1   = 1;                  // TR1:  timer 1 打开                         
    EA    = 1;                  //打开总中断
    ES    = 1;                  //打开串口中断
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
 
 num++;
 if(num==50)        //大致100ms
   {
    num=0;
    ReadTimeFlag=1; //读标志位置1
	}
}

/*------------------------------------------------
                  串口中断程序
------------------------------------------------*/
void UART_SER (void) interrupt 4 //串行中断服务程序
{
    unsigned char Temp;          //定义临时变量 
    unsigned char i;
    if(RI)                        //判断是接收中断产生
     {
	  RI=0;                      //标志位清零
	  Temp=SBUF;                 //读入缓冲区的值
	  time_buf2[i]=Temp&0x0F;
	  i++;
	  if(i==16)                  //连续接收16个字符信息
	   {
	    i=0;
		SetFlag=1;               //接收完成标志位置1
	   }
      SBUF=Temp; //把接收到的值再发回电脑端
	 }
   if(TI)  //如果是发送标志位，清零
     TI=0;
} 
