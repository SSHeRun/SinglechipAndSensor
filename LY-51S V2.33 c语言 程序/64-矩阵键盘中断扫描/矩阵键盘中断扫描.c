/*-----------------------------------------------
  ���ƣ������������������� ʹ��������ɨ�� �жϷ�ʽ
  ��̳��www.doflye.net
  ��д��shifang
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ������������������ʽ��ͬ �������� ʹ������ɨ�跽��
        �жϷ�ʽ������Ч�ṩcpu����Ч�ʣ����а�������ʱ��ɨ�裬ƽʱ������ɨ�蹤��
------------------------------------------------*/
#include<reg52.h> //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���


#define DataPort P0 //�������ݶ˿� ����������DataPort ����P0 �滻
#define KeyPort  P1

sbit LATCH1=P2^2;//��������ʹ�ܶ˿� ������
sbit LATCH2=P2^3;//                 λ����

bit KeyPressFlag;//���尴����־λ

unsigned char code dofly_DuanMa[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,
		                  	         0x77,0x7c,0x39,0x5e,0x79,0x71};// ��ʾ����ֵ0~F
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//�ֱ��Ӧ��Ӧ������ܵ���,��λ��
unsigned char TempData[10]; //�洢��ʾֵ��ȫ�ֱ���

void DelayUs2x(unsigned char t);//us����ʱ�������� 
void DelayMs(unsigned char t); //ms����ʱ
void Display(unsigned char FirstBit,unsigned char Num);//�������ʾ����
unsigned char KeyScan(void);//����ɨ��
unsigned char KeyPro(void);
void Init_Timer0(void);//��ʱ����ʼ��
void Init_INT0(void);  //�ⲿ�ж�0��ʼ����
/*------------------------------------------------
                    ������
------------------------------------------------*/
void main (void)
{
unsigned char num,i,j;                  
unsigned char temp[8];
Init_Timer0();
Init_INT0();
while (1)         //��ѭ��
  {
  KeyPort=0xf0;// ��ֵ�����жϼ��
 if(KeyPressFlag==1)
 {
  KeyPressFlag=0;  //������־���㣬�Ա��´μ��
  num=KeyPro();    
  if(num!=0xff)
   {
    if(i<8)
      {
       temp[i]=dofly_DuanMa[num];
	   for(j=0;j<=i;j++)
          TempData[7-i+j]=temp[j];
       }
	i++;
	if(i==9)//���һ����������Ϊ������ ԭ��Ӧ��Ϊ8
      {
	  i=0;
      for(j=0;j<8;j++)//����
         TempData[j]=0;
       }
     }
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
 TH0=(65536-2000)/256;		  //���¸�ֵ 2ms
 TL0=(65536-2000)%256;
 
 Display(0,8);       // ���������ɨ��

}

/*------------------------------------------------
����ɨ�躯��������ɨ���ֵ
------------------------------------------------*/
unsigned char KeyScan(void)  //����ɨ�躯����ʹ��������ɨ�跨
{
 unsigned char Val;
 KeyPort=0xf0;//����λ�øߣ�����λ����
 if(KeyPort!=0xf0)//��ʾ�а�������
   {
    DelayMs(10);  //ȥ��
	if(KeyPort!=0xf0)
	  {           //��ʾ�а�������
    	KeyPort=0xfe; //����һ��
		if(KeyPort!=0xfe)
	  		{
			  Val=KeyPort&0xf0;
	  	      Val+=0x0e;
	  		  while(KeyPort!=0xfe);
			  DelayMs(10); //ȥ��
			  while(KeyPort!=0xfe);
	     	  return Val;
	        }
        KeyPort=0xfd; //���ڶ���
		if(KeyPort!=0xfd)
	  		{
			  Val=KeyPort&0xf0;
	  	      Val+=0x0d;
	  		  while(KeyPort!=0xfd);
			  DelayMs(10); //ȥ��
			  while(KeyPort!=0xfd);
	     	  return Val;
	        }
    	KeyPort=0xfb; //��������
		if(KeyPort!=0xfb)
	  		{
			  Val=KeyPort&0xf0;
	  	      Val+=0x0b;
	  		  while(KeyPort!=0xfb);
			  DelayMs(10); //ȥ��
			  while(KeyPort!=0xfb);
	     	  return Val;
	        }
    	KeyPort=0xf7; //��������
		if(KeyPort!=0xf7)
	  		{
			  Val=KeyPort&0xf0;
	  	      Val+=0x07;
	  		  while(KeyPort!=0xf7);
			  DelayMs(10); //ȥ��
			  while(KeyPort!=0xf7);
	     	  return Val;
	        }
     }
   }
  return 0xff;
}
/*------------------------------------------------
����ֵ������������ɨ��ֵ
------------------------------------------------*/
unsigned char KeyPro(void)
{
 switch(KeyScan())
 {
  case 0x7e:return 0;break;//0 ������Ӧ�ļ���ʾ���Ӧ����ֵ
  case 0x7d:return 1;break;//1
  case 0x7b:return 2;break;//2
  case 0x77:return 3;break;//3
  case 0xbe:return 4;break;//4
  case 0xbd:return 5;break;//5
  case 0xbb:return 6;break;//6
  case 0xb7:return 7;break;//7
  case 0xde:return 8;break;//8
  case 0xdd:return 9;break;//9
  case 0xdb:return 10;break;//a
  case 0xd7:return 11;break;//b
  case 0xee:return 12;break;//c
  case 0xed:return 13;break;//d
  case 0xeb:return 14;break;//e
  case 0xe7:return 15;break;//f
  default:return 0xff;break;
 }
}
/*------------------------------------------------
           �ⲿ�ж�0��ʼ��
------------------------------------------------*/
void Init_INT0(void)
{
  EA=1;          //ȫ���жϿ�
  EX0=1;         //�ⲿ�ж�0��
  IT0=1;         //���ش���
}

/*------------------------------------------------
                 �ⲿ�ж�0����
------------------------------------------------*/
void ISR_INT0(void) interrupt 0 
{
 KeyPressFlag=1;   //���������ж��������Խ��а���ɨ��
}
