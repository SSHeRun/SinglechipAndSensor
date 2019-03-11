/*-----------------------------------------------
  名称：数码管显示，按键控制秒表
  论坛：www.doflye.net
  编写：shifang
  日期：2009.5
  修改：无
  内容：中断按键控制，数码管显示,中断0控制计时和停止，中断1清零
------------------------------------------------*/
#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义

unsigned int msecond,second;//定义全局变量
bit GoFlag;//定义停止，计时标志

#define DataPort P0 //定义数据端口 程序中遇到DataPort 则用P0 替换
sbit LATCH1=P2^2;//定义锁存使能端口 段锁存
sbit LATCH2=P2^3;//                 位锁存

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// 显示段码值0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//分别对应相应的数码管点亮,即位码
unsigned char TempData[8]; //存储显示值的全局变量

void DelayUs2x(unsigned char t);//函数声明 
void DelayMs(unsigned char t);
void Display(unsigned char FirstBit,unsigned char Num);
void Init_Timer0(void);
void CLR(void);
/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main (void)
{
unsigned char num=0;                  

  
  EX0=1;         //外部中断0开
  IT0=1;         //IT1=0表示边沿触发
  EX1=1;         //外部中断1开
  IT1=1;         //IT1=1表示边沿触发
  Init_Timer0();
while (1)         //主循环
  {

	   TempData[0]=dofly_DuanMa[second/10];//分解显示信息，如要显示68，则68/10=6  68%10=8  
	   TempData[1]=dofly_DuanMa[second%10];
       TempData[2]=0x40;// "-"
	   TempData[3]=dofly_DuanMa[msecond/10];//分解显示信息，如要显示68，则68/10=6  68%10=8  
	   TempData[4]=dofly_DuanMa[msecond%10];
	
     //主循环中添加其他需要一直工作的程序
	
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
 static unsigned char num;
 TH0=(65536-2000)/256;		  //重新赋值 2ms
 TL0=(65536-2000)%256;
 
 Display(0,8);
 if(GoFlag)
   {
   num++;
  if(num==5)
    {
	num=0;
   msecond++;
   if (msecond==100)//100进1
     {
     msecond=0;
     second++;//秒加1
	 if(second==100)
	   second=0;
     }
   }
  }
}
/*------------------------------------------------
              外部中断0程序
------------------------------------------------*/
void ISR_INT0(void) interrupt 0 
{
 GoFlag=!GoFlag; //利用外部中断打开和关闭定时标志 用于开始和停止计时
}
/*------------------------------------------------
              外部中断1程序
------------------------------------------------*/
void ISR_INT1(void) interrupt 2 
{
if(GoFlag==0)//停止时才可以清零
   CLR();
}/*------------------------------------------------
              数值清零
------------------------------------------------*/
void CLR(void)
{
 second=0;      //利用外部中断清零
 msecond=0;
}

