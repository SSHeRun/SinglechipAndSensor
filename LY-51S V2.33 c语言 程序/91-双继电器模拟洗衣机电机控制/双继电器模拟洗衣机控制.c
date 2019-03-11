/*-----------------------------------------------
  名称：独立按键控制继电器开关
  论坛：www.doflye.net
  编写：shifang
  日期：2009.5
  修改：无
  内容：对应的继电器接口需用杜邦线连接到uln2003继电器控制端
        通过控制2个继电器循环工作状态，模拟洗衣机间歇正反转
        一般正转3s，停止2s，然后反转3s，停止2s，循环上述动作
------------------------------------------------*/
#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义

#define KeyPort P3

#define DataPort P0 //定义数据端口 程序中遇到DataPort 则用P0 替换
sbit LATCH1=P2^2;//定义锁存使能端口 段锁存
sbit LATCH2=P2^3;//                 位锁存

sbit RELAY1 = P1^1;//定义继电器信号输出端口1
sbit RELAY2 = P1^2;//定义继电器信号输出端口2
/*------------------------------------------------
                  全局变量
------------------------------------------------*/
unsigned char hour,minute,second;//定义时分秒

bit UpdateTimeFlag;//定义读时间标志

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
void Stop(void);
void Clockwise (void);
void Anticlockwise(void);
/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main (void)
{
unsigned char num;                  
Init_Timer0();    //初始化定时器0，主要用于数码管动态扫描
Stop();           //初始化洗衣机停止转动
while (1)         //主循环
  {
 num=KeyScan();    //循环调用按键扫描

 if(num==1)        //定时长度增加
   {
    if(minute<99)
	  minute++;
   }
 else if(num==2)   //定时长度减小
   {
    if(minute>0)
	  minute--;
   }
 if(UpdateTimeFlag==1)//定时刷新显示数据
   {
    UpdateTimeFlag=0;
	TempData[2]=dofly_DuanMa[minute/10];//分解显示信息，如要显示68，则68/10=6  68%10=8 
    TempData[3]=dofly_DuanMa[minute%10];//分解显示信息，如要显示68，则68/10=6  68%10=8  
    TempData[4]=0x40;
    TempData[5]=dofly_DuanMa[second/10];//分解显示信息，如要显示68，则68/10=6  68%10=8 
    TempData[6]=dofly_DuanMa[second%10];//分解显示信息，如要显示68，则68/10=6  68%10=8 

	if((minute==0)&&(second==0))//定时时间到，洗衣机停止转动
      {
      Stop();
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
 static unsigned int num,i,times;
 TH0=(65536-2000)/256;		  //重新赋值 2ms
 TL0=(65536-2000)%256;
 
 Display(0,8);                // 调用数码管扫描
 i++;
 if(i==20)          //20ms更新一次
    {
	i=0;
	UpdateTimeFlag=1; //更新时间志位置1
	}
 num++;
 if(num==500)        //大致1s
   {
    num=0;
   
    ///////////////////////////////////////
	//执行正反转动作。
	switch(times)
	   {
	    case 1:Clockwise ();   break;//正转3s
		case 4:Stop();         break;//停止2s
		case 6:Anticlockwise();break;//反转3s
		case 9:Stop();         break;//停止2s
		case 11:times=0;       break;//清零循环上述动作
		default:break;
	   }
	times++;
   ///////////////////////////////////////////////////////
    if((minute!=0)||second)//如果分钟和秒都为0，不进行计时
       second--;//秒减1
    if(second==0xff)//如果=0后再减1则赋值59，即00过后显示59
	   {
	   second=59;
	   if(minute>0)//倒计时条件
	     {
	      minute--;
	      }

	   }
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
                 顺时针转动
------------------------------------------------*/
void Clockwise (void)
{
 RELAY1=1;    //继电器1吸合，接通正向线圈
 RELAY2=0;
}
/*------------------------------------------------
                 逆时针转动
------------------------------------------------*/
void Anticlockwise(void)
{
  RELAY1=0;
  RELAY2=1;   //继电器2吸合，接通反向线圈
}

/*------------------------------------------------
                 停止转动
------------------------------------------------*/
void Stop(void)
{
  RELAY1=0;   
  RELAY2=0;
}
