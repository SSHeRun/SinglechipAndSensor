/*-----------------------------------------------
  名称：IIC协议 PCF8591 AD/DA转换
  论坛：www.doflye.net
  编写：shifang
  修改：无
  内容：使用4路AD中的4路检测外部模拟量输入
------------------------------------------------*/  
#include <reg52.h>                
#include "i2c.h"
#include "delay.h"
#include "display.h"

#define AddWr 0x90   //写数据地址 
#define AddRd 0x91   //读数据地址

extern bit ack;
bit ReadADFlag;



unsigned char ReadADC(unsigned char Chl);
bit WriteDAC(unsigned char dat);
/*------------------------------------------------
              主程序
------------------------------------------------*/
main()
{
 unsigned char num=0,i;
 Init_Timer0();
 DelayMs(20);

while (1)         //主循环
  {
 if(ReadADFlag)
  {
  ReadADFlag=0; 
//连续读5次，输入通道后多读几次，取最后一次值，以便读出稳定值
 for(i=0;i<5;i++)
     num=ReadADC(0);
// x10表示把实际值扩大10，如4.5 变成 45 方便做下一步处理 x5 表示基准电压5V 
  num=num*5*10/256;   
  TempData[0]=dofly_DuanMa[num/10]|0x80;    
  TempData[1]=dofly_DuanMa[num%10];

 for(i=0;i<5;i++)
   num=ReadADC(1);
 num=num*5*10/256;   // x10表示把实际值扩大10，如4.5 变成 45 方便做下一步处理
 TempData[2]=dofly_DuanMa[num/10]|0x80;    
 TempData[3]=dofly_DuanMa[num%10];

 for(i=0;i<5;i++)
    num=ReadADC(2);
 num=num*5*10/256;   // x10表示把实际值扩大10，如4.5 变成 45 方便做下一步处理
 TempData[4]=dofly_DuanMa[num/10]|0x80;    
 TempData[5]=dofly_DuanMa[num%10];

 for(i=0;i<5;i++)
     num=ReadADC(3);
 num=num*5*10/256;   // x10表示把实际值扩大10，如4.5 变成 45 方便做下一步处理
 TempData[6]=dofly_DuanMa[num/10]|0x80;    
 TempData[7]=dofly_DuanMa[num%10];
 //主循环中添加其他需要一直工作的程序
   }
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
   SendByte(Chl);            //发送器件子地址
     if(ack==0)return(0);
   Start_I2c();
   SendByte(AddRd);
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
/*bit WriteDAC(unsigned char dat)
{
   Start_I2c();               //启动总线
   SendByte(AddWr);             //发送器件地址
     if(ack==0)return(0);
   SendByte(0x40);            //发送器件子地址
     if(ack==0)return(0);
   SendByte(dat);             //发送数据
     if(ack==0)return(0);
   Stop_I2c();  
}*/


