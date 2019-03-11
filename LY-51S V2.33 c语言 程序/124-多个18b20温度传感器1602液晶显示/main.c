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
#include "18b20a.h"
#include "1602.h"
#include "delay.h"

bit ReadTempFlag;//定义读时间标志

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
	TI=1;
}
/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main (void)
{                  
int temp;
float temperature;
char displaytemp[16];//定义显示区域临时存储数组

LCD_Init();           //初始化液晶
DelayMs(20);          //延时有助于稳定
LCD_Clear();          //清屏
Init_Timer0();
UART_Init();
Lcd_User_Chr();       //写入自定义字符

LCD_Write_Char(13,0,0x01);//写入温度右上角点
LCD_Write_Char(14,0,'C'); //写入字符C

LCD_Write_Char(13,1,0x01);//写入温度右上角点
LCD_Write_Char(14,1,'C'); //写入字符C

while (1)         //主循环
  {

if(ReadTempFlag==1)
 {
  ReadTempFlag=0;
  temp=ReadTemperature();
  temperature=(float)temp*0.0625;
  sprintf(displaytemp,"Temp1 %7.3f",temperature);//打印温度值
  LCD_Write_String(0,0,displaytemp);//显示第二行

  temp=ReadTemperature_a();
  temperature=(float)temp*0.0625;
  sprintf(displaytemp,"Temp2 %7.3f",temperature);//打印温度值
  LCD_Write_String(0,1,displaytemp);//显示第二行
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
 if(num==600)        //
   {
    num=0;
    ReadTempFlag=1; //读标志位置1
	}
}


