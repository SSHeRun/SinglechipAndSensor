/*-----------------------------------------------
  ���ƣ�IICЭ�� PCF8591 AD/DAת��
  ��̳��www.doflye.net
  ��д��shifang
  �޸ģ���
  ���ݣ�ʹ��4·AD�е�4·����ⲿģ�������� ʹ��Һ����ʾ
------------------------------------------------*/  
#include <reg52.h>                
#include "i2c.h"
#include "delay.h"
#include "1602.h"
#include <stdio.h>

#define AddWr 0x90   //д���ݵ�ַ 
#define AddRd 0x91   //�����ݵ�ַ

extern bit ack;
unsigned char ReadADC(unsigned char Chl);
bit WriteDAC(unsigned char dat);
/*------------------------------------------------
              ������
------------------------------------------------*/
main()
{
 unsigned char num=0,i;
 unsigned char temp[7];//������ʾ������ʱ�洢����
 float Voltage;        //���帡�����
 LCD_Init();           //��ʼ��Һ��
 DelayMs(20);          //��ʱ�������ȶ�
 LCD_Clear();          //����

while (1)              //��ѭ��
  {
 
 for(i=0;i<5;i++)//������5�Σ�ȡ���һ�Σ��Ա��ȡ�ȶ�ֵ
    num=ReadADC(0);      //��ȡ��1·��ѹֵ����Χ��0-255
 Voltage=(float)num*5/256;   //���ݲο���ԴVREF���ʱ���ѹ��float��ǿ��ת�����ţ����ڽ����ת���ɸ�����
 sprintf(temp,"V0 %3.2f ",Voltage);//��ʽ�����ѹֵ��%3.2f ��ʾ�����������3λ����С�����2λ
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
 //��ѭ�������������Ҫһֱ�����ĳ���
 DelayMs(200);

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
   SendByte(0x40|Chl);            //���������ӵ�ַ
     if(ack==0)return(0);
   Start_I2c();
   SendByte(AddWr+1);
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
