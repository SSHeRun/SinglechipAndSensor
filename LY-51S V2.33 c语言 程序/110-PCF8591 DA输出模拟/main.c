/*-----------------------------------------------
  ���ƣ�IICЭ�� PCF8591 AD/DAת��
  ��̳��www.doflye.net
  ��д��shifang
  �޸ģ���
  ���ݣ�ʹ��DA���룬�������ʾ�����������LED��ʾģ���ѹ��С
------------------------------------------------*/  
#include <reg52.h>                
#include "i2c.h"
#include "delay.h"
#include "display.h"

#define AddWr 0x90   //д���ݵ�ַ 
#define AddRd 0x91   //�����ݵ�ַ

extern bit ack;
bit WriteDAC(unsigned char dat);
/*------------------------------------------------
              ������
------------------------------------------------*/
main()
{
 unsigned char num=0;
 Init_Timer0();
 while (1)         //��ѭ��
  {
 WriteDAC(num);
 num++;//�����ۼӣ�ֵ��0-255����ѭ��������ʾ��������ϣ�
 TempData[0]=dofly_DuanMa[num/100];    
 TempData[1]=dofly_DuanMa[(num%100)/10];
 TempData[2]=dofly_DuanMa[(num%100)%10];
 DelayMs(100);
  }
}

/*------------------------------------------------
               д��DAת����ֵ
���������dat ��ʾ��Ҫת����DA��ֵ����Χ��0-255
------------------------------------------------*/
bit WriteDAC(unsigned char dat)
{
   Start_I2c();               //��������
   SendByte(AddWr);             //����������ַ
     if(ack==0)return(0);
   SendByte(0x40);            //���������ӵ�ַ
     if(ack==0)return(0);
   SendByte(dat);             //��������
     if(ack==0)return(0);
   Stop_I2c();  
}
