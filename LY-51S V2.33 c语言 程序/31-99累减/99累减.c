/*-----------------------------------------------
  ���ƣ�2λ����ܶ�̬ɨ����ʾ�仯����
  ��̳��www.doflye.net
  ��д��shifang
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ�2λ����ֱܷ���ʾ��ͬ���֣�����ɨ����ʾ��ʽ��Ϊ��̬ɨ�裬����ͣ�仯��ֵ
------------------------------------------------*/
#include<reg52.h> //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���

#define DataPort P0 //�������ݶ˿� ����������DataPort ����P0 �滻
sbit LATCH1=P2^2;//��������ʹ�ܶ˿� ������
sbit LATCH2=P2^3;//                 λ����

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// ��ʾ����ֵ0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//�ֱ��Ӧ��Ӧ������ܵ���,��λ��
unsigned char TempData[8]; //�洢��ʾֵ��ȫ�ֱ���
void Delay(unsigned int t); //��������
void Display(unsigned char FirstBit,unsigned char Num);
/*------------------------------------------------
                    ������
------------------------------------------------*/
main()
{
 unsigned char num=99;
 unsigned int j;
 while(1)
      {
	   j++;
	   if(j==500)  //��⵱ǰ��ֵ��ʾ��һС��ʱ�����Ҫ��ʾ����ֵ-1��ʵ��������ʾ�ı仯
	     {
		  j=0;
		  if(num>0)//������ʾ0~99
		    num--;
		  else
		    num=99;

		 }
	   TempData[0]=dofly_DuanMa[num/10];//�ֽ���ʾ��Ϣ����Ҫ��ʾ68����68/10=6  68%10=8  
	   TempData[1]=dofly_DuanMa[num%10];
	   Display(2,2);
      }
}
/*------------------------------------------------
 ��ʱ����������������� unsigned int t���޷���ֵ
 unsigned int �Ƕ����޷������α�������ֵ�ķ�Χ��
 0~65535
------------------------------------------------*/
void Delay(unsigned int t)
{
 while(--t);
}
/*------------------------------------------------
 ��ʾ���������ڶ�̬ɨ�������
 ������� FirstBit ��ʾ��Ҫ��ʾ�ĵ�һλ���縳ֵ2��ʾ�ӵ���������ܿ�ʼ��ʾ
 ������0��ʾ�ӵ�һ����ʾ��
 Num��ʾ��Ҫ��ʾ��λ��������Ҫ��ʾ99��λ��ֵ���ֵ����2
------------------------------------------------*/
void Display(unsigned char FirstBit,unsigned char Num)
{
      unsigned char i;
	  
	  for(i=0;i<Num;i++)
	   { 
	   DataPort=0;   //������ݣ���ֹ�н�����Ӱ
       LATCH1=1;     //������
       LATCH1=0;

       DataPort=dofly_WeiMa[i+FirstBit]; //ȡλ�� 
       LATCH2=1;     //λ����
       LATCH2=0;

       DataPort=TempData[i]; //ȡ��ʾ���ݣ�����
       LATCH1=1;     //������
       LATCH1=0;
       
	   Delay(200); // ɨ���϶��ʱ��ʱ��̫������˸��̫�̻������Ӱ

       }

}