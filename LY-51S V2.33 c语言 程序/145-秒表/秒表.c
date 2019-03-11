/*-----------------------------------------------
  ���ƣ��������ʾ�������������
  ��̳��www.doflye.net
  ��д��shifang
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ��жϰ������ƣ��������ʾ,�ж�0���Ƽ�ʱ��ֹͣ���ж�1����
------------------------------------------------*/
#include<reg52.h> //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���

unsigned int msecond,second;//����ȫ�ֱ���
bit GoFlag;//����ֹͣ����ʱ��־

#define DataPort P0 //�������ݶ˿� ����������DataPort ����P0 �滻
sbit LATCH1=P2^2;//��������ʹ�ܶ˿� ������
sbit LATCH2=P2^3;//                 λ����

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// ��ʾ����ֵ0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//�ֱ��Ӧ��Ӧ������ܵ���,��λ��
unsigned char TempData[8]; //�洢��ʾֵ��ȫ�ֱ���

void DelayUs2x(unsigned char t);//�������� 
void DelayMs(unsigned char t);
void Display(unsigned char FirstBit,unsigned char Num);
void Init_Timer0(void);
void CLR(void);
/*------------------------------------------------
                    ������
------------------------------------------------*/
void main (void)
{
unsigned char num=0;                  

  
  EX0=1;         //�ⲿ�ж�0��
  IT0=1;         //IT1=0��ʾ���ش���
  EX1=1;         //�ⲿ�ж�1��
  IT1=1;         //IT1=1��ʾ���ش���
  Init_Timer0();
while (1)         //��ѭ��
  {

	   TempData[0]=dofly_DuanMa[second/10];//�ֽ���ʾ��Ϣ����Ҫ��ʾ68����68/10=6  68%10=8  
	   TempData[1]=dofly_DuanMa[second%10];
       TempData[2]=0x40;// "-"
	   TempData[3]=dofly_DuanMa[msecond/10];//�ֽ���ʾ��Ϣ����Ҫ��ʾ68����68/10=6  68%10=8  
	   TempData[4]=dofly_DuanMa[msecond%10];
	
     //��ѭ�������������Ҫһֱ�����ĳ���
	
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
 static unsigned char num;
 TH0=(65536-2000)/256;		  //���¸�ֵ 2ms
 TL0=(65536-2000)%256;
 
 Display(0,8);
 if(GoFlag)
   {
   num++;
  if(num==5)
    {
	num=0;
   msecond++;
   if (msecond==100)//100��1
     {
     msecond=0;
     second++;//���1
	 if(second==100)
	   second=0;
     }
   }
  }
}
/*------------------------------------------------
              �ⲿ�ж�0����
------------------------------------------------*/
void ISR_INT0(void) interrupt 0 
{
 GoFlag=!GoFlag; //�����ⲿ�жϴ򿪺͹رն�ʱ��־ ���ڿ�ʼ��ֹͣ��ʱ
}
/*------------------------------------------------
              �ⲿ�ж�1����
------------------------------------------------*/
void ISR_INT1(void) interrupt 2 
{
if(GoFlag==0)//ֹͣʱ�ſ�������
   CLR();
}/*------------------------------------------------
              ��ֵ����
------------------------------------------------*/
void CLR(void)
{
 second=0;      //�����ⲿ�ж�����
 msecond=0;
}

