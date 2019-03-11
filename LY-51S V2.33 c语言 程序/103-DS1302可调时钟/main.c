/*-----------------------------------------------
  ���ƣ�DS1302ʱ���������ʾ
  ��̳��www.doflye.net
  ��д��shifang
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ�DS1302ʵʱʱ���������ʾ��ֻ��ʾʱ�䡣��ͨ��4�������Ӽ�Сʱ�����ӣ��������������������
        
------------------------------------------------*/
#include<reg52.h> //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���
#include "ds1302.h"

#define KeyPort P3 //���尴���˿�

#define DataPort P0 //�������ݶ˿� ����������DataPort ����P0 �滻

sbit LATCH1=P2^2;//��������ʹ�ܶ˿� ������
sbit LATCH2=P2^3;//                 λ����

bit ReadTimeFlag;//�����ʱ���־

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// ��ʾ����ֵ0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//�ֱ��Ӧ��Ӧ������ܵ���,��λ��
unsigned char TempData[8]; //�洢��ʾֵ��ȫ�ֱ���

void DelayUs2x(unsigned char t);//us����ʱ�������� 
void DelayMs(unsigned char t); //ms����ʱ
void Display(unsigned char FirstBit,unsigned char Num);//�������ʾ����
unsigned char KeyScan(void);//����ɨ��
void Init_Timer0(void);//��ʱ����ʼ��
/*------------------------------------------------
                    ������
------------------------------------------------*/
void main (void)
{
unsigned char num;                  

Init_Timer0();
Ds1302_Init();
while (1)         //��ѭ��
  {


 num=KeyScan();
 switch(num)
       {
	    case 1:time_buf1[4]++;if(time_buf1[4]==24)time_buf1[4]=0;    
              Ds1302_Write_Time();break; //����ʱ�� Сʱ ��1
		case 2:time_buf1[4]--;if(time_buf1[4]==255)time_buf1[4]=23;  
	          Ds1302_Write_Time();break; //����ʱ�� Сʱ��1
		case 3:time_buf1[5]++;if(time_buf1[5]==60)time_buf1[5]=0;     
	          Ds1302_Write_Time();break;//�ּ�1
		case 4:time_buf1[5]--;if(time_buf1[5]==255)time_buf1[5]=59; 
	          Ds1302_Write_Time();break; //�ּ�1
		default:break;
	   }
if(ReadTimeFlag==1)
{
  ReadTimeFlag=0;
  Ds1302_Read_Time();
 //���ݵ�ת���������ǲ��������0~9����ʾ,�����ݷֿ�
 TempData[0]=dofly_DuanMa[time_buf1[4]/10];	//ʱ		
 TempData[1]=dofly_DuanMa[time_buf1[4]%10];
 TempData[2]=0x40;					    	        //����"-"
 TempData[3]=dofly_DuanMa[time_buf1[5]/10]; //��
 TempData[4]=dofly_DuanMa[time_buf1[5]%10];
 TempData[5]=0x40;
 TempData[6]=dofly_DuanMa[time_buf1[6]/10]; //��
 TempData[7]=dofly_DuanMa[time_buf1[6]%10];	

 }	
  }
}
/*------------------------------------------------
 uS��ʱ����������������� unsigned char t���޷���ֵ
 unsigned char �Ƕ����޷����ַ���������ֵ�ķ�Χ��
 0~255 ����ʹ�þ���12M����ȷ��ʱ��ʹ�û��,������ʱ
 �������� T=tx2+5 uS 
------------------------------------------------*/
void DelayUs2x(unsigned char t)
{   
 while(--t);
}
/*------------------------------------------------
 mS��ʱ����������������� unsigned char t���޷���ֵ
 unsigned char �Ƕ����޷����ַ���������ֵ�ķ�Χ��
 0~255 ����ʹ�þ���12M����ȷ��ʱ��ʹ�û��
------------------------------------------------*/
void DelayMs(unsigned char t)
{
     
 while(t--)
 {
     //������ʱ1mS
     DelayUs2x(245);
	 DelayUs2x(245);
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
 if(num==50)        //����100ms
   {
    num=0;
    ReadTimeFlag=1; //����־λ��1
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

