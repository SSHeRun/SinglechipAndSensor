/*-----------------------------------------------
  ���ƣ�18B20�¶ȴ�����
  ��վ��www.doflye.net
  ��д��shifang
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ�18B20�����¶ȼ���Ӧ����������
------------------------------------------------*/
#include"delay.h"
#include"18b20a.h"
/*------------------------------------------------
                    18b20��ʼ��
------------------------------------------------*/
bit Init_DS18B20_a(void)
{
 bit dat=0;
 DQ1 = 1;    //DQ1��λ
 DelayUs2x(5);   //������ʱ
 DQ1 = 0;         //��Ƭ����DQ1����
 DelayUs2x(200); //��ȷ��ʱ ���� 480us С��960us
 DelayUs2x(200);
 DQ1 = 1;        //��������
 DelayUs2x(50); //15~60us �� ����60-240us�Ĵ�������
 dat=DQ1;        //���x=0���ʼ���ɹ�, x=1���ʼ��ʧ��
 DelayUs2x(25); //������ʱ����
 return dat;
}

/*------------------------------------------------
                    ��ȡһ���ֽ�
------------------------------------------------*/
unsigned char ReadOneChar_a(void)
{
unsigned char i=0;
unsigned char dat = 0;
for (i=8;i>0;i--)
 {
  DQ1 = 0; // �������ź�
  dat>>=1;
  DQ1 = 1; // �������ź�
  if(DQ1)
   dat|=0x80;
  DelayUs2x(25);
 }
 return(dat);
}
/*------------------------------------------------
                    д��һ���ֽ�
------------------------------------------------*/
void WriteOneChar_a(unsigned char dat)
{
 unsigned char i=0;
 for (i=8; i>0; i--)
 {
  DQ1 = 0;
  DQ1 = dat&0x01;
  DelayUs2x(25);
  DQ1 = 1;
  dat>>=1;
 }
DelayUs2x(25);
}

/*------------------------------------------------
                    ��ȡ�¶�
------------------------------------------------*/
unsigned int ReadTemperature_a(void)
{
unsigned char a=0;
unsigned int b=0;
unsigned int t=0;
Init_DS18B20_a();
WriteOneChar_a(0xCC); // ����������кŵĲ���
WriteOneChar_a(0x44); // �����¶�ת��
DelayMs(10);
Init_DS18B20_a();
WriteOneChar_a(0xCC); //����������кŵĲ��� 
WriteOneChar_a(0xBE); //��ȡ�¶ȼĴ����ȣ����ɶ�9���Ĵ����� ǰ���������¶�
a=ReadOneChar_a();   //��λ
b=ReadOneChar_a();   //��λ

b<<=8;
t=a+b;

return(t);
}
