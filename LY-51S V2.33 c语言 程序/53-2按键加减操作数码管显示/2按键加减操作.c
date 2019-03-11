/*-----------------------------------------------
  名称：单个独立按键控制led输出状态
  论坛：www.doflye.net
  编写：shifang
  日期：2009.5
  修改：无
  内容：按下加按键，数值增加直到最大值，按减按键，数值减小，直到最小值
------------------------------------------------*/
#include<reg52.h> //包含头文件，一般情况不需要改动，
//头文件包含特殊功能寄存器的定义

sbit KEY_ADD=P3^3;  //定义按键输入端口
sbit KEY_DEC=P3^4;

#define DataPort P0 //定义数据端口 程序中遇到
//DataPort 则用P0 替换
sbit LATCH1=P2^2;//定义锁存使能端口 段锁存
sbit LATCH2=P2^3;//                 位锁存

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// 显示段码值0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//分别对应相应的数码管点亮,即位码
unsigned char TempData[8]; //存储显示值的全局变量
/*------------------------------------------------
                    函数声明
------------------------------------------------*/
void DelayUs2x(unsigned char t);//函数声明 
void DelayMs(unsigned char t);
void Display(unsigned char FirstBit,unsigned char Num);
/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main (void)
{
unsigned char num=0;                  
KEY_ADD=1; //按键输入端口电平置高
KEY_DEC=1;
while (1)         //主循环
  {

  if(!KEY_ADD)  //如果检测到低电平，说明按键按下
    {
	 DelayMs(10); //延时去抖，一般10-20ms
     if(!KEY_ADD)     //再次确认按键是否按下，没有按下则退出
	   {
        while(!KEY_ADD);//如果确认按下按键等待按键释放，没有释放则一直等待
	       {
		   if(num<9)    //加操作
	          num++;

	 		}
	   }
	}

 if(!KEY_DEC)  //如果检测到低电平，说明按键按下
    {
	 DelayMs(10); //延时去抖，一般10-20ms
     if(!KEY_DEC)     //再次确认按键是否按下，没有按下则退出
	   {
        while(!KEY_DEC);//如果确认按下按键等待按键释放，没有释放则一直等待
	       {
	       if(num>0)  //减操作
	          num--;
	 		}
	   }
	}
	   TempData[0]=dofly_DuanMa[num%10];
	   Display(0,1);
     //主循环中添加其他需要一直工作的程序	
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
      unsigned char i;
	  
	  for(i=0;i<Num;i++)
	   { 
	   DataPort=0;   //清空数据，防止有交替重影
       LATCH1=1;     //段锁存
       LATCH1=0;

       DataPort=dofly_WeiMa[i+FirstBit]; //取位码 
       LATCH2=1;     //位锁存
       LATCH2=0;

       DataPort=TempData[i]; //取显示数据，段码
       LATCH1=1;     //段锁存
       LATCH1=0;
       
	   DelayMs(2); // 扫描间隙延时，时间太长会闪烁，太短会造成重影

       }

}