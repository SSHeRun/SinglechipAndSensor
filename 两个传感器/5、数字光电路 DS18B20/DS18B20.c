/********************************************************************
*��ʾ��ʵ�������¶ȴ�������DS18B20���¶ȵĲɼ����������������ʾ������
***********************************************************************/
#include<reg52.h>
#include  <math.h>    //Keil library  
#include  <stdio.h>   //Keil library
#include  <INTRINS.H>	
#define uchar unsigned char
#define uint  unsigned int

#define jump_ROM 0xCC
#define start 0x44
#define read_EEROM 0xBE

sbit DQ = P3^5;            //DS18B20���ݿ�

unsigned char TMPH,TMPL; 

uchar code table[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
sbit E=P2^3;
sbit RW=P2^4;
sbit RS=P2^5;

#define LCM_Data  P0

#define   uchar unsigned char
#define   uint unsigned int	

sbit	  SCL=P1^0;      //IICʱ�����Ŷ���
sbit  	  SDA=P1^1;      //IIC�������Ŷ���

#define	  SlaveAddress   0x46 //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
                              //ALT  ADDRESS���Žӵ�ʱ��ַΪ0x46���ӵ�Դʱ��ַΪ0xB8
typedef   unsigned char BYTE;
typedef   unsigned short WORD;

BYTE    BUF[8];                         //�������ݻ�����      	
uchar   ge,shi,bai,qian,wan;            //��ʾ����
int     dis_data;                       //����

/********************************************************************
* ���� : delay()
* ���� : ��ʱ,��ʱʱ����Ϊ140US��
* ���� : ��
* ��� : ��
***********************************************************************/

void delay_1()
{
	int i,j;
	for(i=0; i<=10; i++)
	for(j=0; j<=2; j++)
;
}

void Delay1(unsigned int t)   // delay 40us
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
   Delay1(1);
   E=0;
   Delay1(100);  //delay 40us
}

//д����
void WriteDataLCM(unsigned char ch)
{  RS=1;
   RW=0;
   E=1;
   LCM_Data=ch;
   Delay1(1);
   E=0;
   Delay1(100); //delay 40us
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

//********���ֽڶ�ȡ*****************************************
/*
uchar Single_Read_BH1750(uchar REG_Address)
{  uchar REG_data;
    BH1750_Start();                          //��ʼ�ź�
    BH1750_SendByte(SlaveAddress);           //�����豸��ַ+д�ź�
    BH1750_SendByte(REG_Address);                   //���ʹ洢��Ԫ��ַ����0��ʼ	
    BH1750_Start();                          //��ʼ�ź�
    BH1750_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
    REG_data=BH1750_RecvByte();              //�����Ĵ�������
	BH1750_SendACK(1);   
	BH1750_Stop();                           //ֹͣ�ź�
    return REG_data; 
}
*/
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



//LCD��ʼ��
void InitLcd()
{WriteCommandLCM(0x30);		 //�������ã�����8λ���ݽӿڣ�
 WriteCommandLCM(0x30);
 WriteCommandLCM(0x30);
 WriteCommandLCM(0x38);	//���ù�����ʽ,8λ���ݽӿڣ�������ʾ��5*7����
 WriteCommandLCM(0x08);	//����ʾ���ر���ʾ���رչ�꣬�ر���˸
 WriteCommandLCM(0x0c); //��ʾ״̬���ã�����ʾ���رչ������˸
 WriteCommandLCM(0x01); //����
 WriteCommandLCM(0x06); //���뷽ʽ���� ,AC�Զ���һ�����治��
}
/********************************************************************
* ���� : delay()
* ���� : ��ʱ����
* ���� : ��
* ��� : ��
***********************************************************************/
void delay(uint N)
{
	int i;
	for(i=0; i<N; i++)
	;
}

/********************************************************************
* ���� : Delay_1ms()
* ���� : ��ʱ�ӳ�����ʱʱ��Ϊ 1ms * x
* ���� : x (��ʱһ����ĸ���)
* ��� : ��
***********************************************************************/
void Delay_1ms(uint i)//1ms��ʱ
{
	uchar x,j;
	for(j=0;j<i;j++)
	for(x=0;x<=148;x++);	
}
/********************************************************************
* ���� : Reset()
* ���� : ��λDS18B20
* ���� : ��
* ��� : ��
***********************************************************************/
uchar Reset(void)
{
	uchar deceive_ready;
	DQ = 0;
	delay(29);
	DQ = 1;
	delay(3);
	deceive_ready = DQ;
	delay(25);
	return(deceive_ready);
}

/********************************************************************
* ���� : read_bit()
* ���� : ��DS18B20��һ��λֵ
* ���� : ��
* ��� : ��DS18B20������һ��λֵ
***********************************************************************/
uchar read_bit(void)
{
	uchar i;
	DQ = 0;
	DQ = 1;
	for(i=0; i<3; i++);
	return(DQ);
}

/********************************************************************
* ���� : write_bit()
* ���� : ��DS18B20дһλ
* ���� : bitval��Ҫ��DS18B20д���λֵ��
* ��� : ��
***********************************************************************/
void write_bit(uchar bitval)
{
DQ=0;if(bitval==1)
DQ=1;
delay(5);
DQ=1;
}

/********************************************************************
* ���� : read_byte()
* ���� : ��DS18B20��һ���ֽ�
* ���� : ��
* ��� : ��DS18B20������ֵ
***********************************************************************/
uchar read_byte(void)
{
	uchar i,m,receive_data;
	m = 1;
	receive_data = 0;
	for(i=0; i<8; i++)
	{
		if(read_bit())
		{
			receive_data = receive_data + (m << i);
		}
		delay(6);
	}
	return(receive_data);
}

/********************************************************************
* ���� : write_byte()
* ���� : ��DS18B20дһ���ֽ�
* ���� : val��Ҫ��DS18B20д�������ֵ��
* ��� : ��
***********************************************************************/
void write_byte(uchar val)
{
	uchar i,temp;
	for(i=0; i<8; i++)
	{
		temp = val >> i;
		temp = temp & 0x01;
		write_bit(temp);
		delay(5);
	}
}


/********************************************************************
* ���� : Main()
* ���� : ������
* ���� : ��
* ��� : ��
***********************************************************************/
void main()
{
	float temp1;
	uint temp;
	P2 = 0x00;

	
   delay_nms(100);	    //��ʱ100ms	
   InitLcd();           //��ʼ��LCD
   Init_BH1750();       //��ʼ��BH1750	
	while(1)
	{

		  Single_Write_BH1750(0x01);   // power on
    Single_Write_BH1750(0x10);   // H- resolution mode

     delay_nms(180);              //��ʱ180ms

     Multiple_read_BH1750();       //�����������ݣ��洢��BUF��

    dis_data=BUF[0];
    dis_data=(dis_data<<8)+BUF[1];//�ϳ����ݣ�����������
    
    temp1=(float)dis_data/1.2;

    conversion(temp1);         //�������ݺ���ʾ
	DisplayOneChar(0,0,'L'); 
	DisplayOneChar(1,0,'i'); 
	DisplayOneChar(2,0,'g'); 
	DisplayOneChar(3,0,'h'); 
	DisplayOneChar(4,0,'t'); 
    DisplayOneChar(5,0,':'); 

    DisplayOneChar(7,0,wan); //��ʾ����
    DisplayOneChar(8,0,qian);  
    DisplayOneChar(9,0,bai); 
    DisplayOneChar(10,0,shi); 
	DisplayOneChar(11,0,ge); 

	DisplayOneChar(13,0,'L'); ////��ʾ����λ
	DisplayOneChar(14,0,'X');  

	if(bai >= '1'){
		while(1){
		 		Reset();
		write_byte(jump_ROM);
		write_byte(start);
		Reset();
		write_byte(jump_ROM);
		write_byte(read_EEROM);
		TMPL = read_byte();
		TMPH = read_byte();
		temp = TMPL / 16 + TMPH * 16;
		P0 = table[temp/10%10];
		P2 = 6;
		Delay_1ms(5);
		P0 = table[temp%10];
		P2 = 7;
		Delay_1ms(5);
		}
	}

	}
}