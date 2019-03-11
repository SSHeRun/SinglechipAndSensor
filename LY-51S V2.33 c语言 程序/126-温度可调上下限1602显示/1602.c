/*-----------------------------------------------
  ���ƣ�LCD1602
  ��̳��www.doflye.net
  ��д��shifang
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ�
  ���Ŷ������£�1-VSS 2-VDD 3-V0 4-RS 5-R/W 6-E 7-14 DB0-DB7 15-BLA 16-BLK
------------------------------------------------*/
#include "1602.h"
#include "delay.h"

#define CHECK_BUSY

sbit RS = P2^4;   //����˿� 
sbit RW = P2^5;
sbit EN = P2^6;

#define RS_CLR RS=0 
#define RS_SET RS=1

#define RW_CLR RW=0 
#define RW_SET RW=1 

#define EN_CLR EN=0
#define EN_SET EN=1

#define DataPort P0

/*------------------------------------------------
              ��æ����
------------------------------------------------*/
 bit LCD_Check_Busy(void) 
 { 
#ifdef CHECK_BUSY
 DataPort= 0xFF; 
 RS_CLR; 
 RW_SET; 
 EN_CLR; 
 _nop_(); 
 EN_SET;
 return (bit)(DataPort & 0x80);
#else
 return 0;
#endif
 }
/*------------------------------------------------
              д�������
-----------------   ----------------------------*/
 void LCD_Write_Com(unsigned char com) 
 {  
 while(LCD_Check_Busy()); //æ��ȴ�
 RS_CLR; 
 RW_CLR; 
 EN_SET; 
 DataPort= com; 
 _nop_(); 
 EN_CLR;
 }
/*------------------------------------------------
              д�����ݺ���
------------------------------------------------*/
 void LCD_Write_Data(unsigned char Data) 
 { 
 while(LCD_Check_Busy()); //æ��ȴ�
 RS_SET; 
 RW_CLR; 
 EN_SET; 
 DataPort= Data; 
 _nop_();
 EN_CLR;
 }

/*------------------------------------------------
                ��������
------------------------------------------------*/
 void LCD_Clear(void) 
 { 
 LCD_Write_Com(0x01); 
 DelayMs(5);
 }
/*------------------------------------------------
              д���ַ�������
------------------------------------------------*/
 void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) 
 {     
 if (y == 0) 
 	{     
	 LCD_Write_Com(0x80 + x);     //��ʾ��һ��
 	}
 else 
 	{      
 	LCD_Write_Com(0xC0 + x);      //��ʾ�ڶ���
 	}        
 while (*s) 
 	{     
 LCD_Write_Data( *s);     
 s ++;     
 	}
 }
/*------------------------------------------------
              д���ַ�����
------------------------------------------------*/
 void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data) 
 {     
 if (y == 0) 
 	{     
 	LCD_Write_Com(0x80 + x);     
 	}    
 else 
 	{     
 	LCD_Write_Com(0xC0 + x);     
 	}        
 LCD_Write_Data( Data);  
 }
/*------------------------------------------------
              ��ʼ������
------------------------------------------------*/
 void LCD_Init(void) 
 {
   LCD_Write_Com(0x38);    /*��ʾģʽ����*/ 
   DelayMs(5); 
   LCD_Write_Com(0x38); 
   DelayMs(5); 
   LCD_Write_Com(0x38); 
   DelayMs(5); 
   LCD_Write_Com(0x38);  
   LCD_Write_Com(0x08);    /*��ʾ�ر�*/ 
   LCD_Write_Com(0x01);    /*��ʾ����*/ 
   LCD_Write_Com(0x06);    /*��ʾ����ƶ�����*/ 
   DelayMs(5); 
   LCD_Write_Com(0x0C);    /*��ʾ�����������*/
   }
/*------------------------------------------------   
�趨�����Զ����ַ�,LCD1602���Զ����ַ��ĵ�ַΪ0x00--0x07,
���ɶ���8���ַ�
���������趨��һ���Զ����ַ�����0x00λ�ã�000��,
��һ������0x01λ�ӣ�001��
------------------------------------------------*/
void Lcd_User_Chr(void)
{ //��һ���Զ����ַ�
 LCD_Write_Com(0x40); //"01 000 000"  ��1�е�ַ (D7D6Ϊ��ַ�趨������ʽ�DD5D4D3Ϊ�ַ����λ��(0--7)��D2D1D0Ϊ�ַ��е�ַ(0--7)��
 LCD_Write_Data(0x00); //"XXX 11111" ��1�����ݣ�D7D6D5ΪXXX����ʾΪ������(һ����000����D4D3D2D1D0Ϊ�ַ�������(1-������0-Ϩ��
 LCD_Write_Com(0x41); //"01 000 001"  ��2�е�ַ
 LCD_Write_Data(0x04); //"XXX 10001" ��2������
 LCD_Write_Com(0x42); //"01 000 010"  ��3�е�ַ
 LCD_Write_Data(0x0e); //"XXX 10101" ��3������
 LCD_Write_Com(0x43); //"01 000 011"  ��4�е�ַ
 LCD_Write_Data(0x0e); //"XXX 10001" ��4������
 LCD_Write_Com(0x44); //"01 000 100"  ��5�е�ַ
 LCD_Write_Data(0x0e); //"XXX 11111" ��5������
 LCD_Write_Com(0x45); //"01 000 101"  ��6�е�ַ
 LCD_Write_Data(0x1f); //"XXX 01010" ��6������
 LCD_Write_Com(0x46); //"01 000 110"  ��7�е�ַ
 LCD_Write_Data(0x04); //"XXX 11111" ��7������
 LCD_Write_Com(0x47); //"01 000 111"  ��8�е�ַ
   LCD_Write_Data(0x00); //"XXX 00000" ��8������ 
 //�ڶ����Զ����ַ�

 LCD_Write_Com(0x48); //"01 001 000"  ��1�е�ַ  
 LCD_Write_Data(0x03); //"XXX 00001" ��1������ 
 LCD_Write_Com(0x49); //"01 001 001"  ��2�е�ַ
 LCD_Write_Data(0x03); //"XXX 11011" ��2������
 LCD_Write_Com(0x4a); //"01 001 010"  ��3�е�ַ
 LCD_Write_Data(0x00); //"XXX 11101" ��3������
 LCD_Write_Com(0x4b); //"01 001 011"  ��4�е�ַ
 LCD_Write_Data(0x00); //"XXX 11001" ��4������
 LCD_Write_Com(0x4c); //"01 001 100"  ��5�е�ַ
 LCD_Write_Data(0x00); //"XXX 11101" ��5������
 LCD_Write_Com(0x4d); //"01 001 101"  ��6�е�ַ
 LCD_Write_Data(0x00); //"XXX 11011" ��6������
 LCD_Write_Com(0x4e); //"01 001 110"  ��7�е�ַ
 LCD_Write_Data(0x00); //"XXX 00001" ��7������
 LCD_Write_Com(0x4f); //"01 001 111"  ��8�е�ַ
 LCD_Write_Data(0x00); //"XXX 00000" ��8������ 
 }

