/*-----------------------------------------------
  ���ƣ�DS18b20 �¶ȼ��Һ����ʾ
  ��̳��www.doflye.net
  ��д��shifang
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ�
------------------------------------------------*/
#include<reg52.h> //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���
#include<stdio.h>
#include "18b20.h"
#include "18b20a.h"
#include "1602.h"
#include "delay.h"

bit ReadTempFlag;//�����ʱ���־

void Init_Timer0(void);//��ʱ����ʼ��
/*------------------------------------------------
              ����ͨѶ��ʼ��
------------------------------------------------*/
void UART_Init(void)
{
    SCON  = 0x50;		        // SCON: ģʽ 1, 8-bit UART, ʹ�ܽ���  
    TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit ��װ
    TH1   = 0xFD;               // TH1:  ��װֵ 9600 ������ ���� 11.0592MHz  
    TR1   = 1;                  // TR1:  timer 1 ��                         
    //EA    = 1;                  //�����ж�
    //ES    = 1;                  //�򿪴����ж�
	TI=1;
}
/*------------------------------------------------
                    ������
------------------------------------------------*/
void main (void)
{                  
int temp;
float temperature;
char displaytemp[16];//������ʾ������ʱ�洢����

LCD_Init();           //��ʼ��Һ��
DelayMs(20);          //��ʱ�������ȶ�
LCD_Clear();          //����
Init_Timer0();
UART_Init();
Lcd_User_Chr();       //д���Զ����ַ�

LCD_Write_Char(13,0,0x01);//д���¶����Ͻǵ�
LCD_Write_Char(14,0,'C'); //д���ַ�C

LCD_Write_Char(13,1,0x01);//д���¶����Ͻǵ�
LCD_Write_Char(14,1,'C'); //д���ַ�C

while (1)         //��ѭ��
  {

if(ReadTempFlag==1)
 {
  ReadTempFlag=0;
  temp=ReadTemperature();
  temperature=(float)temp*0.0625;
  sprintf(displaytemp,"Temp1 %7.3f",temperature);//��ӡ�¶�ֵ
  LCD_Write_String(0,0,displaytemp);//��ʾ�ڶ���

  temp=ReadTemperature_a();
  temperature=(float)temp*0.0625;
  sprintf(displaytemp,"Temp2 %7.3f",temperature);//��ӡ�¶�ֵ
  LCD_Write_String(0,1,displaytemp);//��ʾ�ڶ���
  }	
 }
}

/*------------------------------------------------
                    ��ʱ����ʼ���ӳ���
------------------------------------------------*/
void Init_Timer0(void)
{
 TMOD |= 0x01;	  //ʹ��ģʽ1��16λ��ʱ����ʹ��"|"���ſ�����ʹ�ö����ʱ��ʱ����Ӱ��		     
 //TH0=0x00;	      //������ֵ
 //TL0=0x00;
 EA=1;            //���жϴ�
 ET0=1;           //��ʱ���жϴ�
 TR0=1;           //��ʱ�����ش�
}
/*------------------------------------------------
                 ��ʱ���ж��ӳ���
------------------------------------------------*/
void Timer0_isr(void) interrupt 1 
{
 static unsigned int num;
 TH0=(65536-2000)/256;		  //���¸�ֵ 2ms
 TL0=(65536-2000)%256;
 
 num++;
 if(num==600)        //
   {
    num=0;
    ReadTempFlag=1; //����־λ��1
	}
}


