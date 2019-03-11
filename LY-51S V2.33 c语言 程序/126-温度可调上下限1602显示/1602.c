/*-----------------------------------------------
  名称：LCD1602
  论坛：www.doflye.net
  编写：shifang
  日期：2009.5
  修改：无
  内容：
  引脚定义如下：1-VSS 2-VDD 3-V0 4-RS 5-R/W 6-E 7-14 DB0-DB7 15-BLA 16-BLK
------------------------------------------------*/
#include "1602.h"
#include "delay.h"

#define CHECK_BUSY

sbit RS = P2^4;   //定义端口 
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
              判忙函数
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
              写入命令函数
-----------------   ----------------------------*/
 void LCD_Write_Com(unsigned char com) 
 {  
 while(LCD_Check_Busy()); //忙则等待
 RS_CLR; 
 RW_CLR; 
 EN_SET; 
 DataPort= com; 
 _nop_(); 
 EN_CLR;
 }
/*------------------------------------------------
              写入数据函数
------------------------------------------------*/
 void LCD_Write_Data(unsigned char Data) 
 { 
 while(LCD_Check_Busy()); //忙则等待
 RS_SET; 
 RW_CLR; 
 EN_SET; 
 DataPort= Data; 
 _nop_();
 EN_CLR;
 }

/*------------------------------------------------
                清屏函数
------------------------------------------------*/
 void LCD_Clear(void) 
 { 
 LCD_Write_Com(0x01); 
 DelayMs(5);
 }
/*------------------------------------------------
              写入字符串函数
------------------------------------------------*/
 void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) 
 {     
 if (y == 0) 
 	{     
	 LCD_Write_Com(0x80 + x);     //表示第一行
 	}
 else 
 	{      
 	LCD_Write_Com(0xC0 + x);      //表示第二行
 	}        
 while (*s) 
 	{     
 LCD_Write_Data( *s);     
 s ++;     
 	}
 }
/*------------------------------------------------
              写入字符函数
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
              初始化函数
------------------------------------------------*/
 void LCD_Init(void) 
 {
   LCD_Write_Com(0x38);    /*显示模式设置*/ 
   DelayMs(5); 
   LCD_Write_Com(0x38); 
   DelayMs(5); 
   LCD_Write_Com(0x38); 
   DelayMs(5); 
   LCD_Write_Com(0x38);  
   LCD_Write_Com(0x08);    /*显示关闭*/ 
   LCD_Write_Com(0x01);    /*显示清屏*/ 
   LCD_Write_Com(0x06);    /*显示光标移动设置*/ 
   DelayMs(5); 
   LCD_Write_Com(0x0C);    /*显示开及光标设置*/
   }
/*------------------------------------------------   
设定二个自定义字符,LCD1602中自定义字符的地址为0x00--0x07,
即可定义8个字符
这里我们设定把一个自定义字符放在0x00位置（000）,
另一个放在0x01位子（001）
------------------------------------------------*/
void Lcd_User_Chr(void)
{ //第一个自定义字符
 LCD_Write_Com(0x40); //"01 000 000"  第1行地址 (D7D6为地址设定命令形式D5D4D3为字符存放位置(0--7)，D2D1D0为字符行地址(0--7)）
 LCD_Write_Data(0x00); //"XXX 11111" 第1行数据（D7D6D5为XXX，表示为任意数(一般用000），D4D3D2D1D0为字符行数据(1-点亮，0-熄灭）
 LCD_Write_Com(0x41); //"01 000 001"  第2行地址
 LCD_Write_Data(0x04); //"XXX 10001" 第2行数据
 LCD_Write_Com(0x42); //"01 000 010"  第3行地址
 LCD_Write_Data(0x0e); //"XXX 10101" 第3行数据
 LCD_Write_Com(0x43); //"01 000 011"  第4行地址
 LCD_Write_Data(0x0e); //"XXX 10001" 第4行数据
 LCD_Write_Com(0x44); //"01 000 100"  第5行地址
 LCD_Write_Data(0x0e); //"XXX 11111" 第5行数据
 LCD_Write_Com(0x45); //"01 000 101"  第6行地址
 LCD_Write_Data(0x1f); //"XXX 01010" 第6行数据
 LCD_Write_Com(0x46); //"01 000 110"  第7行地址
 LCD_Write_Data(0x04); //"XXX 11111" 第7行数据
 LCD_Write_Com(0x47); //"01 000 111"  第8行地址
   LCD_Write_Data(0x00); //"XXX 00000" 第8行数据 
 //第二个自定义字符

 LCD_Write_Com(0x48); //"01 001 000"  第1行地址  
 LCD_Write_Data(0x03); //"XXX 00001" 第1行数据 
 LCD_Write_Com(0x49); //"01 001 001"  第2行地址
 LCD_Write_Data(0x03); //"XXX 11011" 第2行数据
 LCD_Write_Com(0x4a); //"01 001 010"  第3行地址
 LCD_Write_Data(0x00); //"XXX 11101" 第3行数据
 LCD_Write_Com(0x4b); //"01 001 011"  第4行地址
 LCD_Write_Data(0x00); //"XXX 11001" 第4行数据
 LCD_Write_Com(0x4c); //"01 001 100"  第5行地址
 LCD_Write_Data(0x00); //"XXX 11101" 第5行数据
 LCD_Write_Com(0x4d); //"01 001 101"  第6行地址
 LCD_Write_Data(0x00); //"XXX 11011" 第6行数据
 LCD_Write_Com(0x4e); //"01 001 110"  第7行地址
 LCD_Write_Data(0x00); //"XXX 00001" 第7行数据
 LCD_Write_Com(0x4f); //"01 001 111"  第8行地址
 LCD_Write_Data(0x00); //"XXX 00000" 第8行数据 
 }

