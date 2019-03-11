#include <reg51.h>
#include  <math.h>    //Keil library  
#include  <stdio.h>   //Keil library	
#include  <INTRINS.H>
#include   <STDIO.H>

sbit	  S_SCL=P1^3;      //���ֹ�ǿIICʱ�����Ŷ���
sbit  	  S_SDA=P1^4;      //���ֹ�ǿIIC�������Ŷ���

#define   uchar unsigned char
#define   uint unsigned int	
#define	  S_SlaveAddress   0x46 //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
                              //ALT  ADDRESS���Žӵ�ʱ��ַΪ0x46���ӵ�Դʱ��ַΪ0xB8

char    S_BUF[8];                         //�������ݻ�����   

sbit  RX=P1^1;
sbit  TX=P1^2;
unsigned int  time=0;
unsigned int  timer=0;
float         S=0;
bit           flag =0;

#define   DataPort P0    //LCD1602���ݶ˿�

sbit	  Z_SCL=P2^1;      //������ٶ�IICʱ�����Ŷ���
sbit 	  Z_SDA=P2^0;      //������ٶ�IIC�������Ŷ���

sbit      LCM_RS=P2^5;   //LCD1602����˿�		
sbit      LCM_RW=P2^4;   //LCD1602����˿�		
sbit      LCM_EN=P2^3;   //LCD1602����˿�
#define	Z_SlaveAddress   0xA6	  //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
                              //ALT  ADDRESS���Žӵ�ʱ��ַΪ0xA6���ӵ�Դʱ��ַΪ0x3A
typedef unsigned char  BYTE;
typedef unsigned short WORD;

BYTE Z_BUF[8];                         //�������ݻ�����      	
uchar ge,shi,bai,qian,wan;           //��ʾ����
int  dis_data;                       //����

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

/*******************************/
void delay(unsigned int k)	
{						
unsigned int i,j;				
	for(i=0;i<k;i++)
	{			
	for(j=0;j<121;j++)			
	{;}}						
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

/*******************************/
void WriteCommandLCM(uchar CMD)
{					
	LCM_RS=0;
	LCM_RW=0;
	DataPort=CMD;
	LCM_EN=1;
	LCM_EN=0;
	delay(1);
}					
/*******************************/
void WriteDataLCM(uchar dataW)
{					
	LCM_RS=1;
	LCM_RW=0;
	DataPort=dataW;
	LCM_EN=1;
	LCM_EN=0;
	delay(1);
}		
/***********************************/
void InitLcd()				
{			
	WriteCommandLCM(0x30);	
	WriteCommandLCM(0x30);	
	WriteCommandLCM(0x38);	
	WriteCommandLCM(0x08);	
	WriteCommandLCM(0x0c);
	WriteCommandLCM(0x01);	
	WriteCommandLCM(0x06);	

}			
/***********************************/
void DisplayOneChar(uchar X,uchar Y,uchar DData)
{						
	Y&=1;						
	X&=15;						
	if(Y)X|=0x40;					
	X|=0x80;			
	WriteCommandLCM(X);		
	WriteDataLCM(DData);		
}	


/**************************************
��ʼ�ź�
**************************************/
void BH1750_Start()
{
    S_SDA = 1;                    //����������
    S_SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    S_SDA = 0;                    //�����½���
    Delay5us();                 //��ʱ
    S_SCL = 0;                    //����ʱ����
}

/**************************************
ֹͣ�ź�
**************************************/
void BH1750_Stop()
{
    S_SDA = 0;                    //����������
    S_SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    S_SDA = 1;                    //����������
    Delay5us();                 //��ʱ
}

/**************************************
����Ӧ���ź�
��ڲ���:ack (0:ACK 1:NAK)
**************************************/
void BH1750_SendACK(bit ack)
{
    S_SDA = ack;                  //дӦ���ź�
    S_SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    S_SCL = 0;                    //����ʱ����
    Delay5us();                 //��ʱ
}

/**************************************
����Ӧ���ź�
**************************************/
bit BH1750_RecvACK()
{
    S_SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    CY = S_SDA;                   //��Ӧ���ź�
    S_SCL = 0;                    //����ʱ����
    Delay5us();                 //��ʱ

    return CY;
}

/**************************************
��IIC���߷���һ���ֽ�����
**************************************/
void BH1750_SendByte(BYTE dat)
{
    char i;

    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;              //�Ƴ����ݵ����λ
        S_SDA = CY;               //�����ݿ�
        S_SCL = 1;                //����ʱ����
        Delay5us();             //��ʱ
        S_SCL = 0;                //����ʱ����
        Delay5us();             //��ʱ
    }
    BH1750_RecvACK();
}

/**************************************
��IIC���߽���һ���ֽ�����
**************************************/
BYTE BH1750_RecvByte()
{
    char i;
    char dat = 0;

    S_SDA = 1;                    //ʹ���ڲ�����,׼����ȡ����,
    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;
        S_SCL = 1;                //����ʱ����
        Delay5us();             //��ʱ
        dat |= S_SDA;             //������               
        S_SCL = 0;                //����ʱ����
        Delay5us();             //��ʱ
    }
    return dat;
}

//*********************************

void Single_Write_BH1750(uchar REG_Address)
{
    BH1750_Start();                  //��ʼ�ź�
    BH1750_SendByte(S_SlaveAddress);   //�����豸��ַ+д�ź�
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
    BH1750_SendByte(S_SlaveAddress+1);         //�����豸��ַ+���ź�
	
	 for (i=0; i<3; i++)                      //������ȡ2����ַ���ݣ��洢��BUF
    {
        S_BUF[i] = BH1750_RecvByte();          //S_BUF[0]�洢0x32��ַ�е�����
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


/**************************************
��ʼ�ź�
**************************************/
void ADXL345_Start()
{
    Z_SDA = 1;                    //����������
    Z_SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    Z_SDA = 0;                    //�����½���
    Delay5us();                 //��ʱ
    Z_SCL = 0;                    //����ʱ����
}

/**************************************
ֹͣ�ź�
**************************************/
void ADXL345_Stop()
{
    Z_SDA = 0;                    //����������
    Z_SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    Z_SDA = 1;                    //����������
    Delay5us();                 //��ʱ
}

/**************************************
����Ӧ���ź�
��ڲ���:ack (0:ACK 1:NAK)
**************************************/
void ADXL345_SendACK(bit ack)
{
    Z_SDA = ack;                  //дӦ���ź�
    Z_SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    Z_SCL = 0;                    //����ʱ����
    Delay5us();                 //��ʱ
}

/**************************************
����Ӧ���ź�
**************************************/
bit ADXL345_RecvACK()
{
    Z_SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    CY = Z_SDA;                   //��Ӧ���ź�
    Z_SCL = 0;                    //����ʱ����
    Delay5us();                 //��ʱ

    return CY;
}

/**************************************
��IIC���߷���һ���ֽ�����
**************************************/
void ADXL345_SendByte(BYTE dat)
{
    char i;

    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;              //�Ƴ����ݵ����λ
        Z_SDA = CY;               //�����ݿ�
        Z_SCL = 1;                //����ʱ����
        Delay5us();             //��ʱ
        Z_SCL = 0;                //����ʱ����
        Delay5us();             //��ʱ
    }
    ADXL345_RecvACK();
}

/**************************************
��IIC���߽���һ���ֽ�����
**************************************/
BYTE ADXL345_RecvByte()
{
    char i;
    char dat = 0;

    Z_SDA = 1;                    //ʹ���ڲ�����,׼����ȡ����,
    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;
        Z_SCL = 1;                //����ʱ����
        Delay5us();             //��ʱ
        dat |= Z_SDA;             //������               
        Z_SCL = 0;                //����ʱ����
        Delay5us();             //��ʱ
    }
    return dat;
}

//******���ֽ�д��*******************************************

void Single_Write_ADXL345(uchar REG_Address,uchar REG_data)
{
    ADXL345_Start();                  //��ʼ�ź�
    ADXL345_SendByte(Z_SlaveAddress);   //�����豸��ַ+д�ź�
    ADXL345_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ����ο�����pdf22ҳ 
    ADXL345_SendByte(REG_data);       //�ڲ��Ĵ������ݣ���ο�����pdf22ҳ 
    ADXL345_Stop();                   //����ֹͣ�ź�
}

//********���ֽڶ�ȡ*****************************************
uchar Single_Read_ADXL345(uchar REG_Address)
{  uchar REG_data;
    ADXL345_Start();                          //��ʼ�ź�
    ADXL345_SendByte(Z_SlaveAddress);           //�����豸��ַ+д�ź�
    ADXL345_SendByte(REG_Address);            //���ʹ洢��Ԫ��ַ����0��ʼ	
    ADXL345_Start();                          //��ʼ�ź�
    ADXL345_SendByte(Z_SlaveAddress+1);         //�����豸��ַ+���ź�
    REG_data=ADXL345_RecvByte();              //�����Ĵ�������
	ADXL345_SendACK(1);   
	ADXL345_Stop();                           //ֹͣ�ź�
    return REG_data; 
}
//*********************************************************
//
//��������ADXL345�ڲ����ٶ����ݣ���ַ��Χ0x32~0x37
//
//*********************************************************
void Multiple_read_ADXL345(void)
{   uchar i;
    ADXL345_Start();                          //��ʼ�ź�
    ADXL345_SendByte(Z_SlaveAddress);           //�����豸��ַ+д�ź�
    ADXL345_SendByte(0x32);                   //���ʹ洢��Ԫ��ַ����0x32��ʼ	
    ADXL345_Start();                          //��ʼ�ź�
    ADXL345_SendByte(Z_SlaveAddress+1);         //�����豸��ַ+���ź�
	 for (i=0; i<6; i++)                      //������ȡ6����ַ���ݣ��洢��BUF
    {
        Z_BUF[i] = ADXL345_RecvByte();          //BUF[0]�洢0x32��ַ�е�����
        if (i == 5)
        {
           ADXL345_SendACK(1);                //���һ��������Ҫ��NOACK
        }
        else
        {
          ADXL345_SendACK(0);                //��ӦACK
       }
   }
    ADXL345_Stop();                          //ֹͣ�ź�
    Delay5ms();
}


//*****************************************************************

//��ʼ��ADXL345��������Ҫ��ο�pdf�����޸�************************
void Init_ADXL345()
{
   Single_Write_ADXL345(0x31,0x0B);   //������Χ,����16g��13λģʽ
   Single_Write_ADXL345(0x2C,0x08);   //�����趨Ϊ12.5 �ο�pdf13ҳ
   Single_Write_ADXL345(0x2D,0x08);   //ѡ���Դģʽ   �ο�pdf24ҳ
   Single_Write_ADXL345(0x2E,0x80);   //ʹ�� DATA_READY �ж�
   Single_Write_ADXL345(0x1E,0x00);   //X ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
   Single_Write_ADXL345(0x1F,0x00);   //Y ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
   Single_Write_ADXL345(0x20,0x05);   //Z ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
}
//***********************************************************************
//��ʾx��
void display_x()
{   float temp;
    dis_data=(Z_BUF[1]<<8)+Z_BUF[0];  //�ϳ�����   
	if(dis_data<0){
	dis_data=-dis_data;
    DisplayOneChar(10,0,'-');      //��ʾ��������λ
	}
	else DisplayOneChar(10,0,' '); //��ʾ�ո�

    temp=(float)dis_data*3.9;  //�������ݺ���ʾ,�鿼ADXL345�������ŵ�4ҳ
    conversion(temp);          //ת������ʾ��Ҫ������
	DisplayOneChar(8,0,'X');
    DisplayOneChar(9,0,':'); 
    DisplayOneChar(11,0,qian); 
	DisplayOneChar(12,0,'.'); 
    DisplayOneChar(13,0,bai); 
    DisplayOneChar(14,0,shi); 
	DisplayOneChar(15,0,' '); 
}

//***********************************************************************
//��ʾy��
void display_y()
{   
	float temp;
    dis_data=(Z_BUF[3]<<8)+Z_BUF[2];  //�ϳ�����   
	if(dis_data<0){
	dis_data=-dis_data;
    DisplayOneChar(2,1,'-');      //��ʾ��������λ
	}
	else DisplayOneChar(2,1,' '); //��ʾ�ո�

    temp=(float)dis_data*3.9;  //�������ݺ���ʾ,�鿼ADXL345�������ŵ�4ҳ
    conversion(temp);          //ת������ʾ��Ҫ������
	DisplayOneChar(0,1,'Y');   //��1�У���0�� ��ʾy
    DisplayOneChar(1,1,':'); 
    DisplayOneChar(3,1,qian); 
	DisplayOneChar(4,1,'.'); 
    DisplayOneChar(5,1,bai); 
    DisplayOneChar(6,1,shi);  
	DisplayOneChar(7,1,' ');  
}

//***********************************************************************
//��ʾz��
void display_z()
{
    float temp;
    dis_data=(Z_BUF[5]<<8)+Z_BUF[4];    //�ϳ�����   
	if(dis_data<0){
	dis_data=-dis_data;
    DisplayOneChar(10,1,'-');       //��ʾ������λ
	}
	else DisplayOneChar(10,1,' ');  //��ʾ�ո�

    temp=(float)dis_data*3.9;  //�������ݺ���ʾ,�鿼ADXL345�������ŵ�4ҳ
    conversion(temp);          //ת������ʾ��Ҫ������
	DisplayOneChar(8,1,'Z');  //��0�У���10�� ��ʾZ
    DisplayOneChar(9,1,':'); 
    DisplayOneChar(11,1,qian); 
	DisplayOneChar(12,1,'.'); 
    DisplayOneChar(13,1,bai); 
    DisplayOneChar(14,1,shi); 
	DisplayOneChar(15,1,' ');  
}


//=============================================
main()
{
	float temp;
	uchar devid;
	delay(500);	                   	//�ϵ���ʱ

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
   		
	InitLcd();                      //Һ����ʼ��ADXL345
    Init_BH1750();       //��ʼ��BH1750
 
	DisplayOneChar(0,0,'A');
	DisplayOneChar(1,0,'D'); 
	DisplayOneChar(2,0,'X'); 
	DisplayOneChar(3,0,'L'); 
	DisplayOneChar(4,0,'3'); 
	DisplayOneChar(5,0,'4');  
	DisplayOneChar(6,0,'5'); 

 	while(1)              //ѭ��
 	{ 

	    Single_Write_BH1750(0x01);   // power on
	    Single_Write_BH1750(0x10);   // H- resolution mode
	
	    delay_nms(180);              //��ʱ180ms
	
	    Multiple_read_BH1750();       //�����������ݣ��洢��S_BUF��
	
	    dis_data=S_BUF[0];
	    dis_data=(dis_data<<8)+S_BUF[1];//�ϳ����ݣ�����������
	    
	    temp=(float)dis_data/1.2;
	
	    conversion(temp);         //�������ݺ���ʾ
		printf("Light: %c%c%c%c%c     ",wan,qian,bai,shi,ge);
		StartModule();
		while(!RX);		//��RXΪ��ʱ�ȴ�
		TR0=1;			    //��������
		while(RX);			//��RXΪ1�������ȴ�
		TR0=0;				//�رռ���
	    Conut();			//����
		delay_nms(100);		//100MS  
		 
		DisplayOneChar(0,0,'A');
		DisplayOneChar(1,0,'D'); 
		DisplayOneChar(2,0,'X'); 
		DisplayOneChar(3,0,'L'); 
		DisplayOneChar(4,0,'3'); 
		DisplayOneChar(5,0,'4');  
		DisplayOneChar(6,0,'5'); 
			
		Init_ADXL345();                 	//��ʼ��ADXL345
		devid=Single_Read_ADXL345(0X00);	//����������Ϊ0XE5,��ʾ��ȷ
		if(devid!=0XE5)
		{			
			DisplayOneChar(8,0,'N');
			DisplayOneChar(9,0,'o');		
			DisplayOneChar(10,0,' ');		
			DisplayOneChar(11,0,'F');
			DisplayOneChar(12,0,'i');
			DisplayOneChar(13,0,'n');
			DisplayOneChar(14,0,'d');		
			DisplayOneChar(15,0,'!');		
			//
			DisplayOneChar(0,1,' ');   //��1�У���0�� ��ʾy
			DisplayOneChar(1,1,' '); 
			DisplayOneChar(2,1,' '); 	
			DisplayOneChar(3,1,' '); 
			DisplayOneChar(4,1,' '); 
			DisplayOneChar(5,1,' '); 
			DisplayOneChar(6,1,' ');  
			DisplayOneChar(7,1,' ');  
			DisplayOneChar(8,1,' ');  //��0�У���10�� ��ʾZ
			DisplayOneChar(9,1,' '); 
			DisplayOneChar(11,1,' '); 
			DisplayOneChar(12,1,' '); 
			DisplayOneChar(13,1,' '); 
			DisplayOneChar(14,1,' '); 
			DisplayOneChar(15,1,' ');  			
				
		}
		else
		{			
			Multiple_read_ADXL345();       	//�����������ݣ��洢��BUF��
			display_x();                   	//---------��ʾX��
			display_y();                   	//---------��ʾY��
			display_z();                   	//---------��ʾZ��			    
		}      
		delay(350);                    	//��ʱ  	 
		       
	  }
}
