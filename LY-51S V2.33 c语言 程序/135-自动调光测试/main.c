/*-----------------------------------------------
  名称：IIC协议 PCF8591 AD/DA转换 测试光敏电阻
  论坛：www.doflye.net
  编写：shifang
  修改：无
  内容：使用4路AD中的1路检测外部模拟量输入，显示0-255数值
        DA输出电压用LED显示，主要用于检测环境光线，光线强，则灯亮度小，以保证整体光强稳定在一个范围
		这种思路和用法主要用于自动调光，可以根据环境亮度自动调节，从而达到节能最大化，本样例演示用
------------------------------------------------*/  
#include <reg52.h>                
#include "i2c.h"
#include "delay.h"
#include "display.h"

#define AddWr 0x90   //写数据地址 
#define AddRd 0x91   //读数据地址

extern bit ack;
unsigned char ReadADC(unsigned char Chl);
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
  
 num=256-ReadADC(0);//
 TempData[0]=dofly_DuanMa[num/100];    
 TempData[1]=dofly_DuanMa[(num%100)/10];
 TempData[2]=dofly_DuanMa[(num%100)%10];
 //主循环中添加其他需要一直工作的程序
 WriteDAC(num);//把当前值写入DA，用led演示亮度
 DelayMs(100);
  }
}
/*------------------------------------------------
             读AD转值程序
输入参数 Chl 表示需要转换的通道，范围从0-3
返回值范围0-255
------------------------------------------------*/
unsigned char ReadADC(unsigned char Chl)
 {
   unsigned char Val;
   Start_I2c();               //启动总线
   SendByte(AddWr);             //发送器件地址
     if(ack==0)return(0);
   SendByte(0x40|Chl);            //发送器件子地址
     if(ack==0)return(0);
   Start_I2c();
   SendByte(AddWr+1);
      if(ack==0)return(0);
   Val=RcvByte();
   NoAck_I2c();                 //发送非应位
   Stop_I2c();                  //结束总线
  return(Val);
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
