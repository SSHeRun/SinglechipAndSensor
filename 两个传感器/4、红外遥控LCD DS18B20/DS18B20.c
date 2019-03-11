/********************************************************************
*��ʾ��ʵ�������¶ȴ�������DS18B20���¶ȵĲɼ����������������ʾ������
***********************************************************************/
#include<reg52.h>
#define uchar unsigned char
#define uint  unsigned int

#define jump_ROM 0xCC
#define start 0x44
#define read_EEROM 0xBE

sbit DQ = P3^5;            //DS18B20���ݿ�

unsigned char TMPH,TMPL; 

uchar code table[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

sbit IRIN = P3^2;         //���������������

uchar IRCOM[7];

sbit E=P2^3;
sbit RW=P2^4;
sbit RS=P2^5;

#define LCM_Data  P0

uchar code  cdis1[ ] = {"  Red Control  "};
uchar code  cdis2[ ] = {"  IR-CODE:--H  "};
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

//��ʱ����
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
/*

	*/
 uchar m;
    
    IRIN=1;                    //I/O�ڳ�ʼ��

    Delay1(10);                 //��ʱ
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

   while(1)	;

} //end main

/**********************************************************/
void IR_IN() interrupt 0 using 0
{
  unsigned char j,k,N=0;
     EX0 = 0;   
	 Delay1(200);
	 if (IRIN==1) 
     { EX0 =1;
	   return;
	  } 
                           //ȷ��IR�źų���
  while (!IRIN)            //��IR��Ϊ�ߵ�ƽ������9ms��ǰ���͵�ƽ�źš�
    {Delay1(13);}

 for (j=0;j<4;j++)         //�ռ���������
 { 
  for (k=0;k<8;k++)        //ÿ��������8λ
  {
   while (IRIN)            //�� IR ��Ϊ�͵�ƽ������4.5ms��ǰ���ߵ�ƽ�źš�
     {Delay1(13);}
    while (!IRIN)          //�� IR ��Ϊ�ߵ�ƽ
     {Delay1(13);}
     while (IRIN)           //����IR�ߵ�ƽʱ��
      {
   Delay1(13);
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

	 if(IRCOM[6] == '0' && IRCOM[5] == '8'){
	 	    	//float tt;
	uint temp;
	P2 = 0x00;
	while(1)
	{
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