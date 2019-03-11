#include <reg52.h>
sbit KEY1 = P3^2;
sbit PWM = P1^5;
unsigned char CYCLE; //�������� ������X��׼��ʱʱ�� �����10 ��������10 x 0.1ms
unsigned char PWM_ON ;//����ߵ�ƽʱ��
void delay(unsigned int cnt)
{
 while(--cnt);
}

main()
{
unsigned char PWM_Num;//���嵵λ
TMOD |=0x01;//��ʱ������ 1ms in 12M crystal
TH0=(65536-1000)/256; 
TL0=(65536-1000)%256;//��ʱ1mS 
IE= 0x82;  //���ж�
TR0=1;

CYCLE = 10;// ʱ����Ե��� �����10������ ����10ms 8λPWM����256��
while(1)
  {
if(!KEY1)
  {
   delay(30000);
   if(!KEY1)
     {
      PWM_Num++;
	  if(PWM_Num==4)PWM_Num=0;
       switch(PWM_Num){
       case 0:P0=0x06;PWM_ON=0;break;//�ߵ�ƽʱ�� 
       case 1:P0=0x5B;PWM_ON=4;break;
       case 2:P0=0x4F;PWM_ON=6;break;
       case 3:P0=0x66;PWM_ON=8;break;
       default:break;
     }
   }

  }
 }

}
/********************************/
/*      ��ʱ�ж�                */
/********************************/
void tim(void) interrupt 1 using 1
{
static unsigned char count; //
TH0=(65536-1000)/256; 
TL0=(65536-1000)%256;//��ʱ1mS 

if (count==PWM_ON)
    {
     PWM = 1; //���� 
    }
  count++;
if(count == CYCLE)
    {
    count=0;
	if(PWM_ON!=0) //�������ʱ����0 ����ԭ��״̬
	   PWM = 0;//����

    }

}
