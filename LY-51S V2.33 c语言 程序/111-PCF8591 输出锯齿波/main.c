/*-----------------------------------------------
  名称：IIC协议 PCF8591 AD/DA转换
  论坛：www.doflye.net
  编写：shifang
  修改：无
  内容：使用DA输入，数码管显示输出数字量，LED显示模拟电压大小
------------------------------------------------*/  
#include <reg52.h>                
#include "i2c.h"


#define AddWr 0x90   //写数据地址 
#define AddRd 0x91   //读数据地址

/*unsigned char code tab[]={
                          0,25,50,75,100,125,150,175,200,225,250  //表格数值越多，波形越平滑
                          };*/
unsigned char code tab1[]={
                          0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,
						  170,180,190,200,210,220,230,240,250
                          };

extern bit ack;
bit WriteDAC(unsigned char dat,unsigned char num);
/*------------------------------------------------
              主程序
------------------------------------------------*/
main()
{
 unsigned char i;
 while (1)         //主循环
  {
  for(i=0;i<26;i++)
     WriteDAC(tab1[i],1);
  }
}

/*------------------------------------------------
               写入DA转换数值
输入参数：dat 表示需要转换的DA数值，范围是0-255
------------------------------------------------*/
bit WriteDAC(unsigned char dat,unsigned char num)
{  unsigned char i;
   Start_I2c();               //启动总线
   SendByte(AddWr);             //发送器件地址
     if(ack==0)return(0);
   SendByte(0x40);            //发送器件子地址
     if(ack==0)return(0);
  for(i=0;i<num;i++)
     {
   SendByte(dat);             //发送数据
     if(ack==0)return(0);
     }
   Stop_I2c();  
}
