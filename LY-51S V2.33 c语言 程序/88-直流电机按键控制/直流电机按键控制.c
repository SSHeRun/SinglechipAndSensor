/*-----------------------------------------------
  名称：单个独立按键控制直流电机开关
  论坛：www.doflye.net
  编写：shifang
  日期：2009.5
  修改：无
  内容：对应的电机接口需用杜邦线连接到uln2003电机控制端
        使用5V-12V 小功率电机皆可
------------------------------------------------*/
#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义

#define KeyPort P3

#define DataPort P0 //定义数据端口 程序中遇到DataPort 则用P0 替换
sbit LATCH1=P2^2;//定义锁存使能端口 段锁存
sbit LATCH2=P2^3;//                 位锁存

sbit DCOUT1 = P1^1;//定义电机信号输出端口1
sbit DCOUT2 = P1^2;//定义电机信号输出端口2

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// 显示段码值0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//分别对应相应的数码管点亮,即位码
unsigned char TempData[8]; //存储显示值的全局变量
/*------------------------------------------------
                  函数声明
------------------------------------------------*/
void DelayUs2x(unsigned char t);//us级延时函数声明 
void DelayMs(unsigned char t); //ms级延时
void Display(unsigned char FirstBit,unsigned char Num);//数码管显示函数
unsigned char KeyScan(void);//键盘扫描
void Init_Timer0(void);//定时器初始化
/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main (void)
{
unsigned char num;                  

Init_Timer0();    //初始化定时器0，主要用于数码管动态扫描

TempData[0]=0x5E; //'d'
TempData[1]=0x39; //'C'
TempData[2]=0x06; //'1'

TempData[5]=0x5E; //'d'
TempData[6]=0x39; //'C'
TempData[7]=0x5b; //'2'
while (1)         //主循环
  {

 
 num=KeyScan();    //循环调用按键扫描
 switch(num)
     {
      //电机1改变运行状态,数码管显示运行状态
	  case 1:DCOUT1=!DCOUT1;
	         TempData[0]=0x5E; //'d'
			 TempData[1]=0x39; //'C'
			 TempData[2]=0x06; //'1'

			 if(DCOUT1)
			   {
			 TempData[5]=0x3F; //'O'
			 TempData[6]=0x54; //'n'
			 TempData[7]=0;
			   }
			 else
			   {
			 TempData[5]=0x3F; //'O'
			 TempData[6]=0x71; //'F'
			 TempData[7]=0x71; //'F'
			   }
			 break;
	 //电机2改变运行状态,数码管显示运行状态
	  case 2:DCOUT2=!DCOUT2;
	         TempData[0]=0x5E; //'d'
			 TempData[1]=0x39; //'C'
			 TempData[2]=0x5B; //'2'

			 if(DCOUT2)
			   {
			 TempData[5]=0x3F; //'O'
			 TempData[6]=0x54; //'n'
			 TempData[7]=0;
			   }
			 else
			   {
			 TempData[5]=0x3F; //'O'
			 TempData[6]=0x71; //'F'
			 TempData[7]=0x71; //'F'
			   }
			   break;
	    default:break;
   }	
  }
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
 输入参数 FirstBit 表示需要显示的第一位，如赋值2表
 示从第三个数码管开始显示，如输入0表示从第一个显示。
 Num表示需要显示的位数，如需要显示99两位数值则该值
 输入2
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
 TH0=(65536-2000)/256;		  //重新赋值 2ms
 TL0=(65536-2000)%256;
 
 Display(0,8);       // 调用数码管扫描

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

