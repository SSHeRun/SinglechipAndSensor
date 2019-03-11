/*-----------------------------------------------
  ���ƣ������������Ƽ̵�������
  ��̳��www.doflye.net
  ��д��shifang
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ���Ӧ�ļ̵����ӿ����öŰ������ӵ�uln2003�̵������ƶ�
        ͨ������2���̵���ѭ������״̬��ģ��ϴ�»���Ъ����ת
        һ����ת3s��ֹͣ2s��Ȼ��ת3s��ֹͣ2s��ѭ����������
------------------------------------------------*/
#include<reg52.h> //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���

#define KeyPort P3

#define DataPort P0 //�������ݶ˿� ����������DataPort ����P0 �滻
sbit LATCH1=P2^2;//��������ʹ�ܶ˿� ������
sbit LATCH2=P2^3;//                 λ����

sbit RELAY1 = P1^1;//����̵����ź�����˿�1
sbit RELAY2 = P1^2;//����̵����ź�����˿�2
/*------------------------------------------------
                  ȫ�ֱ���
------------------------------------------------*/
unsigned char hour,minute,second;//����ʱ����

bit UpdateTimeFlag;//�����ʱ���־

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// ��ʾ����ֵ0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//�ֱ��Ӧ��Ӧ������ܵ���,��λ��
unsigned char TempData[8]; //�洢��ʾֵ��ȫ�ֱ���
/*------------------------------------------------
                  ��������
------------------------------------------------*/
void DelayUs2x(unsigned char t);//us����ʱ�������� 
void DelayMs(unsigned char t); //ms����ʱ
void Display(unsigned char FirstBit,unsigned char Num);//�������ʾ����
unsigned char KeyScan(void);//����ɨ��
void Init_Timer0(void);//��ʱ����ʼ��
void Stop(void);
void Clockwise (void);
void Anticlockwise(void);
/*------------------------------------------------
                    ������
------------------------------------------------*/
void main (void)
{
unsigned char num;                  
Init_Timer0();    //��ʼ����ʱ��0����Ҫ��������ܶ�̬ɨ��
Stop();           //��ʼ��ϴ�»�ֹͣת��
while (1)         //��ѭ��
  {
 num=KeyScan();    //ѭ�����ð���ɨ��

 if(num==1)        //��ʱ��������
   {
    if(minute<99)
	  minute++;
   }
 else if(num==2)   //��ʱ���ȼ�С
   {
    if(minute>0)
	  minute--;
   }
 if(UpdateTimeFlag==1)//��ʱˢ����ʾ����
   {
    UpdateTimeFlag=0;
	TempData[2]=dofly_DuanMa[minute/10];//�ֽ���ʾ��Ϣ����Ҫ��ʾ68����68/10=6  68%10=8 
    TempData[3]=dofly_DuanMa[minute%10];//�ֽ���ʾ��Ϣ����Ҫ��ʾ68����68/10=6  68%10=8  
    TempData[4]=0x40;
    TempData[5]=dofly_DuanMa[second/10];//�ֽ���ʾ��Ϣ����Ҫ��ʾ68����68/10=6  68%10=8 
    TempData[6]=dofly_DuanMa[second%10];//�ֽ���ʾ��Ϣ����Ҫ��ʾ68����68/10=6  68%10=8 

	if((minute==0)&&(second==0))//��ʱʱ�䵽��ϴ�»�ֹͣת��
      {
      Stop();
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
 ������� FirstBit ��ʾ��Ҫ��ʾ�ĵ�һλ���縳ֵ2��
 ʾ�ӵ���������ܿ�ʼ��ʾ��������0��ʾ�ӵ�һ����ʾ��
 Num��ʾ��Ҫ��ʾ��λ��������Ҫ��ʾ99��λ��ֵ���ֵ
 ����2
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
 static unsigned int num,i,times;
 TH0=(65536-2000)/256;		  //���¸�ֵ 2ms
 TL0=(65536-2000)%256;
 
 Display(0,8);                // ���������ɨ��
 i++;
 if(i==20)          //20ms����һ��
    {
	i=0;
	UpdateTimeFlag=1; //����ʱ��־λ��1
	}
 num++;
 if(num==500)        //����1s
   {
    num=0;
   
    ///////////////////////////////////////
	//ִ������ת������
	switch(times)
	   {
	    case 1:Clockwise ();   break;//��ת3s
		case 4:Stop();         break;//ֹͣ2s
		case 6:Anticlockwise();break;//��ת3s
		case 9:Stop();         break;//ֹͣ2s
		case 11:times=0;       break;//����ѭ����������
		default:break;
	   }
	times++;
   ///////////////////////////////////////////////////////
    if((minute!=0)||second)//������Ӻ��붼Ϊ0�������м�ʱ
       second--;//���1
    if(second==0xff)//���=0���ټ�1��ֵ59����00������ʾ59
	   {
	   second=59;
	   if(minute>0)//����ʱ����
	     {
	      minute--;
	      }

	   }
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

/*------------------------------------------------
                 ˳ʱ��ת��
------------------------------------------------*/
void Clockwise (void)
{
 RELAY1=1;    //�̵���1���ϣ���ͨ������Ȧ
 RELAY2=0;
}
/*------------------------------------------------
                 ��ʱ��ת��
------------------------------------------------*/
void Anticlockwise(void)
{
  RELAY1=0;
  RELAY2=1;   //�̵���2���ϣ���ͨ������Ȧ
}

/*------------------------------------------------
                 ֹͣת��
------------------------------------------------*/
void Stop(void)
{
  RELAY1=0;   
  RELAY2=0;
}
