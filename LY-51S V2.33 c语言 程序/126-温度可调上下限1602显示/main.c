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
#include "1602.h"
#include "delay.h"

#define KeyPort P3 //���尴���˿�

bit ReadTempFlag;//�����ʱ���־

void Init_Timer0(void);//��ʱ����ʼ��
unsigned char KeyScan(void);//����ɨ��
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
int temp,tempH=50,tempL=1;
float temperature;
unsigned char TempFlag=0;
char displaytemp[16],num;//������ʾ������ʱ�洢����

LCD_Init();           //��ʼ��Һ��
DelayMs(20);          //��ʱ�������ȶ�
LCD_Clear();          //����
Init_Timer0();
UART_Init();
Lcd_User_Chr();       //д���Զ����ַ�


while (1)         //��ѭ��
  {
 num=KeyScan();
 switch(num)
       {
	    case 1:if(tempH<127)tempH++;break;
		case 2:if(tempH>-55)tempH--;break;
		case 3:if(tempL<127)tempL++;break;
		case 4:if(tempL>-55)tempL--;break;
		default:break;
	   }
 
switch(TempFlag)
       {
	    case 0:    //����¶���������
		      sprintf(displaytemp,"H.%3d  L.%3d ",tempH,tempL);
              LCD_Write_String(0,1,displaytemp);//��ʾ�ڶ���
			  break;
		case 1:LCD_Write_String(0,1,"over  tempH    ");break;
		case 2:LCD_Write_String(0,1,"under  tempL   ");break;
		default:break;
	   }
if(ReadTempFlag==1)
 {
  ReadTempFlag=0;
  temp=ReadTemperature();


  temperature=temp*0.0625;

  temp>>=4;
 if(temp>tempH)
    TempFlag=1;  //���ڸ��±�־
 else if(temp<tempL)
    TempFlag=2;  //���ڵ��±�־
 else
   TempFlag=0;   //������ʾ��־

    

  sprintf(displaytemp,"Temp  %6.2f ",temperature);//��ӡ�¶�ֵ
  LCD_Write_String(0,0,displaytemp);//��ʾ��һ��
  LCD_Write_Char(13,0,0x01);//д���¶����Ͻǵ�
  LCD_Write_Char(14,0,'C'); //д���ַ�C
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
 if(num==400)        //
   {
    num=0;
    ReadTempFlag=1; //����־λ��1
	}
}

/*------------------------------------------------
����ɨ�躯��������ɨ���ֵ
------------------------------------------------*/
unsigned char KeyScan(void)
{
 unsigned char keyvalue;
 if(KeyPort!=0xff)
   {
    DelayMs(10);
    if(KeyPort!=0xff)
	   {
	    keyvalue=KeyPort;
	    while(KeyPort!=0xff);
		switch(keyvalue)
		{
		 case 0xfe:return 1;break;
		 case 0xfd:return 2;break;
		 case 0xfb:return 3;break;
		 case 0xf7:return 4;break;
		 case 0xef:return 5;break;
		 case 0xdf:return 6;break;
		 case 0xbf:return 7;break;
		 case 0x7f:return 8;break;
		 default:return 0;break;
		}
	  }
   }
   return 0;
}


