/*-----------------------------------------------
  名称：LCD1602
  论坛：www.doflye.net
  编写：shifang
  日期：2009.5
  修改：无
  内容：通过矩阵键盘输入，依次显示0-F16中字符
  引脚定义如下：1-VSS 2-VDD 3-V0 4-RS 5-R/W 6-E 7-14 DB0-DB7 15-BLA 16-BLK
------------------------------------------------*/
#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
#include<intrins.h>

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
#define KeyPort P1

unsigned char code dofly_code[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};//转换成液晶显示的字符
/*------------------------------------------------
 uS延时函数，含有输入参数 unsigned char t，无返回值
 unsigned char 是定义无符号字符变量，其值的范围是
 0~255 这里使用晶振12M，精确延时请使用汇编,大致延时
 长度如下 T=tx2+5 uS 
------------------------------------------------*/
void DelayUs2x(unsigned char t)
{   
 while(--t);
}
/*------------------------------------------------
 mS延时函数，含有输入参数 unsigned char t，无返回值
 unsigned char 是定义无符号字符变量，其值的范围是
 0~255 这里使用晶振12M，精确延时请使用汇编
------------------------------------------------*/
void DelayMs(unsigned char t)
{
     
 while(t--)
 {
     //大致延时1mS
     DelayUs2x(245);
	 DelayUs2x(245);
 }
}
/*------------------------------------------------
              判忙函数
------------------------------------------------*/
 bit LCD_Check_Busy(void) 
 { 
 DataPort= 0xFF; 
 RS_CLR; 
 RW_SET; 
 EN_CLR; 
 _nop_(); 
 EN_SET;
 return (bit)(DataPort & 0x80);
 }
/*------------------------------------------------
              写入命令函数
------------------------------------------------*/
 void LCD_Write_Com(unsigned char com) 
 {  
// while(LCD_Check_Busy()); //忙则等待
 DelayMs(5);
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
 //while(LCD_Check_Busy()); //忙则等待
 DelayMs(5);
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
	 LCD_Write_Com(0x80 + x);     
 	}
 else 
 	{     
 	LCD_Write_Com(0xC0 + x);     
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
按键扫描函数，返回扫描键值
------------------------------------------------*/
unsigned char KeyScan(void)  //键盘扫描函数，使用行列反转扫描法
{
 unsigned char cord_h,cord_l;//行列值中间变量
 KeyPort=0x0f;            //行线输出全为0
 cord_h=KeyPort&0x0f;     //读入列线值
 if(cord_h!=0x0f)    //先检测有无按键按下
 {
  DelayMs(10);        //去抖
  if((KeyPort&0x0f)!=0x0f)
  {
    cord_h=KeyPort&0x0f;  //读入列线值
    KeyPort=cord_h|0xf0;  //输出当前列线值
    cord_l=KeyPort&0xf0;  //读入行线值

    while((KeyPort&0xf0)!=0xf0);//等待松开并输出

    return(cord_h+cord_l);//键盘最后组合码值
   }
  }return(0xff);     //返回该值
}
/*------------------------------------------------
按键值处理函数，返回扫键值
------------------------------------------------*/
unsigned char KeyPro(void)
{
 switch(KeyScan())
 {
  case 0x7e:return 0;break;//0 按下相应的键显示相对应的码值
  case 0x7d:return 1;break;//1
  case 0x7b:return 2;break;//2
  case 0x77:return 3;break;//3
  case 0xbe:return 4;break;//4
  case 0xbd:return 5;break;//5
  case 0xbb:return 6;break;//6
  case 0xb7:return 7;break;//7
  case 0xde:return 8;break;//8
  case 0xdd:return 9;break;//9
  case 0xdb:return 10;break;//a
  case 0xd7:return 11;break;//b
  case 0xee:return 12;break;//c
  case 0xed:return 13;break;//d
  case 0xeb:return 14;break;//e
  case 0xe7:return 15;break;//f
  default:return 0xff;break;
 }
}

/*------------------------------------------------
                    主函数
------------------------------------------------*/ 
void main(void) 
{ 
unsigned char i,j,num; 

LCD_Init(); 
LCD_Write_Com(0x0F);//光标开，光标闪烁开

LCD_Write_String(0,0,"Press the key !");

while (1) 
 {  
 num=KeyPro();
 if(num!=0xff)
   {
    if((i==0)&&(j==0))//回到第一个字符时清屏
	    LCD_Clear();//清屏
    LCD_Write_Char(0+i,0+j,dofly_code[num]);//依次显示输入字符
	i++;
	if(i==16)//如果第一行显示满，转到第二行
	  {
	   i=0;j++;
	   if(j==2)//如果2行都显示满，清屏后重新从第一行显示
	   {
	      j=0;
		  }
	  }
   }
 }
}

