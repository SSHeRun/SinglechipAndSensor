#include <reg52.h>
#include <intrins.h>
#include  <math.h>    //Keil library  
#include  <stdio.h>   //Keil library
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

sbit E=P2^3;
sbit RW=P2^4;
sbit RS=P2^5;

#define LCM_Data  P0

#define   uchar unsigned char
#define   uint unsigned int	

sbit	  SCL=P1^0;      //IIC时钟引脚定义
sbit  	  SDA=P1^1;      //IIC数据引脚定义

#define	  SlaveAddress   0x46 //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
                              //ALT  ADDRESS引脚接地时地址为0x46，接电源时地址为0xB8
typedef   unsigned char BYTE;
typedef   unsigned short WORD;

BYTE    BUF[8];                         //接收数据缓存区      	
uchar   ge,shi,bai,qian,wan;            //显示变量
int     dis_data;                       //变量

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


//*********************************************************
void conversion(uint temp_data)  //  数据转换出 个，十，百，千，万
{  
    wan=temp_data/10000+0x30 ;
    temp_data=temp_data%10000;   //取余运算
	qian=temp_data/1000+0x30 ;
    temp_data=temp_data%1000;    //取余运算
    bai=temp_data/100+0x30   ;
    temp_data=temp_data%100;     //取余运算
    shi=temp_data/10+0x30    ;
    temp_data=temp_data%10;      //取余运算
    ge=temp_data+0x30; 	
}

//毫秒延时**************************
void delay_nms(unsigned int k)	
{						
unsigned int i,j;				
for(i=0;i<k;i++)
{			
for(j=0;j<121;j++)			
{;}}						
}

/**************************************
延时5微秒(STC90C52RC@12M)
不同的工作环境,需要调整此函数，注意时钟过快时需要修改
当改用1T的MCU时,请调整此延时函数
**************************************/
void Delay5us()
{
    _nop_();_nop_();_nop_();_nop_();
    _nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
}

/**************************************
延时5毫秒(STC90C52RC@12M)
不同的工作环境,需要调整此函数
当改用1T的MCU时,请调整此延时函数
**************************************/
void Delay5ms()
{
    WORD n = 560;

    while (n--);
}

/**************************************
起始信号
**************************************/
void BH1750_Start()
{
    SDA = 1;                    //拉高数据线
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SDA = 0;                    //产生下降沿
    Delay5us();                 //延时
    SCL = 0;                    //拉低时钟线
}

/**************************************
停止信号
**************************************/
void BH1750_Stop()
{
    SDA = 0;                    //拉低数据线
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SDA = 1;                    //产生上升沿
    Delay5us();                 //延时
}

/**************************************
发送应答信号
入口参数:ack (0:ACK 1:NAK)
**************************************/
void BH1750_SendACK(bit ack)
{
    SDA = ack;                  //写应答信号
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时
}

/**************************************
接收应答信号
**************************************/
bit BH1750_RecvACK()
{
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    CY = SDA;                   //读应答信号
    SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时

    return CY;
}

/**************************************
向IIC总线发送一个字节数据
**************************************/
void BH1750_SendByte(BYTE dat)
{
    BYTE i;

    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;              //移出数据的最高位
        SDA = CY;               //送数据口
        SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    BH1750_RecvACK();
}

/**************************************
从IIC总线接收一个字节数据
**************************************/
BYTE BH1750_RecvByte()
{
    BYTE i;
    BYTE dat = 0;

    SDA = 1;                    //使能内部上拉,准备读取数据,
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;
        SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        dat |= SDA;             //读数据               
        SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    return dat;
}

//*********************************

void Single_Write_BH1750(uchar REG_Address)
{
    BH1750_Start();                  //起始信号
    BH1750_SendByte(SlaveAddress);   //发送设备地址+写信号
    BH1750_SendByte(REG_Address);    //内部寄存器地址，
  //  BH1750_SendByte(REG_data);       //内部寄存器数据，
    BH1750_Stop();                   //发送停止信号
}

//********单字节读取*****************************************
/*
uchar Single_Read_BH1750(uchar REG_Address)
{  uchar REG_data;
    BH1750_Start();                          //起始信号
    BH1750_SendByte(SlaveAddress);           //发送设备地址+写信号
    BH1750_SendByte(REG_Address);                   //发送存储单元地址，从0开始	
    BH1750_Start();                          //起始信号
    BH1750_SendByte(SlaveAddress+1);         //发送设备地址+读信号
    REG_data=BH1750_RecvByte();              //读出寄存器数据
	BH1750_SendACK(1);   
	BH1750_Stop();                           //停止信号
    return REG_data; 
}
*/
//*********************************************************
//
//连续读出BH1750内部数据
//
//*********************************************************
void Multiple_read_BH1750(void)
{   uchar i;	
    BH1750_Start();                          //起始信号
    BH1750_SendByte(SlaveAddress+1);         //发送设备地址+读信号
	
	 for (i=0; i<3; i++)                      //连续读取2个地址数据，存储中BUF
    {
        BUF[i] = BH1750_RecvByte();          //BUF[0]存储0x32地址中的数据
        if (i == 3)
        {

           BH1750_SendACK(1);                //最后一个数据需要回NOACK
        }
        else
        {		
          BH1750_SendACK(0);                //回应ACK
       }
   }

    BH1750_Stop();                          //停止信号
    Delay5ms();
}


//初始化BH1750，根据需要请参考pdf进行修改****
void Init_BH1750()
{
   Single_Write_BH1750(0x01);  

}



//LCD初始化
void InitLcd()
{WriteCommandLCM(0x30);		 //功能设置，设置8位数据接口，
 WriteCommandLCM(0x30);
 WriteCommandLCM(0x30);
 WriteCommandLCM(0x38);	//设置工作方式,8位数据接口，两行显示，5*7点阵
 WriteCommandLCM(0x08);	//关显示，关闭显示，关闭光标，关闭闪烁
 WriteCommandLCM(0x0c); //显示状态设置，开显示，关闭光标与闪烁
 WriteCommandLCM(0x01); //清屏
 WriteCommandLCM(0x06); //输入方式设置 ,AC自动增一，画面不动
}

volatile bit ClearWeighFlag = 0; //传感器调零标志位，清除0漂

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
//****************************************************
//主函数
//****************************************************
void main()
{
/*
	*/
	float temp1;
   delay_nms(100);	    //延时100ms	
   InitLcd();           //初始化LCD
   Init_BH1750();       //初始化BH1750	 
 
  while(1)              //循环
  { 

    Single_Write_BH1750(0x01);   // power on
    Single_Write_BH1750(0x10);   // H- resolution mode

     delay_nms(180);              //延时180ms

     Multiple_read_BH1750();       //连续读出数据，存储在BUF中

    dis_data=BUF[0];
    dis_data=(dis_data<<8)+BUF[1];//合成数据，即光照数据
    
    temp1=(float)dis_data/1.2;

    conversion(temp1);         //计算数据和显示
	DisplayOneChar(0,0,'L'); 
	DisplayOneChar(1,0,'i'); 
	DisplayOneChar(2,0,'g'); 
	DisplayOneChar(3,0,'h'); 
	DisplayOneChar(4,0,'t'); 
    DisplayOneChar(5,0,':'); 

    DisplayOneChar(7,0,wan); //显示数据
    DisplayOneChar(8,0,qian);  
    DisplayOneChar(9,0,bai); 
    DisplayOneChar(10,0,shi); 
	DisplayOneChar(11,0,ge); 

	DisplayOneChar(13,0,'L'); ////显示数单位
	DisplayOneChar(14,0,'X');  
            

	if(bai >= '2')	{

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