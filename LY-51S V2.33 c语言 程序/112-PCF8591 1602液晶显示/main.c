/*-----------------------------------------------
  名称：IIC协议 PCF8591 AD/DA转换
  论坛：www.doflye.net
  编写：shifang
  修改：无
  内容：使用4路AD中的4路检测外部模拟量输入 使用液晶显示
------------------------------------------------*/  
#include <reg52.h>                
#include "i2c.h"
#include "delay.h"
#include "1602.h"
#include <stdio.h>

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
 unsigned char num=0,i;
 unsigned char temp[7];//定义显示区域临时存储数组
 float Voltage;        //定义浮点变量
 LCD_Init();           //初始化液晶
 DelayMs(20);          //延时有助于稳定
 LCD_Clear();          //清屏

while (1)              //主循环
  {
 
 for(i=0;i<5;i++)//连续读5次，取最后一次，以便读取稳定值
    num=ReadADC(0);      //读取第1路电压值，范围是0-255
 Voltage=(float)num*5/256;   //根据参考电源VREF算出时间电压，float是强制转换符号，用于将结果转换成浮点型
 sprintf(temp,"V0 %3.2f ",Voltage);//格式输出电压值，%3.2f 表示浮点输出，共3位数，小数点后2位
 LCD_Write_String(0,0,temp);

 for(i=0;i<5;i++) 
     num=ReadADC(1);
 Voltage=(float)num*5/256;   
 sprintf(temp,"V1 %3.2f ",Voltage);
 LCD_Write_String(8,0,temp);

 for(i=0;i<5;i++) 
     num=ReadADC(2);
 Voltage=(float)num*5/256;   
 sprintf(temp,"V2 %3.2f ",Voltage);
 LCD_Write_String(0,1,temp);

 for(i=0;i<5;i++) 
     num=ReadADC(3);
 Voltage=(float)num*5/256;   
 sprintf(temp,"V3 %3.2f ",Voltage);
 LCD_Write_String(8,1,temp);
 //主循环中添加其他需要一直工作的程序
 DelayMs(200);

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
