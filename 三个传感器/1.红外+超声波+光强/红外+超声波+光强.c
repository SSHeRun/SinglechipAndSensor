#include <AT89X51.H>
#include <math.h>    //Keil library  
#include <stdio.h>   //Keil library	
#include <INTRINS.H>

#define uchar unsigned char
#define uint  unsigned int

//����
sbit IRIN = P3^2;         //���������������

uchar IRCOM[7];

sbit E=P2^3;
sbit RW=P2^4;
sbit RS=P2^5;

#define LCM_Data  P0

uchar code  cdis1[ ] = {"  Red Control  "};
uchar code  cdis2[ ] = {"  IR-CODE:--H  "};

//���
#define  RX  P1_1
#define  TX  P1_2
  
unsigned int  time=0;
unsigned int  timer=0;
float         S=0;
bit           flag =0;

//��ǿ
sbit	  SCL=P1^3;      //IICʱ�����Ŷ���
sbit  	  SDA=P1^4;      //IIC�������Ŷ���

#define	  SlaveAddress   0x46 //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
                              //ALT  ADDRESS���Žӵ�ʱ��ַΪ0x46���ӵ�Դʱ��ַΪ0xB8
typedef   unsigned char BYTE;
typedef   unsigned short WORD;

BYTE    BUF[8];                         //�������ݻ�����      	
uchar   ge,shi,bai,qian,wan;            //��ʾ����
int     dis_data;                       //����

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

//���
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

//��ǿ
//*********************************************************
void conversion(uint temp_data)  //  ����ת���� ����ʮ���٣�ǧ����
{  
    wan=temp_data/10000+0x30 ;
    temp_data=temp_data%10000;   //ȡ������
	qian=temp_data/1000+0x30 ;
    temp_data=temp_data%1000;    //ȡ������
    bai=temp_data/100+0x30   ;
    temp_data=temp_data%100;     //ȡ������
    shi=temp_data/10+0x30    ;
    temp_data=temp_data%10;      //ȡ������
    ge=temp_data+0x30; 	
}

//������ʱ**************************
void delay_nms(unsigned int k)	
{						
unsigned int i,j;				
for(i=0;i<k;i++)
{			
for(j=0;j<121;j++)			
{;}}						
}

/**************************************
��ʱ5΢��(STC90C52RC@12M)
��ͬ�Ĺ�������,��Ҫ�����˺�����ע��ʱ�ӹ���ʱ��Ҫ�޸�
������1T��MCUʱ,���������ʱ����
**************************************/
void Delay5us()
{
    _nop_();_nop_();_nop_();_nop_();
    _nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
}

/**************************************
��ʱ5����(STC90C52RC@12M)
��ͬ�Ĺ�������,��Ҫ�����˺���
������1T��MCUʱ,���������ʱ����
**************************************/
void Delay5ms()
{
    WORD n = 560;

    while (n--);
}

/**************************************
��ʼ�ź�
**************************************/
void BH1750_Start()
{
    SDA = 1;                    //����������
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    SDA = 0;                    //�����½���
    Delay5us();                 //��ʱ
    SCL = 0;                    //����ʱ����
}

/**************************************
ֹͣ�ź�
**************************************/
void BH1750_Stop()
{
    SDA = 0;                    //����������
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    SDA = 1;                    //����������
    Delay5us();                 //��ʱ
}

/**************************************
����Ӧ���ź�
��ڲ���:ack (0:ACK 1:NAK)
**************************************/
void BH1750_SendACK(bit ack)
{
    SDA = ack;                  //дӦ���ź�
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    SCL = 0;                    //����ʱ����
    Delay5us();                 //��ʱ
}

/**************************************
����Ӧ���ź�
**************************************/
bit BH1750_RecvACK()
{
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    CY = SDA;                   //��Ӧ���ź�
    SCL = 0;                    //����ʱ����
    Delay5us();                 //��ʱ

    return CY;
}

/**************************************
��IIC���߷���һ���ֽ�����
**************************************/
void BH1750_SendByte(BYTE dat)
{
    BYTE i;

    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;              //�Ƴ����ݵ����λ
        SDA = CY;               //�����ݿ�
        SCL = 1;                //����ʱ����
        Delay5us();             //��ʱ
        SCL = 0;                //����ʱ����
        Delay5us();             //��ʱ
    }
    BH1750_RecvACK();
}

/**************************************
��IIC���߽���һ���ֽ�����
**************************************/
BYTE BH1750_RecvByte()
{
    BYTE i;
    BYTE dat = 0;

    SDA = 1;                    //ʹ���ڲ�����,׼����ȡ����,
    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;
        SCL = 1;                //����ʱ����
        Delay5us();             //��ʱ
        dat |= SDA;             //������               
        SCL = 0;                //����ʱ����
        Delay5us();             //��ʱ
    }
    return dat;
}

//*********************************

void Single_Write_BH1750(uchar REG_Address)
{
    BH1750_Start();                  //��ʼ�ź�
    BH1750_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
    BH1750_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��
  //  BH1750_SendByte(REG_data);       //�ڲ��Ĵ������ݣ�
    BH1750_Stop();                   //����ֹͣ�ź�
}

//*********************************************************
//
//��������BH1750�ڲ�����
//
//*********************************************************
void Multiple_read_BH1750(void)
{   uchar i;	
    BH1750_Start();                          //��ʼ�ź�
    BH1750_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
	
	 for (i=0; i<3; i++)                      //������ȡ2����ַ���ݣ��洢��BUF
    {
        BUF[i] = BH1750_RecvByte();          //BUF[0]�洢0x32��ַ�е�����
        if (i == 3)
        {

           BH1750_SendACK(1);                //���һ��������Ҫ��NOACK
        }
        else
        {		
          BH1750_SendACK(0);                //��ӦACK
       }
   }

    BH1750_Stop();                          //ֹͣ�ź�
    Delay5ms();
}


//��ʼ��BH1750��������Ҫ��ο�pdf�����޸�****
void Init_BH1750()
{
   Single_Write_BH1750(0x01);  

}

main()
{
	float temp;
	//����
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
   

	IE = 0x81;                 //�������ж��ж�,ʹ�� INT0 �ⲿ�ж�
	TCON = 0x01;               //������ʽΪ���帺���ش���

	
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

   //��ǿ
   Init_BH1750();       //��ʼ��BH1750
	
	while(1)
	{
	//���
	 StartModule();
	 while(!RX);		//��RXΪ��ʱ�ȴ�
	 TR0=1;			    //��������
	 while(RX);			//��RXΪ1�������ȴ�
	 TR0=0;				//�رռ���
     Conut();			//����
	 delay_nms(100);		//100MS
	 
	  //��ǿ
	 Single_Write_BH1750(0x01);   // power on
    Single_Write_BH1750(0x10);   // H- resolution mode

     delay_nms(180);              //��ʱ180ms

     Multiple_read_BH1750();       //�����������ݣ��洢��BUF��

    dis_data=BUF[0];
    dis_data=(dis_data<<8)+BUF[1];//�ϳ����ݣ�����������
    
    temp=(float)dis_data/1.2;
	conversion(temp);         //�������ݺ���ʾ	
	printf("Light: %c%c%c%c%c LX\n",wan,qian,bai,shi,ge);
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
