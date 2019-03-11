/*-----------------------------------------------
  名称：IIC协议 PCF8591 AD/DA转换
  论坛：www.doflye.net
  编写：shifang
  修改：无
  内容：使用DA输入，数码管显示输出数字量，LED显示模拟电压大小
------------------------------------------------*/  
#include <reg52.h>                
#include "i2c.h"
#include "delay.h"
#include "display.h"

#define AddWr 0x90   //写数据地址 
#define AddRd 0x91   //读数据地址

extern bit ack;
bit WriteDAC(unsigned char dat);
/*------------------------------------------------
              主程序
------------------------------------------------*/
main()
{
 unsigned char num=0;
 Init_Timer0();
 while (1)         //主循环
  {
 WriteDAC(num);
 num++;//连续累加，值从0-255反复循环，并显示在数码管上，
 TempData[0]=dofly_DuanMa[num/100];    
 TempData[1]=dofly_DuanMa[(num%100)/10];
 TempData[2]=dofly_DuanMa[(num%100)%10];
 DelayMs(100);
  }
}

/*------------------------------------------------
               写入DA转换数值
输入参数：dat 表示需要转换的DA数值，范围是0-255
------------------------------------------------*/
bit WriteDAC(unsigned char dat)
{
   Start_I2c();               //启动总线
   SendByte(AddWr);             //发送器件地址
     if(ack==0)return(0);
   SendByte(0x40);            //发送器件子地址
     if(ack==0)return(0);
   SendByte(dat);             //发送数据
     if(ack==0)return(0);
   Stop_I2c();  
}
