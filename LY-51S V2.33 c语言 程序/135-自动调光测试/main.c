/*-----------------------------------------------
  ���ƣ�IICЭ�� PCF8591 AD/DAת�� ���Թ�������
  ��̳��www.doflye.net
  ��д��shifang
  �޸ģ���
  ���ݣ�ʹ��4·AD�е�1·����ⲿģ�������룬��ʾ0-255��ֵ
        DA�����ѹ��LED��ʾ����Ҫ���ڼ�⻷�����ߣ�����ǿ���������С���Ա�֤�����ǿ�ȶ���һ����Χ
		����˼·���÷���Ҫ�����Զ����⣬���Ը��ݻ��������Զ����ڣ��Ӷ��ﵽ������󻯣���������ʾ��
------------------------------------------------*/  
#include <reg52.h>                
#include "i2c.h"
#include "delay.h"
#include "display.h"

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
 unsigned char num=0;
 Init_Timer0();


while (1)         //��ѭ��
  {
  
 num=256-ReadADC(0);//
 TempData[0]=dofly_DuanMa[num/100];    
 TempData[1]=dofly_DuanMa[(num%100)/10];
 TempData[2]=dofly_DuanMa[(num%100)%10];
 //��ѭ�������������Ҫһֱ�����ĳ���
 WriteDAC(num);//�ѵ�ǰֵд��DA����led��ʾ����
 DelayMs(100);
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
