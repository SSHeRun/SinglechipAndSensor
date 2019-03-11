/*-----------------------------------------------
  名称：矩阵键盘依次输入控制 使用行列逐级扫描 中断方式
  论坛：www.doflye.net
  编写：shifang
  日期：2009.5
  修改：无
  内容：如计算器输入数据形式相同 从右至左 使用行列扫描方法
        中断方式可以有效提供cpu工作效率，在有按键动作时才扫描，平时不进行扫描工作
------------------------------------------------*/
#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义


#define DataPort P0 //定义数据端口 程序中遇到DataPort 则用P0 替换
#define KeyPort  P1

sbit LATCH1=P2^2;//定义锁存使能端口 段锁存
sbit LATCH2=P2^3;//                 位锁存

bit KeyPressFlag;//定义按键标志位

unsigned char code dofly_DuanMa[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,
		                  	         0x77,0x7c,0x39,0x5e,0x79,0x71};// 显示段码值0~F
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//分别对应相应的数码管点亮,即位码
unsigned char TempData[10]; //存储显示值的全局变量

void DelayUs2x(unsigned char t);//us级延时函数声明 
void DelayMs(unsigned char t); //ms级延时
void Display(unsigned char FirstBit,unsigned char Num);//数码管显示函数
unsigned char KeyScan(void);//键盘扫描
unsigned char KeyPro(void);
void Init_Timer0(void);//定时器初始化
void Init_INT0(void);  //外部中断0初始化和
/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main (void)
{
unsigned char num,i,j;                  
unsigned char temp[8];
Init_Timer0();
Init_INT0();
while (1)         //主循环
  {
  KeyPort=0xf0;// 赋值用于中断检测
 if(KeyPressFlag==1)
 {
  KeyPressFlag=0;  //按键标志清零，以便下次检测
  num=KeyPro();    
  if(num!=0xff)
   {
    if(i<8)
      {
       temp[i]=dofly_DuanMa[num];
	   for(j=0;j<=i;j++)
          TempData[7-i+j]=temp[j];
       }
	i++;
	if(i==9)//多出一个按键输入为了清屏 原本应该为8
      {
	  i=0;
      for(j=0;j<8;j++)//清屏
         TempData[j]=0;
       }
     }
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
 TH0=(65536-2000)/256;		  //重新赋值 2ms
 TL0=(65536-2000)%256;
 
 Display(0,8);       // 调用数码管扫描

}

/*------------------------------------------------
按键扫描函数，返回扫描键值
------------------------------------------------*/
unsigned char KeyScan(void)  //键盘扫描函数，使用行列逐级扫描法
{
 unsigned char Val;
 KeyPort=0xf0;//高四位置高，低四位拉低
 if(KeyPort!=0xf0)//表示有按键按下
   {
    DelayMs(10);  //去抖
	if(KeyPort!=0xf0)
	  {           //表示有按键按下
    	KeyPort=0xfe; //检测第一行
		if(KeyPort!=0xfe)
	  		{
			  Val=KeyPort&0xf0;
	  	      Val+=0x0e;
	  		  while(KeyPort!=0xfe);
			  DelayMs(10); //去抖
			  while(KeyPort!=0xfe);
	     	  return Val;
	        }
        KeyPort=0xfd; //检测第二行
		if(KeyPort!=0xfd)
	  		{
			  Val=KeyPort&0xf0;
	  	      Val+=0x0d;
	  		  while(KeyPort!=0xfd);
			  DelayMs(10); //去抖
			  while(KeyPort!=0xfd);
	     	  return Val;
	        }
    	KeyPort=0xfb; //检测第三行
		if(KeyPort!=0xfb)
	  		{
			  Val=KeyPort&0xf0;
	  	      Val+=0x0b;
	  		  while(KeyPort!=0xfb);
			  DelayMs(10); //去抖
			  while(KeyPort!=0xfb);
	     	  return Val;
	        }
    	KeyPort=0xf7; //检测第四行
		if(KeyPort!=0xf7)
	  		{
			  Val=KeyPort&0xf0;
	  	      Val+=0x07;
	  		  while(KeyPort!=0xf7);
			  DelayMs(10); //去抖
			  while(KeyPort!=0xf7);
	     	  return Val;
	        }
     }
   }
  return 0xff;
}
/*------------------------------------------------
按键值处理函数，返回扫键值
------------------------------------------------*/
unsigned char KeyPro(void)
{
 switch(KeyScan())
 {
  case 0x7e:return 0;break;//0 按下相应的键显示相对应的码值
  case 0x7d:return 1;break;//1
  case 0x7b:return 2;break;//2
  case 0x77:return 3;break;//3
  case 0xbe:return 4;break;//4
  case 0xbd:return 5;break;//5
  case 0xbb:return 6;break;//6
  case 0xb7:return 7;break;//7
  case 0xde:return 8;break;//8
  case 0xdd:return 9;break;//9
  case 0xdb:return 10;break;//a
  case 0xd7:return 11;break;//b
  case 0xee:return 12;break;//c
  case 0xed:return 13;break;//d
  case 0xeb:return 14;break;//e
  case 0xe7:return 15;break;//f
  default:return 0xff;break;
 }
}
/*------------------------------------------------
           外部中断0初始化
------------------------------------------------*/
void Init_INT0(void)
{
  EA=1;          //全局中断开
  EX0=1;         //外部中断0开
  IT0=1;         //边沿触发
}

/*------------------------------------------------
                 外部中断0程序
------------------------------------------------*/
void ISR_INT0(void) interrupt 0 
{
 KeyPressFlag=1;   //表明按键有动作，可以进行按键扫描
}
