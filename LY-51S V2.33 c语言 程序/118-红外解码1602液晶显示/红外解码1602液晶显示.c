/*-----------------------------------------------
  名称：遥控器红外解码数液晶显示
  论坛：www.doflye.net
  编写：shifang
  日期：2009.5
  修改：无
  内容：按配套遥控器按键，液晶显示4组码值，分别是用户码 用户码 数据码 数据反码
        显示如下：
		 www.doflye.net
		 Code:1E-1E-00-FF
------------------------------------------------*/
#include<reg52.h>    //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
#include"1602.h"
#include"delay.h"

sbit IR=P3^2;  //红外接口标志

char code Tab[16]="0123456789ABCDEF";
/*------------------------------------------------
                全局变量声明
------------------------------------------------*/

unsigned char  irtime;//红外用全局变量

bit irpro_ok,irok;
unsigned char IRcord[4];
unsigned char irdata[33];

unsigned char TempData[16];
/*------------------------------------------------
                  函数声明
------------------------------------------------*/
void Ir_work(void);
void Ircordpro(void);

/*------------------------------------------------
                  定时器0中断处理
------------------------------------------------*/

void tim0_isr (void) interrupt 1 using 1
{
  irtime++;  //用于计数2个下降沿之间的时间
}

/*------------------------------------------------
                  外部中断0中断处理
------------------------------------------------*/
void EX0_ISR (void) interrupt 0 //外部中断0服务函数
{
  static unsigned char  i;             //接收红外信号处理
  static bit startflag;                //是否开始处理标志位

if(startflag)                         
   {
    if(irtime<63&&irtime>=33)//引导码 TC9012的头码，9ms+4.5ms
                        i=0;
    		irdata[i]=irtime;//存储每个电平的持续时间，用于以后判断是0还是1
    		irtime=0;
    		i++;
   			 if(i==33)
      			{
	  			 irok=1;
				 i=0;
	  			}
          }
  	 else
		{
		irtime=0;
		startflag=1;
		}

}

/*------------------------------------------------
                定时器0初始化
------------------------------------------------*/
void TIM0init(void)//定时器0初始化
{

  TMOD=0x02;//定时器0工作方式2，TH0是重装值，TL0是初值
  TH0=0x00; //重载值
  TL0=0x00; //初始化值
  ET0=1;    //开中断
  TR0=1;    
}
/*------------------------------------------------
                  外部中断0初始化
------------------------------------------------*/
void EX0init(void)
{
 IT0 = 1;   //指定外部中断0下降沿触发，INT0 (P3.2)
 EX0 = 1;   //使能外部中断
 EA = 1;    //开总中断
}
/*------------------------------------------------
                  键值处理
------------------------------------------------*/

void Ir_work(void)
{
       TempData[0] = Tab[IRcord[0]/16];   //处理客户码
	   TempData[1] = Tab[IRcord[0]%16];
       TempData[2] = '-';
	   TempData[3] = Tab[IRcord[1]/16];   //处理客户码
	   TempData[4] = Tab[IRcord[1]%16];
	   TempData[5] = '-';
	   TempData[6] = Tab[IRcord[2]/16];   //处理数据码
	   TempData[7] = Tab[IRcord[2]%16];
	   TempData[8] = '-';
	   TempData[9] = Tab[IRcord[3]/16];   //处理数据反码
	   TempData[10] = Tab[IRcord[3]%16];

       LCD_Write_String(5,1,TempData);

	   irpro_ok=0;//处理完成标志

  }
/*------------------------------------------------
                红外码值处理
------------------------------------------------*/
void Ircordpro(void)//红外码值处理函数
{ 
  unsigned char i, j, k;
  unsigned char cord,value;

  k=1;
  for(i=0;i<4;i++)      //处理4个字节
     {
      for(j=1;j<=8;j++) //处理1个字节8位
         {
          cord=irdata[k];
          if(cord>7)//大于某值为1，这个和晶振有绝对关系，这里使用12M计算，此值可以有一定误差
             value|=0x80;
          if(j<8)
		    {
			 value>>=1;
			}
           k++;
         }
     IRcord[i]=value;
     value=0;     
     } 
	 irpro_ok=1;//处理完毕标志位置1
}

/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main(void)
{
 EX0init();            //初始化外部中断
 TIM0init();           //初始化定时器

 LCD_Init();           //初始化液晶
 DelayMs(20);          //延时有助于稳定
 LCD_Clear();          //清屏

 LCD_Write_String(0,0,"www.doflye.net");
 LCD_Write_String(0,1,"Code:");

 while(1)//主循环
   {
    if(irok)                        //如果接收好了进行红外处理
	  {   
	   Ircordpro();
 	   irok=0;
	  }

    if(irpro_ok)                   //如果处理好后进行工作处理，如按对应的按键后显示对应的数字等
	  {
	   Ir_work();
  	  }
   }
}
  
  