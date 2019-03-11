/********************************************************************************
*          lcd1602��ʾ ң�ؼ�ֵ��ȡ��                                          *
*         lcd1602��ʾ ң������p3.2                                             *
********************************************************************************/

#include <reg51.h>
#include <intrins.h>
#include   <STDIO.H>

#define uchar unsigned char
#define uint  unsigned int
sbit RX=P1^1;
sbit  TX=P1^2;

sbit IRIN = P3^2;         //���������������

uchar IRCOM[7];

sbit E=P2^3;
sbit RW=P2^4;
sbit RS=P2^5;

#define LCM_Data  P0

uchar code  cdis1[ ] = {"  Red Control  "};
uchar code  cdis2[ ] = {"  IR-CODE:--H  "};

unsigned int  time=0;
unsigned int  timer=0;
float         S=0;
bit           flag =0;

//��ʱ����
void Delay(unsigned int t)   // delay 40us
{
 for(;t!=0;t--) ;
}


//дָ��
void WriteCommandLCM(unsigned char ch)
{
   RS=0;
   RW=0;
   E=1;
   LCM_Data=ch;
   Delay(1);
   E=0;
   Delay(100);  //delay 40us
}

//д����
void WriteDataLCM(unsigned char ch)
{  RS=1;
   RW=0;
   E=1;
   LCM_Data=ch;
   Delay(1);
   E=0;
   Delay(100); //delay 40us
}

void DisplayOneChar(uchar X,uchar Y,uchar DData)
{						
	Y&=1;						
	X&=15;						
	if(Y)X|=0x40;					
	X|=0x80;			
	WriteCommandLCM(X);		
	WriteDataLCM(DData);		
}				

/*******************************************************************/
/*  LCD��ʼ���趨                                                  */
/*******************************************************************/
void InitLcd()
{
 WriteCommandLCM(0x30);		 //�������ã�����8λ���ݽӿڣ�
 WriteCommandLCM(0x30);
 WriteCommandLCM(0x30);
 WriteCommandLCM(0x38);	//���ù�����ʽ,8λ���ݽӿڣ�������ʾ��5*7����
 WriteCommandLCM(0x08);	//����ʾ���ر���ʾ���رչ�꣬�ر���˸
 WriteCommandLCM(0x0c); //��ʾ״̬���ã�����ʾ���رչ������˸
 WriteCommandLCM(0x01); //����
 WriteCommandLCM(0x06); //���뷽ʽ���� ,AC�Զ���һ�����治��
}

/********************************************************/
void Conut(void)
{
 time=TH0*256+TL0;
 TH0=0;
 TL0=0;
 S=(time*1.87)/100;     //�������CM
 if(flag==1)		    //��������
 {
  flag=0;
  printf("-----\n"); 
 }

  printf("S=%f\n",S); 
}
/********************************************************/
void zd0() interrupt 1 		 //T0�ж��������������,������෶Χ
{
  flag=1;							 //�ж������־
}
/********************************************************/
void  StartModule() 		         //T1�ж�����ɨ������ܺͼ�800MS����ģ��
{
	TX=1;			                 //800MS  ����һ��ģ��
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_();
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_();
	TX=0;
}
/********************************************************/

main()
{
   uchar m;
    
    IRIN=1;                    //I/O�ڳ�ʼ��

    Delay(10);                 //��ʱ
    InitLcd();                //��ʼ��LCD             

     m = 0;
    while(cdis1[m] != '\0')
     {                         //��ʾ�ַ�
       WriteDataLCM(cdis1[m]);
       m++;
     }

    //lcd_pos(0x40);             //������ʾλ��Ϊ�ڶ��е�1���ַ�
	WriteCommandLCM(0x40 | 0x80);
     m = 0;
    while(cdis2[m] != '\0')
     {
       WriteDataLCM(cdis2[m]);      //��ʾ�ַ�
       m++;
     }
   
	TMOD=0x21;		   //��T0Ϊ��ʽ1��GATE=1��
	SCON=0x50;
	TH1=0xFD;
	TL1=0xFD;
	TH0=0;
	TL0=0; 
	TR0=1;  
	ET0=1;             //����T0�ж�
	TR1=1;			   //������ʱ��
	TI=1;

	EA=1;			   //�������ж�
	EX0=1;
	//IE = 0x81;                 //�������ж��ж�,ʹ�� INT0 �ⲿ�ж�
	//TCON = 0x01;               //������ʽΪ���帺���ش���

   while(1)
	{
	 StartModule();
	 while(!RX);		//��RXΪ��ʱ�ȴ�
	 TR0=1;			    //��������
	 while(RX);			//��RXΪ1�������ȴ�
	 TR0=0;				//�رռ���
     Conut();			//����
	 Delay(100000);		
	}

} //end main

/**********************************************************/
void IR_IN() interrupt 0 using 0
{
  unsigned char j,k,N=0;
     EX0 = 0;   
	 Delay(200);
	 if (IRIN==1) 
     { EX0 =1;
	   return;
	  } 
                           //ȷ��IR�źų���
  while (!IRIN)            //��IR��Ϊ�ߵ�ƽ������9ms��ǰ���͵�ƽ�źš�
    {Delay(13);}

 for (j=0;j<4;j++)         //�ռ���������
 { 
  for (k=0;k<8;k++)        //ÿ��������8λ
  {
   while (IRIN)            //�� IR ��Ϊ�͵�ƽ������4.5ms��ǰ���ߵ�ƽ�źš�
     {Delay(13);}
    while (!IRIN)          //�� IR ��Ϊ�ߵ�ƽ
     {Delay(13);}
     while (IRIN)           //����IR�ߵ�ƽʱ��
      {
   Delay(13);
    N++;           
    if (N>=30)
	 { EX0=1;
	 return;}                  //0.14ms���������Զ��뿪��
      }                        //�ߵ�ƽ�������                
     IRCOM[j]=IRCOM[j] >> 1;                  //�������λ����0��
     if (N>=8) {IRCOM[j] = IRCOM[j] | 0x80;}  //�������λ����1��
     N=0;
  }//end for k
 }//end for j
   
   if (IRCOM[2]!=~IRCOM[3])
   { EX0=1;
     return; }

   IRCOM[5]=IRCOM[2] & 0x0F;     //ȡ����ĵ���λ
   IRCOM[6]=IRCOM[2] >> 4;       //����4�Σ�����λ��Ϊ����λ

   if(IRCOM[5]>9)
    { IRCOM[5]=IRCOM[5]+0x37;}
   else
	  IRCOM[5]=IRCOM[5]+0x30;

   if(IRCOM[6]>9)
    { IRCOM[6]=IRCOM[6]+0x37;}
   else
	  IRCOM[6]=IRCOM[6]+0x30;

           
     DisplayOneChar(10,1,IRCOM[6]);          //��һλ����ʾ         
     DisplayOneChar(11,1,IRCOM[5]);        //�ڶ�λ����ʾ

     EX0 = 1; 
} 



