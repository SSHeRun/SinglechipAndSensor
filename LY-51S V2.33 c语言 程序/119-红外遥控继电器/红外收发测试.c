
/*-----------------------------------------------
  ���ƣ�ң�������������Ƽ̵���
  ��̳��www.doflye.net
  ��д��shifang
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ�������ң����������P1.4�Ӽ̵������ƶ�J42��RL1��
        �������J27��P3.2 ��ң����CH�������̵���״̬��ת
        ��ͬң�������޸�Ԥ�����ֵReversionCode �����Ĳ������������о���
------------------------------------------------*/
#include<reg52.h>
////////////////////////////////////////////////
#define TURE 1
#define FALSE 0

#define ShutCode 		0x45
#define ReversionCode   0x46
#define OpenCode        0x47

////////////////////////////////////////////////

sbit IR=P3^2;//����ӿڱ�־
sbit SSR=P1^4;//�̵�������˿�
sbit Indicator_light=P1^7;//ָʾ������˿�
sbit KeyCon=P3^7;
sbit LED=P1^5;//����ָʾ��



unsigned char  irtime;//������ȫ�ֱ���
unsigned char  i;
bit flag;
bit irpro_ok,irok;
unsigned char IRcord[4];
unsigned char   irdata[33];

//////////////////////////////////////////////

void Light(void);//��������
void Flash(void);
void UnLight(void);
void Keyscan(void);
void Delay(unsigned char mS);
void Ir_work(void);
void Ircordpro(void);



/*------------------------------------------------
                 ���ϼ̵���
------------------------------------------------*/
void Light(void)//�̵������ϣ�ָʾ����
{
 SSR=1;
 Indicator_light=0;
 }

/*------------------------------------------------
                ȡ���̵���״̬
------------------------------------------------*/
void Flash(void)//�̵������ָ࣬ʾ�Ʒ���
{
  SSR=!SSR;
  Indicator_light=!Indicator_light;
 }

/*------------------------------------------------
                �ͷż̵���
------------------------------------------------*/
void UnLight(void)//�̵����ͷţ�ָʾ����
{
 SSR=0;
 Indicator_light=1;
 }
/*------------------------------------------------
         ����ɨ���⣬����������ܲ���
------------------------------------------------*/
void Keyscan(void)//����ɨ��
{
/* if(!KeyLeft)//
     {

     Delay(10);//ȥ��
     if(!KeyLeft)
       {
        Light();
       }
      }
  else if(!KeyRight)//
     {
      Delay(10);//ȥ��
      if(!KeyRight)
        {
         Flash();
        } 
     }*/
 if(!KeyCon)//���������򿪹ص���
	{
	 Delay(10);//ȥ��

     if(!KeyCon)
       {
	    while(!KeyCon)
            {};

        Delay(10);//ȥ��

        while(!KeyCon)
            {};

            {
             //Indicator_light=!Indicator_light;
             Flash();
            }
       }
    }
 }
////////////////////////////////////////////////////////////////
void Delay(unsigned char mS)//delay mS
{
 unsigned char us,usn;

 while(mS!=0)
  { 
   usn = 4;
   while(usn!=0)
    {
     us=0xf0;
     while (us!=0) {us--;};
     usn--;
    }

   mS--;

  }
}


/*------------------------------------------------
                ��ʱ��0�жϳ���
------------------------------------------------*/  

void tim0_isr (void) interrupt 1 using 1//��ʱ��0�жϷ�����
{
  irtime++;
}
/*------------------------------------------------
                �ⲿ��ʱ��0�������
------------------------------------------------*/
void ex0_isr (void) interrupt 0 using 0//�ⲿ�ж�0������
{
  static unsigned char  i;             //���պ����źŴ���
  static bit startflag;                //�Ƿ�ʼ�����־λ

if(startflag)                         
   {
    if(irtime<63&&irtime>=33)//������ TC9012��ͷ�룬9ms+4.5ms
                        i=0;
    		irdata[i]=irtime;//�洢ÿ����ƽ�ĳ���ʱ�䣬�����Ժ��ж���0����1
    		irtime=0;
    		i++;
   			 if(i==33)
      			{
	  			 irok=1;
				 i=0;
	  			}
          }
  	 else
		{
		irtime=0;
		startflag=1;
		}
}
/*------------------------------------------------
                ��ʱ��0��ʼ��
------------------------------------------------*/
void TIM0init(void)//��ʱ��0��ʼ��
{

  TMOD=0x02;//��ʱ��0������ʽ2��TH0����װֵ��TL0�ǳ�ֵ
  TH0=0x00;//reload value
  TL0=0x00;//initial value
  ET0=1;//���ж�
  TR0=1;
}
/*------------------------------------------------
                �ⲿ��ʱ��ʼ��
------------------------------------------------*/
void EX0init(void)
{
 IT0 = 1;   // Configure interrupt 0 for falling edge on /INT0 (P3.2)
 EX0 = 1;   // Enable EX0 Interrupt
 EA = 1;  
}
/*------------------------------------------------
               ������
------------------------------------------------*/
void main(void)
{
 SSR=0;//��ʼ�������
 Indicator_light=0;
 
 EX0init(); // ��ʼ��
 TIM0init();

 while(1)//��ѭ��
   {
 
    Keyscan();//����ɨ��

    if(irok)
	  {   
	   Ircordpro();
 	   irok=0;
	  }

    if(irpro_ok)//����ң���ź�
	  {
	   Ir_work();
  	  }


   }
   
}
/*------------------------------------------------
                ���⴦����ֵ���޸�
------------------------------------------------*/

  void Ir_work(void)
  {
    switch(IRcord[2])//���ж�����1�����ݣ������ֹ���룬��Ҫͬʱ�Ա�4���ֽ�����
	      {
           //case ShutCode: UnLight(); break;//�˴����ݲ�ͬ��ң����������Ӧ��ֵ
	       case ReversionCode: Flash(); break;
	       //case OpenCode: Light(); break;
	       default:break;
	      }
		  irpro_ok=0;
	      IRcord[1]=0;//���ֵ����ֹ����
	      IRcord[2]=0;
  }
/*------------------------------------------------
                ���ּ�ֵ
------------------------------------------------*/
void Ircordpro(void)//������ֵ������
{ 
  unsigned char i, j, k;
  unsigned char cord,value;

  k=1;
  for(i=0;i<4;i++)//����4���ֽ�
     {
      for(j=1;j<=8;j++) //����1���ֽ�8λ
         {
          cord=irdata[k];
          if(cord>0x07)//����ĳֵΪ1
		    {
             value=value|0x80;
			}
          else 
		    {
             value=value;
			}
          if(j<8)
		    {
			 value=value>>1;
			}
           k++;
         }
     IRcord[i]=value;
     value=0;     
     } irpro_ok=1;//������ϱ�־λ��1
}

