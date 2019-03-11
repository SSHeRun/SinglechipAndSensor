/*-----------------------------------------------
  名称：99分钟倒计时数码管显示
  论坛：www.doflye.net
  编写：shifang
  日期：2009.5
  修改：无
  内容：99分钟倒计时器，可以通过按键上调下调时间，定时器到时喇叭发声报警
        带有点按和连按功能
------------------------------------------------*/
#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义

#define DataPort P0 //定义数据端口 程序中遇到DataPort 则用P0 替换

sbit LATCH1=P2^2;//定义锁存使能端口 段锁存
sbit LATCH2=P2^3;//                 位锁存

sbit BEEP = P1^1;//定义喇叭端口 
bit BeepFlag;

sbit KEY_ADD=P3^0;  //定义按键输入端口
sbit KEY_DEC=P3^1;

unsigned char hour,minute,second;//定义时分秒

bit UpdateTimeFlag;//定义读时间标志

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// 显示段码值0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//分别对应相应的数码管点亮,即位码
unsigned char TempData[8]; //存储显示值的全局变量

void DelayUs2x(unsigned char t);//us级延时函数声明 
void DelayMs(unsigned char t); //ms级延时
void Display(unsigned char FirstBit,unsigned char Num);//数码管显示函数
void Init_Timer0(void);//定时器初始化
void  Beep_OFF(void);
void  Beep_ON(void);
void Display_Data_Operation(void);
/*------------------------------------------------
                显示数据处理函数
------------------------------------------------*/
void Display_Data_Operation(void)
{
 TempData[2]=dofly_DuanMa[minute/10];//分解显示信息，如要显示68，则68/10=6  68%10=8 
 TempData[3]=dofly_DuanMa[minute%10];//分解显示信息，如要显示68，则68/10=6  68%10=8  
 TempData[4]=0x40;
 TempData[5]=dofly_DuanMa[second/10];//分解显示信息，如要显示68，则68/10=6  68%10=8 
 TempData[6]=dofly_DuanMa[second%10];//分解显示信息，如要显示68，则68/10=6  68%10=8 
}
/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main (void)
{
unsigned char key_press_num;                  

Init_Timer0();

while (1)         //主循环
  {

 if(!KEY_ADD)  //如果检测到低电平，说明按键按下
    {
	 DelayMs(10); //延时去抖，一般10-20ms
     if(!KEY_ADD)     //再次确认按键是否按下，没有按下则退出
	   {
	   while(!KEY_ADD)
	    {
		key_press_num++;
        DelayMs(10);           //10x200=2000ms=2s 
		if(key_press_num==200) //大约2s
		  {
		   key_press_num=0;    //如果达到长按键标准 则进入长按键动作
           while(!KEY_ADD)     //这里用于识别是否按键还在按下，如果按下执行相关动作，否则退出
		       {
		       	if(minute<99)    //加操作
	              minute++;
				//即时把显示数据处理，如果去掉下面2句处理信息，实际上看不到渐变效果，而是看到跳变效果
				//用户可以自行屏蔽测试
                Display_Data_Operation();
                DelayMs(50);//用于调节长按循环操作的速度
				}
		    }
		}
      key_press_num=0;//防止累加造成错误识别
		   if(minute<99)    //加操作
	          minute++;
	   }
	}

 if(!KEY_DEC)  //如果检测到低电平，说明按键按下
    {
	 DelayMs(10); //延时去抖，一般10-20ms
     if(!KEY_DEC)     //再次确认按键是否按下，没有按下则退出
	   {
	   while(!KEY_DEC)
	    {
		key_press_num++;
        DelayMs(10);
		if(key_press_num==200) //大约2s
		  {
		   key_press_num=0;
           while(!KEY_DEC)
		       {
		       	if(minute>0)  //减操作
	               minute--;
                Display_Data_Operation();
                DelayMs(50);//用于调节长按循环操作的速度
				}
		    }
		}
      key_press_num=0;//防止累加造成错误识别
	       if(minute>0)  //减操作
	          minute--;
	 	
	   }
	}

if(UpdateTimeFlag==1)
 {
 UpdateTimeFlag=0;
					    	       
 Display_Data_Operation();	

 if((minute==0)&&(second==0))//条件满足蜂鸣器闪响
   {
    Beep_ON();
   }
 else
    Beep_OFF();   //不满足时关掉
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
 static unsigned int num,i;

 TH0=(65536-1000)/256;//重新赋值 1ms
 TL0=(65536-1000)%256;
 if(BeepFlag)         //启动喇叭标志
   {
   if(num<300 || (num>500&&num<800) )
     BEEP=!BEEP;     //闪响
   else
     BEEP=0;         //停止发声
   }
 Display(0,8);       // 调用数码管扫描
 i++;
 if(i==20)          //20ms更新一次
    {
	i=0;
	UpdateTimeFlag=1; //更新时间志位置1
	}
 num++;
 if(num==1000)        //大致1s
   {
    num=0;
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
             喇叭警报声音开
------------------------------------------------*/
void  Beep_ON(void)
{
 BeepFlag=1;
}
/*------------------------------------------------
             喇叭警报声音关
------------------------------------------------*/
void  Beep_OFF(void)
{
 BeepFlag=0;
 BEEP=0;
}
