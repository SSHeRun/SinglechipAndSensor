/*-----------------------------------------------
  名称：IIC协议 24c02存储 上次使用状态
  论坛：www.doflye.net
  编写：shifang
  修改：无
  内容：用按键加减操作一个数字，数字变化后写入24c02，当关机重新再开机，调用上次最后使用的数值
        这种方式用于存储用户的自用数据和上次使用状态
------------------------------------------------*/  
#include <reg52.h>                
#include "i2c.h"
#include "delay.h"
#include "display.h"

sbit KEY_ADD=P3^3;  //定义按键输入端口
sbit KEY_DEC=P3^4;

/*-----------------------------------------------
                     主函数
------------------------------------------------*/  
main()
{
 unsigned char num=0;
 Init_Timer0();

 IRcvStr(0xae,0,&num,1);                                
                
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
		   if(num<999)    //加操作
            {
	          num++;
              ISendStr(0xae,0,&num,1);  //写入24c02 
              DelayMs(10);
             }

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
	          {
	          num--;
              ISendStr(0xae,0,&num,1);  //写入24c02
              DelayMs(10);
             }

	 		}
	   }
	}
 TempData[0]=dofly_DuanMa[num/100];    
 TempData[1]=dofly_DuanMa[(num%100)/10];
 TempData[2]=dofly_DuanMa[(num%100)%10];
     //主循环中添加其他需要一直工作的程序
	
  }
}
