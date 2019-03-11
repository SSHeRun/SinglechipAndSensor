#include <reg52.h>
sbit KEY1 = P3^2;
sbit PWM = P1^5;
unsigned char CYCLE; //定义周期 该数字X基准定时时间 如果是10 则周期是10 x 0.1ms
unsigned char PWM_ON ;//定义高电平时间
void delay(unsigned int cnt)
{
 while(--cnt);
}

main()
{
unsigned char PWM_Num;//定义档位
TMOD |=0x01;//定时器设置 1ms in 12M crystal
TH0=(65536-1000)/256; 
TL0=(65536-1000)%256;//定时1mS 
IE= 0x82;  //打开中断
TR0=1;

CYCLE = 10;// 时间可以调整 这个是10步调整 周期10ms 8位PWM就是256步
while(1)
  {
if(!KEY1)
  {
   delay(30000);
   if(!KEY1)
     {
      PWM_Num++;
	  if(PWM_Num==4)PWM_Num=0;
       switch(PWM_Num){
       case 0:P0=0x06;PWM_ON=0;break;//高电平时长 
       case 1:P0=0x5B;PWM_ON=4;break;
       case 2:P0=0x4F;PWM_ON=6;break;
       case 3:P0=0x66;PWM_ON=8;break;
       default:break;
     }
   }

  }
 }

}
/********************************/
/*      定时中断                */
/********************************/
void tim(void) interrupt 1 using 1
{
static unsigned char count; //
TH0=(65536-1000)/256; 
TL0=(65536-1000)%256;//定时1mS 

if (count==PWM_ON)
    {
     PWM = 1; //灯灭 
    }
  count++;
if(count == CYCLE)
    {
    count=0;
	if(PWM_ON!=0) //如果左右时间是0 保持原来状态
	   PWM = 0;//灯亮

    }

}
