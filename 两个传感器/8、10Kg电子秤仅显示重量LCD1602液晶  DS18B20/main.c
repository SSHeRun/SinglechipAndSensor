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


//定义标识
volatile bit FlagTest = 0;		//定时测试标志，每0.5秒置位，测完清0
volatile bit FlagKeyPress = 0;  //有键按下标志，处理完毕清0
//校准参数
//因为不同的传感器特性曲线不是很一致，因此，每一个传感器需要矫正这里这个参数才能使测量值很准确。
//当发现测试出来的重量偏大时，增加该数值。
//如果测试出来的重量偏小时，减小改数值。
//该值可以为小数
#define GapValue 210.6
sbit LED=P1^1;

volatile bit ClearWeighFlag = 0; //传感器调零标志位，清除0漂

#define jump_ROM 0xCC
#define start 0x44
#define read_EEROM 0xBE

sbit DQ = P3^5;            //DS18B20数据口

unsigned char TMPH,TMPL; 

uchar code table[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

/********************************************************************
* 名称 : delay()
* 功能 : 延时,延时时间大概为140US。
* 输入 : 无
* 输出 : 无
***********************************************************************/

void delay_1()
{
	int i,j;
	for(i=0; i<=10; i++)
	for(j=0; j<=2; j++)
;
}
/********************************************************************
* 名称 : delay()
* 功能 : 延时函数
* 输入 : 无
* 输出 : 无
***********************************************************************/
void delay(uint N)
{
	int i;
	for(i=0; i<N; i++)
	;
}

/********************************************************************
* 名称 : Delay_1ms()
* 功能 : 延时子程序，延时时间为 1ms * x
* 输入 : x (延时一毫秒的个数)
* 输出 : 无
***********************************************************************/
void Delay_1ms(uint i)//1ms延时
{
	uchar x,j;
	for(j=0;j<i;j++)
	for(x=0;x<=148;x++);	
}

/********************************************************************
* 名称 : Reset()
* 功能 : 复位DS18B20
* 输入 : 无
* 输出 : 无
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
* 名称 : read_bit()
* 功能 : 从DS18B20读一个位值
* 输入 : 无
* 输出 : 从DS18B20读出的一个位值
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
* 名称 : write_bit()
* 功能 : 向DS18B20写一位
* 输入 : bitval（要对DS18B20写入的位值）
* 输出 : 无
***********************************************************************/
void write_bit(uchar bitval)
{
DQ=0;if(bitval==1)
DQ=1;
delay(5);
DQ=1;
}

/********************************************************************
* 名称 : read_byte()
* 功能 : 从DS18B20读一个字节
* 输入 : 无
* 输出 : 从DS18B20读到的值
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
* 名称 : write_byte()
* 功能 : 向DS18B20写一个字节
* 输入 : val（要对DS18B20写入的命令值）
* 输出 : 无
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

//显示单价，单位为元，四位整数，两位小数
/*void Display_Price()
{
            LCD1602_write_com(0x8c);
		//	LCD1602_write_data(price/100 + 0x30);
		//	LCD1602_write_data(price%100/10 + 0x30);
		//	LCD1602_write_data('.');
		//	LCD1602_write_data(price%10 + 0x30);
}
   */
//显示重量，单位kg，两位整数，三位小数
void Display_Weight()
{
            LCD1602_write_com(0x83);
			LCD1602_write_data(Weight_Shiwu/1000 + 0x30);
			LCD1602_write_data('.');
			LCD1602_write_data(Weight_Shiwu%1000/100 + 0x30);
			LCD1602_write_data(Weight_Shiwu%100/10 + 0x30);
			LCD1602_write_data(Weight_Shiwu%10 + 0x30);
}

//显示总价，单位为元，四位整数，两位小数
/*
void Display_Money()
{
  // unsigned int i,j;

   if (money>9999) 	//超出显示量程
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
//数据初始化
void Data_Init()
{
//   price = 0;
 // DotPos = 0;
}
//定时器0初始化
void Timer0_Init()
{
	ET0 = 1;        //允许定时器0中断
	TMOD = 1;       //定时器工作方式选择
	TL0 = 0x06;     
	TH0 = 0xf8;     //定时器赋予初值
	TR0 = 1;        //启动定时器
}

//定时器0中断
void Timer0_ISR (void) interrupt 1 using 0
{
uchar Counter;
	TL0 = 0x06;
	TH0 = 0xf8;     //定时器赋予初值

	//每0.5秒钟刷新重量
    Counter ++;
    if (Counter >= 200)
    {
       FlagTest = 1;
	   Counter = 0;
    }
}


//按键响应程序，参数是键值
//返回键值：
//         7          8    9      10(清0)
//         4          5    6      11(删除)
//         1          2    3      12(未定义)
//         14(未定义) 0    15(.)  13(确定价格)



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
	     //目前在设置整数位，要注意price是整型，存储单位为分
	     if (DotPos == 0)
		 {  
		    //最多只能设置到千位
			if (price<100)
			{
		       price=price*10+keycode*10;
			}
		 }
		 //目前在设置小数位
		 else if (DotPos==1)  //小数点后第一位
		 {
		    price=price+keycode;
			DotPos=2;
		 }
		 Display_Price();
	     break;
      case 10:   //清零键
	     Get_Maopi();
		 Display_Price();
//		 FlagSetPrice = 0;
		 DotPos = 0;
		 break;
	  case 11:	//删除键，按一次删除最右一个数字
		    price=0;
			DotPos=0;
 		 Display_Price();
         break;
		 ////////////////////////////////////
	  case 12:
		 break;


		 //////////////////////////////////////
//      case 13:   //确认键
//	     FlagSetPrice = 1;
//		 DotPos=0;
//		 break;
	  case 15:   //小数点按下 
			DotPos = 1;      //小数点后第一位
	     break;

   }
}  */
//****************************************************
//主函数
//****************************************************
void main()
{
 uint temp1;
	Init_LCD1602();									//初始化LCD1602
   EA = 0;
   Data_Init();
   Timer0_Init();
   //初中始化完成，开断
   EA = 1;

	
//	Get_Maopi();
	LCD1602_write_com(0x80);						//指针设置
   LCD1602_write_word(" Welcome To Use ");	//  
   LCD1602_write_com(0x80+0x40);						//指针设置
   LCD1602_write_word("Wlectronic Scale");
//   Delay_ms(2000);
   Get_Maopi();
   LCD1602_write_com(0x80);						//指针设置
   LCD1602_write_word("WE:0.000");
//   Display_Price();
//	Get_Maopi();				//称毛皮重量

	while(1)
	{
//每0.5秒称重一次
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
	  //有效键值0-15
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
//称重
//****************************************************
void Get_Weight()
{
 	
	Weight_Shiwu = HX711_Read();
	Weight_Shiwu = Weight_Shiwu - Weight_Maopi;		//获取净重
	
	Weight_Shiwu = (unsigned int)((float)Weight_Shiwu/GapValue); 	//计算实物的实际重量																
	if(Weight_Shiwu > 10000)		//超重报警
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
//		money = Weight_Shiwu*price/1000;  //money单位为分
	       //显示总金额
//	       Display_Money();
	}


}

//****************************************************
//获取毛皮重量
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
//MS延时函数(12M晶振下测试)
//****************************************************
void Delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}