//实验名称：字符型LCD接口实验 
#include <reg51.h>
#include  <math.h>    //Keil library  
#include  <stdio.h>   //Keil library	
#include  <INTRINS.H>

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


sbit led1=P1^5;

void delay2(uint xms){
uint i,j;
	for(i=xms;i>0;i--)
		for(j=110;j>0;j--);
}
void externalInterruption() interrupt 0
{
	  led1 = ~led1;
		delay2(100);
}




//延时设置
void Delay(unsigned int t)   // delay 40us
{
 for(;t!=0;t--) ;
}

//写指令
void WriteCommandLCM1(unsigned char ch)
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
void WriteDataLCM1(unsigned char ch)
{  RS=1;
   RW=0;
   E=1;
   LCM_Data=ch;
   Delay(1);
   E=0;
   Delay(100); //delay 40us
}

void DisplayOneChar1(uchar X,uchar Y,uchar DData)
{						
	Y&=1;						
	X&=15;						
	if(Y)X|=0x40;					
	X|=0x80;			
	WriteCommandLCM1(X);		
	WriteDataLCM1(DData);		
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
void delay_nms1(unsigned int k)	
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
void Delay5us1()
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
void Delay5ms1()
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
    Delay5us1();                 //延时
    SDA = 0;                    //产生下降沿
    Delay5us1();                 //延时
    SCL = 0;                    //拉低时钟线
}

/**************************************
停止信号
**************************************/
void BH1750_Stop()
{
    SDA = 0;                    //拉低数据线
    SCL = 1;                    //拉高时钟线
    Delay5us1();                 //延时
    SDA = 1;                    //产生上升沿
    Delay5us1();                 //延时
}

/**************************************
发送应答信号
入口参数:ack (0:ACK 1:NAK)
**************************************/
void BH1750_SendACK(bit ack)
{
    SDA = ack;                  //写应答信号
    SCL = 1;                    //拉高时钟线
    Delay5us1();                 //延时
    SCL = 0;                    //拉低时钟线
    Delay5us1();                 //延时
}

/**************************************
接收应答信号
**************************************/
bit BH1750_RecvACK()
{
    SCL = 1;                    //拉高时钟线
    Delay5us1();                 //延时
    CY = SDA;                   //读应答信号
    SCL = 0;                    //拉低时钟线
    Delay5us1();                 //延时

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
        Delay5us1();             //延时
        SCL = 0;                //拉低时钟线
        Delay5us1();             //延时
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
        Delay5us1();             //延时
        dat |= SDA;             //读数据               
        SCL = 0;                //拉低时钟线
        Delay5us1();             //延时
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
    Delay5ms1();
}


//初始化BH1750，根据需要请参考pdf进行修改****
void Init_BH1750()
{
   Single_Write_BH1750(0x01);  

}



//LCD初始化
void InitLcd1()
{WriteCommandLCM1(0x30);		 //功能设置，设置8位数据接口，
 WriteCommandLCM1(0x30);
 WriteCommandLCM1(0x30);
 WriteCommandLCM1(0x38);	//设置工作方式,8位数据接口，两行显示，5*7点阵
 WriteCommandLCM1(0x08);	//关显示，关闭显示，关闭光标，关闭闪烁
 WriteCommandLCM1(0x0c); //显示状态设置，开显示，关闭光标与闪烁
 WriteCommandLCM1(0x01); //清屏
 WriteCommandLCM1(0x06); //输入方式设置 ,AC自动增一，画面不动
}



//=============================================
main()
{
   float temp1;
   delay_nms1(100);	    //延时100ms	
   InitLcd1();           //初始化LCD
   Init_BH1750();       //初始化BH1750	 
    led1=0; //灯灭
	 IT0=0;//低电平有效
	 EA=1;	
	 EX0=1;

 
  while(1)              //循环
  { 

    Single_Write_BH1750(0x01);   // power on
    Single_Write_BH1750(0x10);   // H- resolution mode

     delay_nms1(180);              //延时180ms

     Multiple_read_BH1750();       //连续读出数据，存储在BUF中

    dis_data=BUF[0];
    dis_data=(dis_data<<8)+BUF[1];//合成数据，即光照数据
    
    temp1=(float)dis_data/1.2;

    conversion(temp1);         //计算数据和显示
	DisplayOneChar1(0,0,'L'); 
	DisplayOneChar1(1,0,'i'); 
	DisplayOneChar1(2,0,'g'); 
	DisplayOneChar1(3,0,'h'); 
	DisplayOneChar1(4,0,'t'); 
    DisplayOneChar1(5,0,':'); 

    DisplayOneChar1(7,0,wan); //显示数据
    DisplayOneChar1(8,0,qian);  
    DisplayOneChar1(9,0,bai); 
    DisplayOneChar1(10,0,shi); 
	DisplayOneChar1(11,0,ge); 

	DisplayOneChar1(13,0,'L'); ////显示数单位
	DisplayOneChar1(14,0,'X');  
	if(led1==1)
			led1=0;
            
  }
}
