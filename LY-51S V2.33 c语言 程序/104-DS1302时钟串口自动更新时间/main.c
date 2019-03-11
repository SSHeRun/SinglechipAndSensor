/*-----------------------------------------------
  名称：DS1302时钟数码管显示 串口更新
  论坛：www.doflye.net
  编写：shifang
  日期：2009.5
  修改：无
  内容：DS1302实时时钟数码管显示，时间和日期切换显示 按下按键，循环切换 
        时间格式xx-xx-xx
		日期格式xx-xx-xx
		周秒格式-x-   xx
		通过dofly自带的串口调试软件，打开串口，波特率默认9600，点击更新时间即可，如果不行，按下开发板复位重新更新
------------------------------------------------*/
#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
#include "ds1302.h"

#define KeyPort P3 //定义按键端口

#define DataPort P0 //定义数据端口 程序中遇到DataPort 则用P0 替换

sbit LATCH1=P2^2;//定义锁存使能端口 段锁存
sbit LATCH2=P2^3;//                 位锁存

bit ReadTimeFlag;//定义读时间标志
bit SetFlag;     //更新时间标志位
unsigned char time_buf2[16];

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// 显示段码值0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//分别对应相应的数码管点亮,即位码
unsigned char TempData[8]; //存储显示值的全局变量

void DelayUs2x(unsigned char t);//us级延时函数声明 
void DelayMs(unsigned char t); //ms级延时
void Display(unsigned char FirstBit,unsigned char Num);//数码管显示函数
unsigned char KeyScan(void);//键盘扫描
void Init_Timer0(void);//定时器初始化
void UART_Init(void);
/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main (void)
{
unsigned char i,num,displaynum;                  

Init_Timer0();
Ds1302_Init();
UART_Init();
while (1)         //主循环
  {


if(SetFlag)     //如果接收到串口信息则更新时钟
  {
	for(i=0;i<8;i++)
		{
		time_buf1[i]=time_buf2[2*i]*10+time_buf2[2*i+1];
		}//数据整合，如2个数 1和5整合成15
		Ds1302_Write_Time();
		SetFlag=0;       //时钟信息更新后标志位清零
   }

 num=KeyScan();
if(num==1)
  {
   displaynum++;
   if(displaynum==3)
      displaynum=0;
  }
if(ReadTimeFlag==1)
{
  ReadTimeFlag=0;
  Ds1302_Read_Time();
if(displaynum==0) //显示时间
 {
 TempData[0]=dofly_DuanMa[time_buf1[4]/10];//时			//数据的转换，因我们采用数码管0~9的显示,将数据分开
 TempData[1]=dofly_DuanMa[time_buf1[4]%10];
 TempData[2]=0x40;					    	        //加入"-"
 TempData[3]=dofly_DuanMa[time_buf1[5]/10];//分
 TempData[4]=dofly_DuanMa[time_buf1[5]%10];
 TempData[5]=0x40;
 TempData[6]=dofly_DuanMa[time_buf1[6]/10];//秒
 TempData[7]=dofly_DuanMa[time_buf1[6]%10];	
 } 
else if(displaynum==1)//显示日期
 { 
 TempData[0]=dofly_DuanMa[time_buf1[1]/10];//年			//数据的转换，因我们采用数码管0~9的显示,将数据分开
 TempData[1]=dofly_DuanMa[time_buf1[1]%10];
 TempData[2]=0x40;					    	        //加入"-"
 TempData[3]=dofly_DuanMa[time_buf1[2]/10];//月
 TempData[4]=dofly_DuanMa[time_buf1[2]%10];
 TempData[5]=0x40;
 TempData[6]=dofly_DuanMa[time_buf1[3]/10];//日
 TempData[7]=dofly_DuanMa[time_buf1[3]%10];	
 }
else if(displaynum==2)//显示周  秒
 {
 TempData[0]=0x40;				//数据的转换，因我们采用数码管0~9的显示,将数据分开
 TempData[1]=dofly_DuanMa[time_buf1[7]%10];//周
 TempData[2]=0x40;	//加入"-"
 TempData[3]=0;
 TempData[4]=0;
 TempData[5]=0;
 TempData[6]=dofly_DuanMa[time_buf1[6]/10];//秒
 TempData[7]=dofly_DuanMa[time_buf1[6]%10];	
 }
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
 uS延时函数，含有输入参数 unsigned char t，无返回值
 unsigned char 是定义无符号字符变量，其值的范围是
 0~255 这里使用晶振12M，精确延时请使用汇编,大致延时
 长度如下 T=tx2+5 uS 
------------------------------------------------*/
void DelayUs2x(unsigned char t)
{   
 while(--t);
}
/*------------------------------------------------
 mS延时函数，含有输入参数 unsigned char t，无返回值
 unsigned char 是定义无符号字符变量，其值的范围是
 0~255 这里使用晶振12M，精确延时请使用汇编
------------------------------------------------*/
void DelayMs(unsigned char t)
{
     
 while(t--)
 {
     //大致延时1mS
     DelayUs2x(245);
	 DelayUs2x(245);
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
 if(num==50)        //大致100ms
   {
    num=0;
    ReadTimeFlag=1; //读标志位置1
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
