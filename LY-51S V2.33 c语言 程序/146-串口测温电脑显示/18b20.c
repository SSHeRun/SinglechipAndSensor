/*-----------------------------------------------
  名称：18B20温度传感器
  网站：www.doflye.net
  编写：shifang
  日期：2009.5
  修改：无
  内容：18B20单线温度检测的应用样例程序
------------------------------------------------*/
#include"delay.h"
#include"18b20.h"
/*------------------------------------------------
                    18b20初始化
------------------------------------------------*/
bit Init_DS18B20(void)
{
 bit dat=0;
 DQ = 1;    //DQ复位
 DelayUs2x(5);   //稍做延时
 DQ = 0;         //单片机将DQ拉低
 DelayUs2x(200); //精确延时 大于 480us 小于960us
 DelayUs2x(200);
 DQ = 1;        //拉高总线
 DelayUs2x(50); //15~60us 后 接收60-240us的存在脉冲
 dat=DQ;        //如果x=0则初始化成功, x=1则初始化失败
 DelayUs2x(25); //稍作延时返回
 return dat;
}

/*------------------------------------------------
                    读取一个字节
------------------------------------------------*/
unsigned char ReadOneChar(void)
{
unsigned char i=0;
unsigned char dat = 0;
for (i=8;i>0;i--)
 {
  DQ = 0; // 给脉冲信号
  dat>>=1;
  DQ = 1; // 给脉冲信号
  if(DQ)
   dat|=0x80;
  DelayUs2x(25);
 }
 return(dat);
}
/*------------------------------------------------
                    写入一个字节
------------------------------------------------*/
void WriteOneChar(unsigned char dat)
{
 unsigned char i=0;
 for (i=8; i>0; i--)
 {
  DQ = 0;
  DQ = dat&0x01;
  DelayUs2x(25);
  DQ = 1;
  dat>>=1;
 }
DelayUs2x(25);
}

/*------------------------------------------------
                    读取温度
------------------------------------------------*/
unsigned int ReadTemperature(void)
{
unsigned char a=0;
unsigned int b=0;
unsigned int t=0;
while(Init_DS18B20()); //检测初始化是否成功
WriteOneChar(0xCC); // 跳过读序号列号的操作
WriteOneChar(0x44); // 启动温度转换
DelayMs(10);
Init_DS18B20();
WriteOneChar(0xCC); //跳过读序号列号的操作 
WriteOneChar(0xBE); //读取温度寄存器等（共可读9个寄存器） 前两个就是温度
a=ReadOneChar();   //低位
b=ReadOneChar();   //高位

b<<=8;
t=a+b;

return(t);
}
