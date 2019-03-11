#include <reg52.h>
#include <intrins.h>
#include <string.h>
#include "main.h"
#include "LCD1602.h"
#include "HX711.h"
#include "keyboard.h"

#define uchar unsigned char
#define uint  unsigned int

unsigned long HX711_Buffer = 0;
unsigned long Weight_Maopi = 0;
unsigned long Weight_Maopi_0 = 0;
long Weight_Shiwu = 0;


//�����ʶ
volatile bit FlagTest = 0;		//��ʱ���Ա�־��ÿ0.5����λ��������0
volatile bit FlagKeyPress = 0;  //�м����±�־�����������0
//У׼����
//��Ϊ��ͬ�Ĵ������������߲��Ǻ�һ�£���ˣ�ÿһ����������Ҫ�������������������ʹ����ֵ��׼ȷ��
//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
//������Գ���������ƫСʱ����С����ֵ��
//��ֵ����ΪС��
#define GapValue 210.6
sbit LED=P1^1;

volatile bit ClearWeighFlag = 0; //�����������־λ�����0Ư

#define jump_ROM 0xCC
#define start 0x44
#define read_EEROM 0xBE

sbit DQ = P3^5;            //DS18B20���ݿ�

unsigned char TMPH,TMPL; 

uchar code table[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

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

//��ʾ���ۣ���λΪԪ����λ��������λС��
/*void Display_Price()
{
            LCD1602_write_com(0x8c);
		//	LCD1602_write_data(price/100 + 0x30);
		//	LCD1602_write_data(price%100/10 + 0x30);
		//	LCD1602_write_data('.');
		//	LCD1602_write_data(price%10 + 0x30);
}
   */
//��ʾ��������λkg����λ��������λС��
void Display_Weight()
{
            LCD1602_write_com(0x83);
			LCD1602_write_data(Weight_Shiwu/1000 + 0x30);
			LCD1602_write_data('.');
			LCD1602_write_data(Weight_Shiwu%1000/100 + 0x30);
			LCD1602_write_data(Weight_Shiwu%100/10 + 0x30);
			LCD1602_write_data(Weight_Shiwu%10 + 0x30);
}

//��ʾ�ܼۣ���λΪԪ����λ��������λС��
/*
void Display_Money()
{
  // unsigned int i,j;

   if (money>9999) 	//������ʾ����
   {
     LCD1602_write_com(0x80+0x40+6);
     LCD1602_write_word("---.-");
      return;       
   }   
   
   if (money>=1000)
   {
       LCD1602_write_com(0x80+0x40+6);
	   LCD1602_write_data(money/1000 + 0x30);
	   LCD1602_write_data(money%1000/100 + 0x30);
	   LCD1602_write_data(money%100/10 + 0x30);
	   LCD1602_write_data('.');
	   LCD1602_write_data(money%10 + 0x30);
   }
   else if (money>=100)
   {
       LCD1602_write_com(0x80+0x40+6);
	   LCD1602_write_data(0x20);
	   LCD1602_write_data(money%1000/100 + 0x30);
	   LCD1602_write_data(money%100/10 + 0x30);
	   LCD1602_write_data('.');
	   LCD1602_write_data(money%10 + 0x30);
   }
    else if(money>=10)
   {
       LCD1602_write_com(0x80+0x40+6);
	  LCD1602_write_data(0x20);
	    LCD1602_write_com(0x80+0x40+7);
	   LCD1602_write_data(0x20);
	   LCD1602_write_data(money%100/10 + 0x30);
	   LCD1602_write_data('.');
	   LCD1602_write_data(money%10+ 0x30);
   }   
   else 
     {
       LCD1602_write_com(0x80+0x40+6);
	   LCD1602_write_data(0x20);
	   LCD1602_write_com(0x80+0x40+7);
	   LCD1602_write_data(0x20);
	   LCD1602_write_com(0x80+0x40+8);
	   LCD1602_write_data(0 + 0x30);
	   LCD1602_write_data('.');
	   LCD1602_write_data(money%10 + 0x30);
   }         
}
 */
//���ݳ�ʼ��
void Data_Init()
{
//   price = 0;
 // DotPos = 0;
}
//��ʱ��0��ʼ��
void Timer0_Init()
{
	ET0 = 1;        //����ʱ��0�ж�
	TMOD = 1;       //��ʱ��������ʽѡ��
	TL0 = 0x06;     
	TH0 = 0xf8;     //��ʱ�������ֵ
	TR0 = 1;        //������ʱ��
}

//��ʱ��0�ж�
void Timer0_ISR (void) interrupt 1 using 0
{
uchar Counter;
	TL0 = 0x06;
	TH0 = 0xf8;     //��ʱ�������ֵ

	//ÿ0.5����ˢ������
    Counter ++;
    if (Counter >= 200)
    {
       FlagTest = 1;
	   Counter = 0;
    }
}


//������Ӧ���򣬲����Ǽ�ֵ
//���ؼ�ֵ��
//         7          8    9      10(��0)
//         4          5    6      11(ɾ��)
//         1          2    3      12(δ����)
//         14(δ����) 0    15(.)  13(ȷ���۸�)



/*
void KeyPress(uchar keycode)
{
   switch (keycode)
   {
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
      case 9:
	     //Ŀǰ����������λ��Ҫע��price�����ͣ��洢��λΪ��
	     if (DotPos == 0)
		 {  
		    //���ֻ�����õ�ǧλ
			if (price<100)
			{
		       price=price*10+keycode*10;
			}
		 }
		 //Ŀǰ������С��λ
		 else if (DotPos==1)  //С������һλ
		 {
		    price=price+keycode;
			DotPos=2;
		 }
		 Display_Price();
	     break;
      case 10:   //�����
	     Get_Maopi();
		 Display_Price();
//		 FlagSetPrice = 0;
		 DotPos = 0;
		 break;
	  case 11:	//ɾ��������һ��ɾ������һ������
		    price=0;
			DotPos=0;
 		 Display_Price();
         break;
		 ////////////////////////////////////
	  case 12:
		 break;


		 //////////////////////////////////////
//      case 13:   //ȷ�ϼ�
//	     FlagSetPrice = 1;
//		 DotPos=0;
//		 break;
	  case 15:   //С���㰴�� 
			DotPos = 1;      //С������һλ
	     break;

   }
}  */
//****************************************************
//������
//****************************************************
void main()
{
 uint temp1;
	Init_LCD1602();									//��ʼ��LCD1602
   EA = 0;
   Data_Init();
   Timer0_Init();
   //����ʼ����ɣ�����
   EA = 1;

	
//	Get_Maopi();
	LCD1602_write_com(0x80);						//ָ������
   LCD1602_write_word(" Welcome To Use ");	//  
   LCD1602_write_com(0x80+0x40);						//ָ������
   LCD1602_write_word("Wlectronic Scale");
//   Delay_ms(2000);
   Get_Maopi();
   LCD1602_write_com(0x80);						//ָ������
   LCD1602_write_word("WE:0.000");
//   Display_Price();
//	Get_Maopi();				//��ëƤ����

	while(1)
	{
//ÿ0.5�����һ��
	  if (FlagTest==1)
	  {
			Get_Weight();
			FlagTest = 0;
			  
		P2 = 0x00;
		Reset();
		write_byte(jump_ROM);
		write_byte(start);
		Reset();
		write_byte(jump_ROM);
		write_byte(read_EEROM);
		TMPL = read_byte();
		TMPH = read_byte();
		temp1 = TMPL / 16 + TMPH * 16;
		P0 = table[temp1/10%10];
		if(P0 == 0x7f){
			P0 = 0x07;
		}
		P2 = 6;
		Delay_1ms(5);
		P0 = table[temp1%10];
		P2 = 7;
		Delay_1ms(5);
	}			
	  	
//	  keycode = Getkeyboard();
	  //��Ч��ֵ0-15
//	  if (keycode<16/*&&(FlagKeyPress==0)*/)
	  {
	     
//		 KeyPress(keycode);
		 
		 Buzzer=0;
		 Delay_ms(50);
		 Buzzer=1;
	//	 FlagKeyPress = 1;
		 Delay_ms(200);
	  }
	}
}



//****************************************************
//����
//****************************************************
void Get_Weight()
{
 	
	Weight_Shiwu = HX711_Read();
	Weight_Shiwu = Weight_Shiwu - Weight_Maopi;		//��ȡ����
	
	Weight_Shiwu = (unsigned int)((float)Weight_Shiwu/GapValue); 	//����ʵ���ʵ������																
	if(Weight_Shiwu > 10000)		//���ر���
	{
		Buzzer = 0;	
		LED=0;
		LCD1602_write_com(0x83);
	   LCD1602_write_word("-.---");
	  
	
		
		}
	else
	{
		if(Weight_Shiwu==0)
		LED=1;
		else if(Weight_Shiwu>0)
		LED=0; 
		Buzzer = 1;
		Display_Weight();
//		money = Weight_Shiwu*price/1000;  //money��λΪ��
	       //��ʾ�ܽ��
//	       Display_Money();
	}


}

//****************************************************
//��ȡëƤ����
//****************************************************
void Get_Maopi()
{
mm:	Weight_Maopi_0 = HX711_Read();
	Delay_ms(200);
	Weight_Maopi = HX711_Read();
	if(Weight_Maopi/GapValue!=Weight_Maopi_0/GapValue)
	goto mm;
} 

//****************************************************
//MS��ʱ����(12M�����²���)
//****************************************************
void Delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}