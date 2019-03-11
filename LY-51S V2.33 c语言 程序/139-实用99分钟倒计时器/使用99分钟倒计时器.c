/*-----------------------------------------------
  ���ƣ�99���ӵ���ʱ�������ʾ
  ��̳��www.doflye.net
  ��д��shifang
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ�99���ӵ���ʱ��������ͨ�������ϵ��µ�ʱ�䣬��ʱ����ʱ���ȷ�������
        ���е㰴����������
------------------------------------------------*/
#include<reg52.h> //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���

#define DataPort P0 //�������ݶ˿� ����������DataPort ����P0 �滻

sbit LATCH1=P2^2;//��������ʹ�ܶ˿� ������
sbit LATCH2=P2^3;//                 λ����

sbit BEEP = P1^1;//�������ȶ˿� 
bit BeepFlag;

sbit KEY_ADD=P3^0;  //���尴������˿�
sbit KEY_DEC=P3^1;

unsigned char hour,minute,second;//����ʱ����

bit UpdateTimeFlag;//�����ʱ���־

unsigned char code dofly_DuanMa[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// ��ʾ����ֵ0~9
unsigned char code dofly_WeiMa[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//�ֱ��Ӧ��Ӧ������ܵ���,��λ��
unsigned char TempData[8]; //�洢��ʾֵ��ȫ�ֱ���

void DelayUs2x(unsigned char t);//us����ʱ�������� 
void DelayMs(unsigned char t); //ms����ʱ
void Display(unsigned char FirstBit,unsigned char Num);//�������ʾ����
void Init_Timer0(void);//��ʱ����ʼ��
void  Beep_OFF(void);
void  Beep_ON(void);
void Display_Data_Operation(void);
/*------------------------------------------------
                ��ʾ���ݴ�����
------------------------------------------------*/
void Display_Data_Operation(void)
{
 TempData[2]=dofly_DuanMa[minute/10];//�ֽ���ʾ��Ϣ����Ҫ��ʾ68����68/10=6  68%10=8 
 TempData[3]=dofly_DuanMa[minute%10];//�ֽ���ʾ��Ϣ����Ҫ��ʾ68����68/10=6  68%10=8  
 TempData[4]=0x40;
 TempData[5]=dofly_DuanMa[second/10];//�ֽ���ʾ��Ϣ����Ҫ��ʾ68����68/10=6  68%10=8 
 TempData[6]=dofly_DuanMa[second%10];//�ֽ���ʾ��Ϣ����Ҫ��ʾ68����68/10=6  68%10=8 
}
/*------------------------------------------------
                    ������
------------------------------------------------*/
void main (void)
{
unsigned char key_press_num;                  

Init_Timer0();

while (1)         //��ѭ��
  {

 if(!KEY_ADD)  //�����⵽�͵�ƽ��˵����������
    {
	 DelayMs(10); //��ʱȥ����һ��10-20ms
     if(!KEY_ADD)     //�ٴ�ȷ�ϰ����Ƿ��£�û�а������˳�
	   {
	   while(!KEY_ADD)
	    {
		key_press_num++;
        DelayMs(10);           //10x200=2000ms=2s 
		if(key_press_num==200) //��Լ2s
		  {
		   key_press_num=0;    //����ﵽ��������׼ ����볤��������
           while(!KEY_ADD)     //��������ʶ���Ƿ񰴼����ڰ��£��������ִ����ض����������˳�
		       {
		       	if(minute<99)    //�Ӳ���
	              minute++;
				//��ʱ����ʾ���ݴ������ȥ������2�䴦����Ϣ��ʵ���Ͽ���������Ч�������ǿ�������Ч��
				//�û������������β���
                Display_Data_Operation();
                DelayMs(50);//���ڵ��ڳ���ѭ���������ٶ�
				}
		    }
		}
      key_press_num=0;//��ֹ�ۼ���ɴ���ʶ��
		   if(minute<99)    //�Ӳ���
	          minute++;
	   }
	}

 if(!KEY_DEC)  //�����⵽�͵�ƽ��˵����������
    {
	 DelayMs(10); //��ʱȥ����һ��10-20ms
     if(!KEY_DEC)     //�ٴ�ȷ�ϰ����Ƿ��£�û�а������˳�
	   {
	   while(!KEY_DEC)
	    {
		key_press_num++;
        DelayMs(10);
		if(key_press_num==200) //��Լ2s
		  {
		   key_press_num=0;
           while(!KEY_DEC)
		       {
		       	if(minute>0)  //������
	               minute--;
                Display_Data_Operation();
                DelayMs(50);//���ڵ��ڳ���ѭ���������ٶ�
				}
		    }
		}
      key_press_num=0;//��ֹ�ۼ���ɴ���ʶ��
	       if(minute>0)  //������
	          minute--;
	 	
	   }
	}

if(UpdateTimeFlag==1)
 {
 UpdateTimeFlag=0;
					    	       
 Display_Data_Operation();	

 if((minute==0)&&(second==0))//�����������������
   {
    Beep_ON();
   }
 else
    Beep_OFF();   //������ʱ�ص�
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
 static unsigned int num,i;

 TH0=(65536-1000)/256;//���¸�ֵ 1ms
 TL0=(65536-1000)%256;
 if(BeepFlag)         //�������ȱ�־
   {
   if(num<300 || (num>500&&num<800) )
     BEEP=!BEEP;     //����
   else
     BEEP=0;         //ֹͣ����
   }
 Display(0,8);       // ���������ɨ��
 i++;
 if(i==20)          //20ms����һ��
    {
	i=0;
	UpdateTimeFlag=1; //����ʱ��־λ��1
	}
 num++;
 if(num==1000)        //����1s
   {
    num=0;
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
             ���Ⱦ���������
------------------------------------------------*/
void  Beep_ON(void)
{
 BeepFlag=1;
}
/*------------------------------------------------
             ���Ⱦ���������
------------------------------------------------*/
void  Beep_OFF(void)
{
 BeepFlag=0;
 BEEP=0;
}
