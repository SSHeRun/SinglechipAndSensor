/*-----------------------------------------------
  ���ƣ�IICЭ�� PCF8591 AD/DAת��
  ��̳��www.doflye.net
  ��д��shifang
  �޸ģ���
  ���ݣ�ʹ��DA���룬�������ʾ�����������LED��ʾģ���ѹ��С
------------------------------------------------*/  
#include <reg52.h>                
#include "i2c.h"


#define AddWr 0x90   //д���ݵ�ַ 
#define AddRd 0x91   //�����ݵ�ַ

/*unsigned char code tab[]={
                          0,25,50,75,100,125,150,175,200,225,250  //�����ֵԽ�࣬����Խƽ��
                          };*/
unsigned char code tab1[]={
                          0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,
						  170,180,190,200,210,220,230,240,250
                          };

extern bit ack;
bit WriteDAC(unsigned char dat,unsigned char num);
/*------------------------------------------------
              ������
------------------------------------------------*/
main()
{
 unsigned char i;
 while (1)         //��ѭ��
  {
  for(i=0;i<26;i++)
     WriteDAC(tab1[i],1);
  }
}

/*------------------------------------------------
               д��DAת����ֵ
���������dat ��ʾ��Ҫת����DA��ֵ����Χ��0-255
------------------------------------------------*/
bit WriteDAC(unsigned char dat,unsigned char num)
{  unsigned char i;
   Start_I2c();               //��������
   SendByte(AddWr);             //����������ַ
     if(ack==0)return(0);
   SendByte(0x40);            //���������ӵ�ַ
     if(ack==0)return(0);
  for(i=0;i<num;i++)
     {
   SendByte(dat);             //��������
     if(ack==0)return(0);
     }
   Stop_I2c();  
}
