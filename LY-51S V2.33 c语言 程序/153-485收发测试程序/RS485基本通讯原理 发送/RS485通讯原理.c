 
/*-----------------------------------------------
  名称：串口通信 rs485
  网站：www.doflye.net
  编写：shifang
  日期：2009.5
  修改：无
  内容：485 与 232 使用相同软件协议，是半双工，需要有2套开发板对发测试，或者使用
        另外一个485设备配套，这里提供测试程序
        发送端接线方式如下：
        P2.0----E(J40) 收发控制端
        TDX ----DI(J40)数据接收 针对485而言
        RXD ----RO(J40)数据发送 针对485而言

        A，B分别连接另一块开发板的 A，B

        发送端使用独立按键输入：
        P1 ----- J26 独立按键
------------------------------------------------*/

#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义                        
#include"delay.h"

unsigned char KeyScan(void);
void SendByte(unsigned char dat);
/*------------------------------------------------
                硬件端口定义
------------------------------------------------*/
#define KeyPort P1

sbit Ctrl_EN = P2^0;  //发送接收控制端
/*------------------------------------------------
                 函数声明
------------------------------------------------*/
void SendStr(unsigned char *s);
/*------------------------------------------------
                串口初始化
------------------------------------------------*/
void InitUART  (void)
{

    SCON  = 0x50;		        // SCON: 模式 1, 8-bit UART, 使能接收  
    TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit 重装
    TH1   = 0xFD;               // TH1:  重装值 9600 波特率 晶振 11.0592MHz  
    TR1   = 1;                  // TR1:  timer 1 打开                         
    EA    = 1;                  //打开总中断
    //ES    = 1;                  //打开串口中断
}                            
/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main (void)
{
unsigned char keynum;
InitUART();

Ctrl_EN=1;  //发送模式
//SendStr("UART test，技术论坛：www.doflye.net thank you!");
while (1)                       
    {
    keynum= KeyScan();
    if(keynum!=0)
       SendByte(keynum);

    }
}

/*------------------------------------------------
                    发送一个字节
------------------------------------------------*/
void SendByte(unsigned char dat)
{
 SBUF = dat;
 while(!TI);
      TI = 0;
}
/*------------------------------------------------
                    发送一个字符串
------------------------------------------------*/
void SendStr(unsigned char *s)
{
 while(*s!='\0')// \0 表示字符串结束标志，通过检测是否字符串末尾
  {
  SendByte(*s);
  s++;
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

