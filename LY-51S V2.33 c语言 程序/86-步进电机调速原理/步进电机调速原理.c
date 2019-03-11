/*-----------------------------------------------
  ���ƣ��������
  ��̳��www.doflye.net
  ��д��shifang
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ����������ڲ���4�ಽ������������� 2������ �����ٶ� 
        ʹ��1-2������
		�������ʾ 1-5�ٶȵȼ�������Խ���ٶ�ԽС
 ------------------------------------------------*/

#include <reg52.h>

#define KeyPort P3
#define DataPort P0 //�������ݶ˿� ����������DataPort ����P0 �滻
sbit LATCH1=P2^2;//��������ʹ�ܶ˿� ������
sbit LATCH2=P2^3;//                 λ����

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// ��ʾ����ֵ0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//�ֱ��Ӧ��Ӧ������ܵ���,��λ��
unsigned char TempData[8]; //�洢��ʾֵ��ȫ�ֱ���

sbit A1=P1^0; //���岽��������Ӷ˿�
sbit B1=P1^1;
sbit C1=P1^2;
sbit D1=P1^3;


#define Coil_AB1 {A1=1;B1=1;C1=0;D1=0;}//AB��ͨ�磬������ϵ�
#define Coil_BC1 {A1=0;B1=1;C1=1;D1=0;}//BC��ͨ�磬������ϵ�
#define Coil_CD1 {A1=0;B1=0;C1=1;D1=1;}//CD��ͨ�磬������ϵ�
#define Coil_DA1 {A1=1;B1=0;C1=0;D1=1;}//D��ͨ�磬������ϵ�
#define Coil_A1 {A1=1;B1=0;C1=0;D1=0;}//A��ͨ�磬������ϵ�
#define Coil_B1 {A1=0;B1=1;C1=0;D1=0;}//B��ͨ�磬������ϵ�
#define Coil_C1 {A1=0;B1=0;C1=1;D1=0;}//C��ͨ�磬������ϵ�
#define Coil_D1 {A1=0;B1=0;C1=0;D1=1;}//D��ͨ�磬������ϵ�
#define Coil_OFF {A1=0;B1=0;C1=0;D1=0;}//ȫ���ϵ�

unsigned char Speed=1;
bit StopFlag;
void Display(unsigned char FirstBit,unsigned char Num);
void Init_Timer0(void);
unsigned char KeyScan(void);
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
                    ������
------------------------------------------------*/
main()
{
 unsigned int i=512;//��תһ��ʱ��
 unsigned char num;
 Init_Timer0();
 Coil_OFF
 while(1)  //����
  { 
   num=KeyScan();    //ѭ�����ð���ɨ��
   if(num==1)//��һ������,�ٶȵȼ�����
      {
	   if(Speed<5)
	   Speed++;
	  }	
   else if(num==2)//�ڶ����������ٶȵȼ���С
      {
	   if(Speed>1)
	   Speed--;
	  }	
 
  TempData[1]=dofly_DuanMa[Speed%10];
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
 PT0=1;           //���ȼ���
}
/*------------------------------------------------
                 ��ʱ���ж��ӳ���
------------------------------------------------*/
void Timer0_isr(void) interrupt 1 
{
 static unsigned char times,i;
 TH0=(65536-1000)/256;		  //���¸�ֵ 1ms
 TL0=(65536-1000)%256;
 
 Display(0,8);

  if(times==Speed)
   {
   times=0;
   switch(i)
       {
	    case 0:Coil_A1;i++;break;
		case 1:Coil_AB1;i++;break;
		case 2:Coil_B1;i++;break;
		case 3:Coil_BC1;i++;break;
		case 4:Coil_C1;i++;break;
		case 5:Coil_CD1;i++;break;
		case 6:Coil_D1;i++;break;
		case 7:Coil_DA1;i++;break;
		case 8:i=0;break;
	    default:break;
	   }
    }
  else
     times++;
 
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

