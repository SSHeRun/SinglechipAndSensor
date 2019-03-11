/*-----------------------------------------------
  名称：双色点阵
  论坛：www.doflye.net
  编写：shifang
  修改：无
  内容：使用hc595驱动双色点阵
------------------------------------------------*/
#include<reg52.h>    //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
#include <intrins.h> 


unsigned char  segout[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}; //8列
unsigned char code tab[96]={
                        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //空格
                        0x00,0x7E,0xFF,0xC3,0xC3,0xFF,0x7E,0x00, //字符0
                        0x00,0x00,0x43,0xFF,0xFF,0x03,0x00,0x00, //字符1
                        0x00,0x63,0xC7,0xCF,0xDB,0xF3,0x63,0x00, //字符2
                        0x00,0x42,0xDB,0xDB,0xDB,0xFF,0x66,0x00, //字符3
                        0x00,0x3E,0x46,0xFF,0xFF,0x06,0x06,0x00, //字符4
                        0x00,0xF6,0xF7,0xD3,0xD3,0xDF,0xDE,0x00, //字符5
                        0x00,0x7E,0xFF,0xDB,0xDB,0xDF,0x4E,0x00, //字符6
                        0x00,0xC0,0xC0,0xC7,0xFF,0xF8,0xC0,0x00, //字符7
                        0x00,0xFF,0xFF,0xDB,0xDB,0xFF,0xFF,0x00, //字符8
                        0x00,0x72,0xFB,0xDB,0xDB,0xFF,0x7E,0x00, //字符9
                        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00   //空格
                        };
/*------------------------------------------------
                硬件端口定义
------------------------------------------------*/
sbit LATCH = P1^0; 
sbit SRCLK= P1^1;
sbit SER  = P1^2;


sbit LATCH_B = P2^2;
sbit SRCLK_B= P2^1;
sbit SER_B= P2^0;


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
                发送字节程序
	   带有方向参数，可以选择从高位节写入或者低位写入
------------------------------------------------*/
void SendByte(unsigned char dat,bit direction)
{    
  unsigned char i,temp; 
   if(direction==0)
     temp=0x80;
   else
     temp=0x01;

   for(i=0;i<8;i++)
        {
         SRCLK=0;
         SER=dat&temp;
	 if(direction==0)
         dat<<=1;
	 else
	     dat>>=1;
         SRCLK=1;
         }
         
}
/*------------------------------------------------
                发送双字节程序
------------------------------------------------*/
void Send2Byte(unsigned char dat1,unsigned char dat2,bit direction)
{    
   SendByte(dat1,direction);
   SendByte(dat2,direction);      
}
/*------------------------------------------------
                   595锁存程序
------------------------------------------------*/
void Out595(void)
{
        LATCH=1;
        _nop_();
        LATCH=0;
}

/*------------------------------------------------
                发送位码字节程序
               使用另外一片单独595
------------------------------------------------*/
void SendSeg(unsigned char dat)
{    
  unsigned char i; 
        
   for(i=0;i<8;i++)  //发送字节
        {
         SRCLK_B=0;
         SER_B=dat&0x80;
         dat<<=1;
         SRCLK_B=1;
         }
      LATCH_B=1;    //锁存
      _nop_();
      LATCH_B=0;
         
}
/*------------------------------------------------
                   主程序
------------------------------------------------*/
void main()
{
unsigned char i,k,l;
while(1)
{
       for(k=0;k<=88;k++)     //所有的字符个数
         for(l=20;l>0;l--)    //延时长度,改变此值可以改变流动速度
            for(i=0;i<=7;i++) //8列显示
                  {
                     SendSeg(segout[i]);
					 Send2Byte(~(*(tab+i+k)),0xff,0);
					 Out595();
                     DelayMs(1);
					 Send2Byte(0xff,0xff,0);//delay(10); //防止重影
					 Out595();
                  }


	  for(k=0;k<=88;k++)     //所有的字符个数
         for(l=20;l>0;l--)    //延时长度,改变此值可以改变流动速度
            for(i=0;i<=7;i++) //8列显示
                  {
                     SendSeg(segout[7-i]);
					 Send2Byte(0xff,~(*(tab+i+k)),1); //写入字节方向改变
					 Out595();
                     DelayMs(1);
					 Send2Byte(0xff,0xff,0);//delay(10); //防止重影
					 Out595();
                  }
	 }
}