/*-----------------------------------------------
  ���ƣ����̵�
  ��̳��www.doflye.net
  ��д��shifang
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ�1������Ŀ�ģ�ʹ�ö�ʱ��ѧϰ ����ʱ ���̵�ԭ�� 
           ��Ҫ�����뵹��ʱ��ͬ
        2��Ӳ��Ҫ������ܡ�����12M
		3��P1��������LED������ʾ���̵�
		4�������������ʾʱ��
------------------------------------------------*/
#include<reg52.h>//����ͷ�ļ���һ���������Ҫ�Ķ���
//ͷ�ļ��������⹦�ܼĴ����Ķ���

sbit LACTCH1=P2^2;    
sbit LACTCH2=P2^3;
/*------------------------------------------------
                 ȫ�ֱ���
------------------------------------------------*/
bit red,green,yellow,turnred;//������̻ƵƱ�־λ



code unsigned char tab[]=
{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f}; 
			                //��������� 0-9 

unsigned char Dis_Shiwei;//����ʮλ
unsigned char Dis_Gewei; //�����λ
/*------------------------------------------------
              
------------------------------------------------*/
void delay(unsigned int cnt)
{
 while(--cnt);
}
/*------------------------------------------------
              ������
------------------------------------------------*/
main()
{
TMOD |=0x01;//��ʱ������ 10ms in 12M crystal
TH0=0xd8;
TL0=0xf0;
IE= 0x82;   //���ж�
TR0=1;

P1=0xfc;    //�����
red =1;
while(1)
  {
  P0=Dis_Shiwei;//��ʾʮλ
  LACTCH1=1;    //����
  LACTCH1=0;
  P0=0xfe; 
  LACTCH2=1;    // ����
  LACTCH2=0;
  delay(300);   //������ʱ
  P0=Dis_Gewei; //��ʾ��λ
  LACTCH1=1;    //����
  LACTCH1=0;
  P0=0xfd;
  LACTCH2=1;    // ����
  LACTCH2=0;
  delay(300);
  }

}
/*------------------------------------------------
             ��ʱ��0�жϺ���
------------------------------------------------*/
void tim(void) interrupt 1 
{
static unsigned char second=60,count; //��ֵ99

TH0=0xd8;//���¸�ֵ
TL0=0xf0;
count++;
if (count==100)
    {
    count=0;
    second--;//���1
	if(second==0)
	   { 
       if(red)
	     {
		  red=0;yellow=1;
          second=5;
		  P1=0xF3;//�Ƶ���5��
		 }
	   else if(yellow && !turnred)
	     {
		  yellow=0;green=1;
          second=50;
		  P1=0xCF;//�̵���50��
		 }
	   else if(green)
	     {
		 yellow=1;green=0;
         second=5;
		 P1=0xF3;//�Ƶ���5��
		 turnred=1;
		 }
       else if(yellow && turnred)
	     {
	     red=1;yellow=0;
		 P1=0xFC;//�����60��
         second=60;
		 turnred=0;
		 }

	   }
	Dis_Shiwei=tab[second/10];//ʮλ��ʾֵ����
    Dis_Gewei=tab[second%10]; //��λ��ʾ����
    
    }
}