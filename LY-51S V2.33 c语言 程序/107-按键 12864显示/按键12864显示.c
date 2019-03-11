/*-----------------------------------------------
  名称：按键测试单个菜单显示
  论坛：www.doflye.net
  编写：shifang
  日期：2009.5
  修改：无
  内容：单个菜单，多页显示，不带子菜单
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

#define DataPort P0        //单片机 P0<------> 液晶DB0-DB7

sbit KEY_ADD=P3^3;         //定义按键输入端口
sbit KEY_DEC=P3^4;

unsigned char curr,currold;//全局变量，当前箭头位置

unsigned char code user16x16[]={ //箭头图片
0x00,0x00,0x20,0x00,0x30,0x00,0x38,0x00,0x3C,0x00,0x3E,0x00,0x3F,0x00,0x3F,0x80,
0x3F,0xC0,0x3F,0x80,0x3F,0x00,0x3E,0x00,0x3C,0x00,0x38,0x00,0x30,0x00,0x20,0x00,

};
unsigned char code *MainMenu[]=
{
{"  1.设置1"},
{"  2.设置2"},
{"  3.设置3"},
{"  4.设置4"},
{"  5.设置5"},
{"  6.设置6"},
{"  7.设置7"},
{"  8.设置8"},
{"  9.设置9"},
{"  10. 设置10"},
};
/*------------------------------------------------
                    检测忙位
------------------------------------------------*/
void Check_Busy()
{  
    RS=0;
    RW=1;
    E=1;
    DataPort=0xff;
    while((DataPort&0x80)==0x80);//忙则等待
    E=0;
}
/*------------------------------------------------
                   写命令
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
                    写数据
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
                   液晶屏初始化
------------------------------------------------*/
void Init_ST7920()
{  
   DelayMs(40);           //大于40MS的延时程序
   PSB=1;                 //设置为8BIT并口工作模式
   DelayMs(1);            //延时
   RES=0;                 //复位
   DelayMs(1);            //延时
   RES=1;                 //复位置高
   DelayMs(10);
   Write_Cmd(0x30);       //选择基本指令集
   DelayUs2x(50);         //延时大于100us
   Write_Cmd(0x30);       //选择8bit数据流
   DelayUs2x(20);         //延时大于37us
   Write_Cmd(0x0c);       //开显示(无游标、不反白)
   DelayUs2x(50);         //延时大于100us
   Write_Cmd(0x01);       //清除显示，并且设定地址指针为00H
   DelayMs(15);           //延时大于10ms
   Write_Cmd(0x06);       //指定在资料的读取及写入时，设定游标的移动方向及指定显示的移位，光标从右向左加1位移动
   DelayUs2x(50);         //延时大于100us
}
/*------------------------------------------------
                   用户自定义字符
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
                   显示用户自定义字符
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
                   显示字符串
x:横坐标值，范围0~8
y:纵坐标值，范围1~4
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
                      清屏
------------------------------------------------*/
void ClrScreen()
{ 
   Write_Cmd(0x01);
   DelayMs(15);
}
   	
/*------------------------------------------------
                   显示图片
------------------------------------------------*/
/*void LCD_PutGraphic(unsigned char code *img)
{ 
 int i,j;
//显示上半屏内容设置
   for(i=0;i<32;i++)            
    { 
      Write_Cmd(0x80 + i); //SET  垂直地址 VERTICAL ADD
      Write_Cmd(0x80);     //SET  水平地址 HORIZONTAL ADD
      for(j=0;j<16;j++)
       {
         Write_Data(*img);
         img++;
       }
    }
//显示下半屏内容设置
   for(i=0;i<32;i++)            
    {
      Write_Cmd(0x80 + i); //SET 垂直地址 VERTICAL ADD
      Write_Cmd(0x88);     //SET 水平地址 HORIZONTAL ADD
      for(j=0;j<16;j++)
       {
         Write_Data(*img);
         img++;
       }
    }  
           
}*/
/*------------------------------------------------
                 设置到绘图模式
------------------------------------------------*/
/*void SetGraphicMode()
{ 
   Write_Cmd(0x36);       //选择8bit数据流 图形模式
   DelayUs2x(20);

}*/
/*------------------------------------------------
                 调用显示更新
------------------------------------------------*/
void DisplayUpdata(void)
{   
    unsigned char num;
 	ClrScreen();
	num=sizeof(MainMenu)/sizeof(MainMenu[0]);//判断数组中数值个数
	if((0+(curr/4)*4)<num)
      LCD_PutString(0,1,MainMenu[0+(curr/4)*4]);
	else //如果超出数组最大元素，则写空信息，不判断此信息可能会出现乱码 
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
                 主程序
------------------------------------------------*/
main()
{

  Init_ST7920();   //初始化
  CGRAM();     //写入自定义字符 
  DisplayUpdata();
   while(1)
   { 
      
    
 if(curr!=currold) //光标位置变化，则更新显示
	{
    DisplayUpdata();
	currold=curr;
	} 
	
 if(!KEY_ADD)  //如果检测到低电平，说明按键按下
    {
	 DelayMs(10); //延时去抖，一般10-20ms
     if(!KEY_ADD)     //再次确认按键是否按下，没有按下则退出
	   {
        while(!KEY_ADD);//如果确认按下按键等待按键释放，没有释放则一直等待
	       {
		   if(curr<sizeof(MainMenu)/sizeof(MainMenu[0])-1)//判断数组中数值个数
		     {
			  curr++;
              }
			}
	   }
	}

 if(!KEY_DEC)  //如果检测到低电平，说明按键按下
    {
	 DelayMs(10); //延时去抖，一般10-20ms
     if(!KEY_DEC)     //再次确认按键是否按下，没有按下则退出
	   {
        while(!KEY_DEC);//如果确认按下按键等待按键释放，没有释放则一直等待
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

