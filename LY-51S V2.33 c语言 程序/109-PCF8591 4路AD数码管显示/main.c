/*-----------------------------------------------
  ���ƣ�IICЭ�� PCF8591 AD/DAת��
  ��̳��www.doflye.net
  ��д��shifang
  �޸ģ���
  ���ݣ�ʹ��4·AD�е�4·����ⲿģ��������
------------------------------------------------*/  
#include <reg52.h>                
#include "i2c.h"
#include "delay.h"
#include "display.h"

#define AddWr 0x90   //д���ݵ�ַ 
#define AddRd 0x91   //�����ݵ�ַ

extern bit ack;
bit ReadADFlag;



unsigned char ReadADC(unsigned char Chl);
bit WriteDAC(unsigned char dat);
/*------------------------------------------------
              ������
------------------------------------------------*/
main()
{
 unsigned char num=0,i;
 Init_Timer0();
 DelayMs(20);

while (1)         //��ѭ��
  {
 if(ReadADFlag)
  {
  ReadADFlag=0; 
//������5�Σ�����ͨ���������Σ�ȡ���һ��ֵ���Ա�����ȶ�ֵ
 for(i=0;i<5;i++)
     num=ReadADC(0);
// x10��ʾ��ʵ��ֵ����10����4.5 ��� 45 ��������һ������ x5 ��ʾ��׼��ѹ5V 
  num=num*5*10/256;   
  TempData[0]=dofly_DuanMa[num/10]|0x80;    
  TempData[1]=dofly_DuanMa[num%10];

 for(i=0;i<5;i++)
   num=ReadADC(1);
 num=num*5*10/256;   // x10��ʾ��ʵ��ֵ����10����4.5 ��� 45 ��������һ������
 TempData[2]=dofly_DuanMa[num/10]|0x80;    
 TempData[3]=dofly_DuanMa[num%10];

 for(i=0;i<5;i++)
    num=ReadADC(2);
 num=num*5*10/256;   // x10��ʾ��ʵ��ֵ����10����4.5 ��� 45 ��������һ������
 TempData[4]=dofly_DuanMa[num/10]|0x80;    
 TempData[5]=dofly_DuanMa[num%10];

 for(i=0;i<5;i++)
     num=ReadADC(3);
 num=num*5*10/256;   // x10��ʾ��ʵ��ֵ����10����4.5 ��� 45 ��������һ������
 TempData[6]=dofly_DuanMa[num/10]|0x80;    
 TempData[7]=dofly_DuanMa[num%10];
 //��ѭ�������������Ҫһֱ�����ĳ���
   }
  }
}
/*------------------------------------------------
             ��ADתֵ����
������� Chl ��ʾ��Ҫת����ͨ������Χ��0-3
����ֵ��Χ0-255
------------------------------------------------*/
unsigned char ReadADC(unsigned char Chl)
 {
  unsigned char Val;
   Start_I2c();               //��������
   SendByte(AddWr);             //����������ַ
     if(ack==0)return(0);
   SendByte(Chl);            //���������ӵ�ַ
     if(ack==0)return(0);
   Start_I2c();
   SendByte(AddRd);
      if(ack==0)return(0);
   Val=RcvByte();
   NoAck_I2c();                 //���ͷ�Ӧλ
   Stop_I2c();                  //��������
  return(Val);
 }
/*------------------------------------------------
               д��DAת����ֵ
���������dat ��ʾ��Ҫת����DA��ֵ����Χ��0-255
------------------------------------------------*/
/*bit WriteDAC(unsigned char dat)
{
   Start_I2c();               //��������
   SendByte(AddWr);             //����������ַ
     if(ack==0)return(0);
   SendByte(0x40);            //���������ӵ�ַ
     if(ack==0)return(0);
   SendByte(dat);             //��������
     if(ack==0)return(0);
   Stop_I2c();  
}*/


