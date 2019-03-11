/*实验名称：字符型LCD接口实验 */
#include <reg51.h>
#include  <math.h>    //Keil library  
#include  <stdio.h>   //Keil library	
#include  <INTRINS.H>

sbit E=P2^3;
sbit RW=P2^4;
sbit RS=P2^5;

#define LCM_Data  P0

#define   uchar unsigned char


//延时设置
void Delay(unsigned int t)   // delay 40us
{
 for(;t!=0;t--) ;
}

//写指令
void WriteCommandLCM(unsigned char ch)
{
   RS=0;
   RW=0;
   E=1;
   LCM_Data=ch;
   Delay(1);
   E=0;
   Delay(100);  //delay 40us
}

//写数据
void WriteDataLCM(unsigned char ch)
{  RS=1;
   RW=0;
   E=1;
   LCM_Data=ch;
   Delay(1);
   E=0;
   Delay(100); //delay 40us
}
					

//LCD初始化
void InitLcd()
{WriteCommandLCM(0x30);		 //功能设置，设置8位数据接口，
 WriteCommandLCM(0x30);
 WriteCommandLCM(0x30);
 WriteCommandLCM(0x38);	//设置工作方式,8位数据接口，两行显示，5*7点阵
 WriteCommandLCM(0x08);	//关显示，关闭显示，关闭光标，关闭闪烁
 WriteCommandLCM(0x0c); //显示状态设置，开显示，关闭光标与闪烁
 WriteCommandLCM(0x01); //清屏
 WriteCommandLCM(0x06); //输入方式设置 ,AC自动增一，画面不动
}

//=============================================
void DisplayMsg1(uchar *p)
{ 
 unsigned char count;
 WriteCommandLCM(0x80);  //设置DDRAM地址
 for(count=0;count<16;count++)
    {WriteDataLCM(*p++);
   	}
} 
//=============================================
void DisplayMsg2(uchar *p)
{ 
 unsigned char count;
 WriteCommandLCM(0xc0);  //设置DDRAM地址
 for(count=0;count<16;count++)
    {WriteDataLCM(*p++);
   	}
}

//=============================================
main()
{
 char msg1[16]="   STC89C52RD   ";
 char msg2[16]="  LCD1602 DEMO  ";
 InitLcd();
 DisplayMsg1(msg1);
 DisplayMsg2(msg2);
 while(1);	 
}
