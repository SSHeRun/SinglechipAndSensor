/*-----------------------------------------------
  ���ƣ�DS18b20�������ʾ 
  ��̳��www.doflye.net
  ��д��shifang
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ���ʾ��ʽ ���� xxx.x C 
        ������ʾ���¶�
        �������ʾͬʱͨ�����ڷ��͵����Զ���ʾ
        ʹ��dofly���׵Ĳ������ ����ʹ��11.0592M
------------------------------------------------*/
#include<reg52.h> //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���
#include<stdio.h>
#include "18b20.h"

#define DataPort P0 //�������ݶ˿� ����������DataPort ����P0 �滻

sbit LATCH1=P2^2;//��������ʹ�ܶ˿� ������
sbit LATCH2=P2^3;//                 λ����

bit ReadTempFlag;//�����ʱ���־

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// ��ʾ����ֵ0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//�ֱ��Ӧ��Ӧ������ܵ���,��λ��
unsigned char TempData[8]; //�洢��ʾֵ��ȫ�ֱ���


void Display(unsigned char FirstBit,unsigned char Num);//�������ʾ����
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
	TI =1;
}
/*------------------------------------------------
                    ������
------------------------------------------------*/
void main (void)
{                  
unsigned int TempH,TempL,temp;
Init_Timer0();
UART_Init();
while (1)         //��ѭ��
  {


if(ReadTempFlag==1)
{
  ReadTempFlag=0;
  temp=ReadTemperature();
  if(temp&0x8000)
     {
     TempData[0]=0x40;//���ű�־
     temp=~temp;  // ȡ����1
	 temp +=1;
	 }
  else
     TempData[0]=0;

     TempH=temp>>4;
     TempL=temp&0x0F;
     TempL=TempL*6/10;//С�����ƴ���
     printf("%d.%d\n",TempH,TempL);	
    if(TempH/100==0)
	  TempData[1]=0;
	else
      TempData[1]=dofly_DuanMa[TempH/100]; //ʮλ�¶�
    if((TempH/100==0)&&((TempH%100)/10==0))//����
	  TempData[2]=0;
	else
     TempData[2]=dofly_DuanMa[(TempH%100)/10]; //ʮλ�¶�
     TempData[3]=dofly_DuanMa[(TempH%100)%10]|0x80; //��λ�¶�,��С����
     TempData[4]=dofly_DuanMa[TempL];
	 TempData[6]=0x39;         //��ʾC����
  }	
 }
}

/*------------------------------------------------
 ��ʾ���������ڶ�̬ɨ�������
 ������� FirstBit ��ʾ��Ҫ��ʾ�ĵ�һλ���縳ֵ2��ʾ�ӵ���������ܿ�ʼ��ʾ
 ������0��ʾ�ӵ�һ����ʾ��
 Num��ʾ��Ҫ��ʾ��λ��������Ҫ��ʾ99��λ��ֵ���ֵ����2
------------------------------------------------*/
void Display(unsigned char FirstBit,unsigned char Num)
{
      static unsigned char i=0;
	  

	   DataPort=0;   //������ݣ���ֹ�н�����Ӱ
       LATCH1=1;     //������
       LATCH1=0;

       DataPort=dofly_WeiMa[i+FirstBit]; //ȡλ�� 
       LATCH2=1;     //λ����
       LATCH2=0;

       DataPort=TempData[i]; //ȡ��ʾ���ݣ�����
       LATCH1=1;     //������
       LATCH1=0;
       
	   i++;
       if(i==Num)
	      i=0;


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
 
 Display(0,8);       // ���������ɨ��
 num++;
 if(num==300)        //
   {
    num=0;
    ReadTempFlag=1; //����־λ��1
	}
}


