/*-----------------------------------------------
  ���ƣ�DS1302ʱ���������ʾ ���ڸ���
  ��̳��www.doflye.net
  ��д��shifang
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ�DS1302ʵʱʱ���������ʾ��ʱ��������л���ʾ ���°�����ѭ���л� 
        ʱ���ʽxx-xx-xx
		���ڸ�ʽxx-xx-xx
		�����ʽ-x-   xx
		ͨ��dofly�Դ��Ĵ��ڵ���������򿪴��ڣ�������Ĭ��9600���������ʱ�伴�ɣ�������У����¿����帴λ���¸���
------------------------------------------------*/
#include<reg52.h> //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���
#include "ds1302.h"

#define KeyPort P3 //���尴���˿�

#define DataPort P0 //�������ݶ˿� ����������DataPort ����P0 �滻

sbit LATCH1=P2^2;//��������ʹ�ܶ˿� ������
sbit LATCH2=P2^3;//                 λ����

bit ReadTimeFlag;//�����ʱ���־
bit SetFlag;     //����ʱ���־λ
unsigned char time_buf2[16];

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// ��ʾ����ֵ0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//�ֱ��Ӧ��Ӧ������ܵ���,��λ��
unsigned char TempData[8]; //�洢��ʾֵ��ȫ�ֱ���

void DelayUs2x(unsigned char t);//us����ʱ�������� 
void DelayMs(unsigned char t); //ms����ʱ
void Display(unsigned char FirstBit,unsigned char Num);//�������ʾ����
unsigned char KeyScan(void);//����ɨ��
void Init_Timer0(void);//��ʱ����ʼ��
void UART_Init(void);
/*------------------------------------------------
                    ������
------------------------------------------------*/
void main (void)
{
unsigned char i,num,displaynum;                  

Init_Timer0();
Ds1302_Init();
UART_Init();
while (1)         //��ѭ��
  {


if(SetFlag)     //������յ�������Ϣ�����ʱ��
  {
	for(i=0;i<8;i++)
		{
		time_buf1[i]=time_buf2[2*i]*10+time_buf2[2*i+1];
		}//�������ϣ���2���� 1��5���ϳ�15
		Ds1302_Write_Time();
		SetFlag=0;       //ʱ����Ϣ���º��־λ����
   }

 num=KeyScan();
if(num==1)
  {
   displaynum++;
   if(displaynum==3)
      displaynum=0;
  }
if(ReadTimeFlag==1)
{
  ReadTimeFlag=0;
  Ds1302_Read_Time();
if(displaynum==0) //��ʾʱ��
 {
 TempData[0]=dofly_DuanMa[time_buf1[4]/10];//ʱ			//���ݵ�ת���������ǲ��������0~9����ʾ,�����ݷֿ�
 TempData[1]=dofly_DuanMa[time_buf1[4]%10];
 TempData[2]=0x40;					    	        //����"-"
 TempData[3]=dofly_DuanMa[time_buf1[5]/10];//��
 TempData[4]=dofly_DuanMa[time_buf1[5]%10];
 TempData[5]=0x40;
 TempData[6]=dofly_DuanMa[time_buf1[6]/10];//��
 TempData[7]=dofly_DuanMa[time_buf1[6]%10];	
 } 
else if(displaynum==1)//��ʾ����
 { 
 TempData[0]=dofly_DuanMa[time_buf1[1]/10];//��			//���ݵ�ת���������ǲ��������0~9����ʾ,�����ݷֿ�
 TempData[1]=dofly_DuanMa[time_buf1[1]%10];
 TempData[2]=0x40;					    	        //����"-"
 TempData[3]=dofly_DuanMa[time_buf1[2]/10];//��
 TempData[4]=dofly_DuanMa[time_buf1[2]%10];
 TempData[5]=0x40;
 TempData[6]=dofly_DuanMa[time_buf1[3]/10];//��
 TempData[7]=dofly_DuanMa[time_buf1[3]%10];	
 }
else if(displaynum==2)//��ʾ��  ��
 {
 TempData[0]=0x40;				//���ݵ�ת���������ǲ��������0~9����ʾ,�����ݷֿ�
 TempData[1]=dofly_DuanMa[time_buf1[7]%10];//��
 TempData[2]=0x40;	//����"-"
 TempData[3]=0;
 TempData[4]=0;
 TempData[5]=0;
 TempData[6]=dofly_DuanMa[time_buf1[6]/10];//��
 TempData[7]=dofly_DuanMa[time_buf1[6]%10];	
 }
 }	
 }
}
/*------------------------------------------------
              ����ͨѶ��ʼ��
------------------------------------------------*/
void UART_Init(void)
{
    SCON  = 0x50;		        // SCON: ģʽ 1, 8-bit UART, ʹ�ܽ���  
    TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit ��װ
    TH1   = 0xFD;               // TH1:  ��װֵ 9600 ������ ���� 11.0592MHz  
    TR1   = 1;                  // TR1:  timer 1 ��                         
    EA    = 1;                  //�����ж�
    ES    = 1;                  //�򿪴����ж�
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
/*------------------------------------------------
                  �����жϳ���
------------------------------------------------*/
void UART_SER (void) interrupt 4 //�����жϷ������
{
    unsigned char Temp;          //������ʱ���� 
    unsigned char i;
    if(RI)                        //�ж��ǽ����жϲ���
     {
	  RI=0;                      //��־λ����
	  Temp=SBUF;                 //���뻺������ֵ
	  time_buf2[i]=Temp&0x0F;
	  i++;
	  if(i==16)                  //��������16���ַ���Ϣ
	   {
	    i=0;
		SetFlag=1;               //������ɱ�־λ��1
	   }
      SBUF=Temp; //�ѽ��յ���ֵ�ٷ��ص��Զ�
	 }
   if(TI)  //����Ƿ��ͱ�־λ������
     TI=0;
} 
