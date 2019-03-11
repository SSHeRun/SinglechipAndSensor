/*-----------------------------------------------
  ���ƣ��������Ե����˵���ʾ
  ��̳��www.doflye.net
  ��д��shifang
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ������˵�����ҳ��ʾ�������Ӳ˵�
------------------------------------------------*/
#include <reg52.h>
#include <intrins.h>
#include "delay.h"

sbit RS = P2^4;
sbit RW = P2^5;
sbit E  = P2^6;
sbit RES = P2^3;
sbit PSB = P2^1;
sbit PAUSE = P3^0;

#define DataPort P0        //��Ƭ�� P0<------> Һ��DB0-DB7

sbit KEY_ADD=P3^3;         //���尴������˿�
sbit KEY_DEC=P3^4;

unsigned char curr,currold;//ȫ�ֱ�������ǰ��ͷλ��

unsigned char code user16x16[]={ //��ͷͼƬ
0x00,0x00,0x20,0x00,0x30,0x00,0x38,0x00,0x3C,0x00,0x3E,0x00,0x3F,0x00,0x3F,0x80,
0x3F,0xC0,0x3F,0x80,0x3F,0x00,0x3E,0x00,0x3C,0x00,0x38,0x00,0x30,0x00,0x20,0x00,

};
unsigned char code *MainMenu[]=
{
{"  1.����1"},
{"  2.����2"},
{"  3.����3"},
{"  4.����4"},
{"  5.����5"},
{"  6.����6"},
{"  7.����7"},
{"  8.����8"},
{"  9.����9"},
{"  10. ����10"},
};
/*------------------------------------------------
                    ���æλ
------------------------------------------------*/
void Check_Busy()
{  
    RS=0;
    RW=1;
    E=1;
    DataPort=0xff;
    while((DataPort&0x80)==0x80);//æ��ȴ�
    E=0;
}
/*------------------------------------------------
                   д����
------------------------------------------------*/
void Write_Cmd(unsigned char Cmd)
{
	Check_Busy();
	RS=0;
	RW=0;
	E=1;
	DataPort=Cmd;
	DelayUs2x(5);
	E=0;
	DelayUs2x(5);
}
/*------------------------------------------------
                    д����
------------------------------------------------*/
void Write_Data(unsigned char Data)
{
	Check_Busy();
	RS=1;
	RW=0;
	E=1;
	DataPort=Data;
	DelayUs2x(5);
	E=0;
	DelayUs2x(5);
}
/*------------------------------------------------
                   Һ������ʼ��
------------------------------------------------*/
void Init_ST7920()
{  
   DelayMs(40);           //����40MS����ʱ����
   PSB=1;                 //����Ϊ8BIT���ڹ���ģʽ
   DelayMs(1);            //��ʱ
   RES=0;                 //��λ
   DelayMs(1);            //��ʱ
   RES=1;                 //��λ�ø�
   DelayMs(10);
   Write_Cmd(0x30);       //ѡ�����ָ�
   DelayUs2x(50);         //��ʱ����100us
   Write_Cmd(0x30);       //ѡ��8bit������
   DelayUs2x(20);         //��ʱ����37us
   Write_Cmd(0x0c);       //����ʾ(���αꡢ������)
   DelayUs2x(50);         //��ʱ����100us
   Write_Cmd(0x01);       //�����ʾ�������趨��ַָ��Ϊ00H
   DelayMs(15);           //��ʱ����10ms
   Write_Cmd(0x06);       //ָ�������ϵĶ�ȡ��д��ʱ���趨�α���ƶ�����ָ����ʾ����λ�������������1λ�ƶ�
   DelayUs2x(50);         //��ʱ����100us
}
/*------------------------------------------------
                   �û��Զ����ַ�
------------------------------------------------*/
void CGRAM()
{ 
     int i;
     Write_Cmd(0x30); 
     Write_Cmd(0x40);
     for(i=0;i<16;i++)
       {
	    Write_Data(user16x16[i*2]);
        Write_Data(user16x16[i*2+1]);
      }
}   
/*------------------------------------------------
                   ��ʾ�û��Զ����ַ�
------------------------------------------------*/
void DisplayCGRAM(unsigned char x,unsigned char y)
{ 
 switch(y)
     {
	  case 1: Write_Cmd(0x80+x);break;
	  case 2: Write_Cmd(0x90+x);break;
	  case 3: Write_Cmd(0x88+x);break;
	  case 4: Write_Cmd(0x98+x);break;
      default:break;
	 }
    Write_Data(00);
    Write_Data(00);

}         
/*------------------------------------------------
                   ��ʾ�ַ���
x:������ֵ����Χ0~8
y:������ֵ����Χ1~4
------------------------------------------------*/
void LCD_PutString(unsigned char x,unsigned char y,unsigned char code *s)
{ 
 switch(y)
     {
	  case 1: Write_Cmd(0x80+x);break;
	  case 2: Write_Cmd(0x90+x);break;
	  case 3: Write_Cmd(0x88+x);break;
	  case 4: Write_Cmd(0x98+x);break;
      default:break;
	 }
 while(*s>0)
   { 
      Write_Data(*s);
      s++;
      DelayUs2x(50);
   }
}
/*------------------------------------------------
                      ����
------------------------------------------------*/
void ClrScreen()
{ 
   Write_Cmd(0x01);
   DelayMs(15);
}
   	
/*------------------------------------------------
                   ��ʾͼƬ
------------------------------------------------*/
/*void LCD_PutGraphic(unsigned char code *img)
{ 
 int i,j;
//��ʾ�ϰ�����������
   for(i=0;i<32;i++)            
    { 
      Write_Cmd(0x80 + i); //SET  ��ֱ��ַ VERTICAL ADD
      Write_Cmd(0x80);     //SET  ˮƽ��ַ HORIZONTAL ADD
      for(j=0;j<16;j++)
       {
         Write_Data(*img);
         img++;
       }
    }
//��ʾ�°�����������
   for(i=0;i<32;i++)            
    {
      Write_Cmd(0x80 + i); //SET ��ֱ��ַ VERTICAL ADD
      Write_Cmd(0x88);     //SET ˮƽ��ַ HORIZONTAL ADD
      for(j=0;j<16;j++)
       {
         Write_Data(*img);
         img++;
       }
    }  
           
}*/
/*------------------------------------------------
                 ���õ���ͼģʽ
------------------------------------------------*/
/*void SetGraphicMode()
{ 
   Write_Cmd(0x36);       //ѡ��8bit������ ͼ��ģʽ
   DelayUs2x(20);

}*/
/*------------------------------------------------
                 ������ʾ����
------------------------------------------------*/
void DisplayUpdata(void)
{   
    unsigned char num;
 	ClrScreen();
	num=sizeof(MainMenu)/sizeof(MainMenu[0]);//�ж���������ֵ����
	if((0+(curr/4)*4)<num)
      LCD_PutString(0,1,MainMenu[0+(curr/4)*4]);
	else //��������������Ԫ�أ���д����Ϣ�����жϴ���Ϣ���ܻ�������� 
	  LCD_PutString(0,1,"");
	if((1+(curr/4)*4)<num)
	  LCD_PutString(0,2,MainMenu[1+(curr/4)*4]);
	else
	  LCD_PutString(0,2,"");
	if((2+(curr/4)*4)<num)
	  LCD_PutString(0,3,MainMenu[2+(curr/4)*4]);
	else
	  LCD_PutString(0,3,"");
	if((3+(curr/4)*4)<num)
	  LCD_PutString(0,4,MainMenu[3+(curr/4)*4]);
	else
	  LCD_PutString(0,4,"");
	DisplayCGRAM(0,curr%4+1); 
}
/*------------------------------------------------
                 ������
------------------------------------------------*/
main()
{

  Init_ST7920();   //��ʼ��
  CGRAM();     //д���Զ����ַ� 
  DisplayUpdata();
   while(1)
   { 
      
    
 if(curr!=currold) //���λ�ñ仯���������ʾ
	{
    DisplayUpdata();
	currold=curr;
	} 
	
 if(!KEY_ADD)  //�����⵽�͵�ƽ��˵����������
    {
	 DelayMs(10); //��ʱȥ����һ��10-20ms
     if(!KEY_ADD)     //�ٴ�ȷ�ϰ����Ƿ��£�û�а������˳�
	   {
        while(!KEY_ADD);//���ȷ�ϰ��°����ȴ������ͷţ�û���ͷ���һֱ�ȴ�
	       {
		   if(curr<sizeof(MainMenu)/sizeof(MainMenu[0])-1)//�ж���������ֵ����
		     {
			  curr++;
              }
			}
	   }
	}

 if(!KEY_DEC)  //�����⵽�͵�ƽ��˵����������
    {
	 DelayMs(10); //��ʱȥ����һ��10-20ms
     if(!KEY_DEC)     //�ٴ�ȷ�ϰ����Ƿ��£�û�а������˳�
	   {
        while(!KEY_DEC);//���ȷ�ϰ��°����ȴ������ͷţ�û���ͷ���һֱ�ȴ�
	       {
	       if(curr>0)
		     {
			  curr--;

              }
	 	   }
	   }
	}  
  }
}

