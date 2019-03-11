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
//键盘处理变量
unsigned char keycode;
unsigned char DotPos;				   //小数点标志及位置

unsigned long idata price;     //单价，长整型值，单位为分   
unsigned long idata money;     //总价，长整型值，单位为分
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


sbit IRIN = P3^2;         //红外接收器数据线

uchar IRCOM[7];

sbit E=P2^3;
sbit RW=P2^4;
sbit RS=P2^5;

#define LCM_Data  P0

uchar code  cdis1[ ] = {"  Red Control  "};
uchar code  cdis2[ ] = {"  IR-CODE:--H  "};


//显示单价，单位为元，四位整数，两位小数
void Display_Price()
{
            LCD1602_write_com(0x8c);
			LCD1602_write_data(price/100 + 0x30);
			LCD1602_write_data(price%100/10 + 0x30);
			LCD1602_write_data('.');
			LCD1602_write_data(price%10 + 0x30);
}

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

//数据初始化
void Data_Init()
{
   price = 0;
   DotPos = 0;
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
}

//延时设置
void Delay(unsigned int t)   // delay 40us
{
 for(;t!=0;t--) ;
}


//写指令
void WriteCommandLCM(unsigned char ch)
{
   RS=0;
   RW=0;
   E=1;
   LCM_Data=ch;
   Delay(1);
   E=0;
   Delay(100);  //delay 40us
}

//写数据
void WriteDataLCM(unsigned char ch)
{  RS=1;
   RW=0;
   E=1;
   LCM_Data=ch;
   Delay(1);
   E=0;
   Delay(100); //delay 40us
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
/*  LCD初始化设定                                                  */
/*******************************************************************/
void InitLcd()
{
 WriteCommandLCM(0x30);		 //功能设置，设置8位数据接口，
 WriteCommandLCM(0x30);
 WriteCommandLCM(0x30);
 WriteCommandLCM(0x38);	//设置工作方式,8位数据接口，两行显示，5*7点阵
 WriteCommandLCM(0x08);	//关显示，关闭显示，关闭光标，关闭闪烁
 WriteCommandLCM(0x0c); //显示状态设置，开显示，关闭光标与闪烁
 WriteCommandLCM(0x01); //清屏
 WriteCommandLCM(0x06); //输入方式设置 ,AC自动增一，画面不动
}



//****************************************************
//主函数
//****************************************************
void main()
{
/*
	
	*/
	 uchar m;
    
    IRIN=1;                    //I/O口初始化

    Delay(10);                 //延时
    InitLcd();                //初始化LCD             

     m = 0;
    while(cdis1[m] != '\0')
     {                         //显示字符
       WriteDataLCM(cdis1[m]);
       m++;
     }

    //lcd_pos(0x40);             //设置显示位置为第二行第1个字符
	WriteCommandLCM(0x40 | 0x80);
     m = 0;
    while(cdis2[m] != '\0')
     {
       WriteDataLCM(cdis2[m]);      //显示字符
       m++;
     }
   

	IE = 0x81;                 //允许总中断中断,使能 INT0 外部中断
	TCON = 0x01;               //触发方式为脉冲负边沿触发

   while(1)	;

}

/**********************************************************/
void IR_IN() interrupt 0 using 0
{
  unsigned char j,k,N=0;
     EX0 = 0;   
	 Delay(200);
	 if (IRIN==1) 
     { EX0 =1;
	   return;
	  } 
                           //确认IR信号出现
  while (!IRIN)            //等IR变为高电平，跳过9ms的前导低电平信号。
    {Delay(13);}

 for (j=0;j<4;j++)         //收集四组数据
 { 
  for (k=0;k<8;k++)        //每组数据有8位
  {
   while (IRIN)            //等 IR 变为低电平，跳过4.5ms的前导高电平信号。
     {Delay(13);}
    while (!IRIN)          //等 IR 变为高电平
     {Delay(13);}
     while (IRIN)           //计算IR高电平时长
      {
   Delay(13);
    N++;           
    if (N>=30)
	 { EX0=1;
	 return;}                  //0.14ms计数过长自动离开。
      }                        //高电平计数完毕                
     IRCOM[j]=IRCOM[j] >> 1;                  //数据最高位补“0”
     if (N>=8) {IRCOM[j] = IRCOM[j] | 0x80;}  //数据最高位补“1”
     N=0;
  }//end for k
 }//end for j
   
   if (IRCOM[2]!=~IRCOM[3])
   { EX0=1;
     return; }

   IRCOM[5]=IRCOM[2] & 0x0F;     //取键码的低四位
   IRCOM[6]=IRCOM[2] >> 4;       //右移4次，高四位变为低四位

   if(IRCOM[5]>9)
    { IRCOM[5]=IRCOM[5]+0x37;}
   else
	  IRCOM[5]=IRCOM[5]+0x30;

   if(IRCOM[6]>9)
    { IRCOM[6]=IRCOM[6]+0x37;}
   else
	  IRCOM[6]=IRCOM[6]+0x30;

	if(IRCOM[5] == '8' && IRCOM[6] == '1'){
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
   LCD1602_write_word("WE:0.000 PR:00.0");
   LCD1602_write_com(0x80+0x40);				//指针设置
   LCD1602_write_word("MONEY:  0.00    ");
   Display_Price();
//	Get_Maopi();				//称毛皮重量

	while(1)
	{
//每0.5秒称重一次
	  if (FlagTest==1)
	  {
			Get_Weight();
			FlagTest = 0;
	}			
	  	
	  keycode = Getkeyboard();
	  //有效键值0-15
	  if (keycode<16/*&&(FlagKeyPress==0)*/)  
	  {
	     
		 KeyPress(keycode);
		 
		 Buzzer=0;
		 Delay_ms(50);
		 Buzzer=1;
	//	 FlagKeyPress = 1;
		 Delay_ms(200);
	  }
	}
	}
           
     DisplayOneChar(10,1,IRCOM[6]);          //第一位数显示         
     DisplayOneChar(11,1,IRCOM[5]);        //第二位数显示

     EX0 = 1; 
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
		money = Weight_Shiwu*price/1000;  //money单位为分
	       //显示总金额
	       Display_Money();
	}
}

//****************************************************
//获取毛皮重量
//****************************************************
void Get_Maopi()
{
mm:	Weight_Maopi_0 = HX711_Read();
	Delay_ms(2000);
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

