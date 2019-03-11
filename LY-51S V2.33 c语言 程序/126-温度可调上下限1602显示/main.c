/*-----------------------------------------------
  名称：DS18b20 温度检测液晶显示
  论坛：www.doflye.net
  编写：shifang
  日期：2009.5
  修改：无
  内容：
------------------------------------------------*/
#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
#include<stdio.h>
#include "18b20.h"
#include "1602.h"
#include "delay.h"

#define KeyPort P3 //定义按键端口

bit ReadTempFlag;//定义读时间标志

void Init_Timer0(void);//定时器初始化
unsigned char KeyScan(void);//键盘扫描
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
	TI=1;
}
/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main (void)
{                  
int temp,tempH=50,tempL=1;
float temperature;
unsigned char TempFlag=0;
char displaytemp[16],num;//定义显示区域临时存储数组

LCD_Init();           //初始化液晶
DelayMs(20);          //延时有助于稳定
LCD_Clear();          //清屏
Init_Timer0();
UART_Init();
Lcd_User_Chr();       //写入自定义字符


while (1)         //主循环
  {
 num=KeyScan();
 switch(num)
       {
	    case 1:if(tempH<127)tempH++;break;
		case 2:if(tempH>-55)tempH--;break;
		case 3:if(tempL<127)tempL++;break;
		case 4:if(tempL>-55)tempL--;break;
		default:break;
	   }
 
switch(TempFlag)
       {
	    case 0:    //输出温度上限下限
		      sprintf(displaytemp,"H.%3d  L.%3d ",tempH,tempL);
              LCD_Write_String(0,1,displaytemp);//显示第二行
			  break;
		case 1:LCD_Write_String(0,1,"over  tempH    ");break;
		case 2:LCD_Write_String(0,1,"under  tempL   ");break;
		default:break;
	   }
if(ReadTempFlag==1)
 {
  ReadTempFlag=0;
  temp=ReadTemperature();


  temperature=temp*0.0625;

  temp>>=4;
 if(temp>tempH)
    TempFlag=1;  //高于高温标志
 else if(temp<tempL)
    TempFlag=2;  //低于低温标志
 else
   TempFlag=0;   //正常显示标志

    

  sprintf(displaytemp,"Temp  %6.2f ",temperature);//打印温度值
  LCD_Write_String(0,0,displaytemp);//显示第一行
  LCD_Write_Char(13,0,0x01);//写入温度右上角点
  LCD_Write_Char(14,0,'C'); //写入字符C
  }	
 }
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
 if(num==400)        //
   {
    num=0;
    ReadTempFlag=1; //读标志位置1
	}
}

/*------------------------------------------------
按键扫描函数，返回扫描键值
------------------------------------------------*/
unsigned char KeyScan(void)
{
 unsigned char keyvalue;
 if(KeyPort!=0xff)
   {
    DelayMs(10);
    if(KeyPort!=0xff)
	   {
	    keyvalue=KeyPort;
	    while(KeyPort!=0xff);
		switch(keyvalue)
		{
		 case 0xfe:return 1;break;
		 case 0xfd:return 2;break;
		 case 0xfb:return 3;break;
		 case 0xf7:return 4;break;
		 case 0xef:return 5;break;
		 case 0xdf:return 6;break;
		 case 0xbf:return 7;break;
		 case 0x7f:return 8;break;
		 default:return 0;break;
		}
	  }
   }
   return 0;
}


