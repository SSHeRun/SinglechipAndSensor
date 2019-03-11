/*-----------------------------------------------
  名称：红绿灯
  论坛：www.doflye.net
  编写：shifang
  日期：2009.5
  修改：无
  内容：1、程序目的：使用定时器学习 倒计时 红绿灯原理 
           主要程序与倒计时相同
        2、硬件要求：数码管、晶振12M
		3、P1口连接至LED用于显示红绿灯
		4、数码管用于显示时间
------------------------------------------------*/
#include<reg52.h>//包含头文件，一般情况不需要改动，
//头文件包含特殊功能寄存器的定义

sbit LACTCH1=P2^2;    
sbit LACTCH2=P2^3;
/*------------------------------------------------
                 全局变量
------------------------------------------------*/
bit red,green,yellow,turnred;//定义红绿黄灯标志位



code unsigned char tab[]=
{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f}; 
			                //共阴数码管 0-9 

unsigned char Dis_Shiwei;//定义十位
unsigned char Dis_Gewei; //定义个位
/*------------------------------------------------
              
------------------------------------------------*/
void delay(unsigned int cnt)
{
 while(--cnt);
}
/*------------------------------------------------
              主程序
------------------------------------------------*/
main()
{
TMOD |=0x01;//定时器设置 10ms in 12M crystal
TH0=0xd8;
TL0=0xf0;
IE= 0x82;   //打开中断
TR0=1;

P1=0xfc;    //红灯亮
red =1;
while(1)
  {
  P0=Dis_Shiwei;//显示十位
  LACTCH1=1;    //锁存
  LACTCH1=0;
  P0=0xfe; 
  LACTCH2=1;    // 锁存
  LACTCH2=0;
  delay(300);   //短暂延时
  P0=Dis_Gewei; //显示个位
  LACTCH1=1;    //锁存
  LACTCH1=0;
  P0=0xfd;
  LACTCH2=1;    // 锁存
  LACTCH2=0;
  delay(300);
  }

}
/*------------------------------------------------
             定时器0中断函数
------------------------------------------------*/
void tim(void) interrupt 1 
{
static unsigned char second=60,count; //初值99

TH0=0xd8;//重新赋值
TL0=0xf0;
count++;
if (count==100)
    {
    count=0;
    second--;//秒减1
	if(second==0)
	   { 
       if(red)
	     {
		  red=0;yellow=1;
          second=5;
		  P1=0xF3;//黄灯亮5秒
		 }
	   else if(yellow && !turnred)
	     {
		  yellow=0;green=1;
          second=50;
		  P1=0xCF;//绿灯亮50秒
		 }
	   else if(green)
	     {
		 yellow=1;green=0;
         second=5;
		 P1=0xF3;//黄灯亮5秒
		 turnred=1;
		 }
       else if(yellow && turnred)
	     {
	     red=1;yellow=0;
		 P1=0xFC;//红灯亮60秒
         second=60;
		 turnred=0;
		 }

	   }
	Dis_Shiwei=tab[second/10];//十位显示值处理
    Dis_Gewei=tab[second%10]; //个位显示处理
    
    }
}