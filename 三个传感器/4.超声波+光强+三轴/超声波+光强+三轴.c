#include <reg51.h>
#include  <math.h>    //Keil library  
#include  <stdio.h>   //Keil library	
#include  <INTRINS.H>
#include   <STDIO.H>

sbit	  S_SCL=P1^3;      //数字光强IIC时钟引脚定义
sbit  	  S_SDA=P1^4;      //数字光强IIC数据引脚定义

#define   uchar unsigned char
#define   uint unsigned int	
#define	  S_SlaveAddress   0x46 //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
                              //ALT  ADDRESS引脚接地时地址为0x46，接电源时地址为0xB8

char    S_BUF[8];                         //接收数据缓存区   

sbit  RX=P1^1;
sbit  TX=P1^2;
unsigned int  time=0;
unsigned int  timer=0;
float         S=0;
bit           flag =0;

#define   DataPort P0    //LCD1602数据端口

sbit	  Z_SCL=P2^1;      //三轴加速度IIC时钟引脚定义
sbit 	  Z_SDA=P2^0;      //三轴加速度IIC数据引脚定义

sbit      LCM_RS=P2^5;   //LCD1602命令端口		
sbit      LCM_RW=P2^4;   //LCD1602命令端口		
sbit      LCM_EN=P2^3;   //LCD1602命令端口
#define	Z_SlaveAddress   0xA6	  //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
                              //ALT  ADDRESS引脚接地时地址为0xA6，接电源时地址为0x3A
typedef unsigned char  BYTE;
typedef unsigned short WORD;

BYTE Z_BUF[8];                         //接收数据缓存区      	
uchar ge,shi,bai,qian,wan;           //显示变量
int  dis_data;                       //变量

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

/*******************************/
void delay(unsigned int k)	
{						
unsigned int i,j;				
	for(i=0;i<k;i++)
	{			
	for(j=0;j<121;j++)			
	{;}}						
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

/*******************************/
void WriteCommandLCM(uchar CMD)
{					
	LCM_RS=0;
	LCM_RW=0;
	DataPort=CMD;
	LCM_EN=1;
	LCM_EN=0;
	delay(1);
}					
/*******************************/
void WriteDataLCM(uchar dataW)
{					
	LCM_RS=1;
	LCM_RW=0;
	DataPort=dataW;
	LCM_EN=1;
	LCM_EN=0;
	delay(1);
}		
/***********************************/
void InitLcd()				
{			
	WriteCommandLCM(0x30);	
	WriteCommandLCM(0x30);	
	WriteCommandLCM(0x38);	
	WriteCommandLCM(0x08);	
	WriteCommandLCM(0x0c);
	WriteCommandLCM(0x01);	
	WriteCommandLCM(0x06);	

}			
/***********************************/
void DisplayOneChar(uchar X,uchar Y,uchar DData)
{						
	Y&=1;						
	X&=15;						
	if(Y)X|=0x40;					
	X|=0x80;			
	WriteCommandLCM(X);		
	WriteDataLCM(DData);		
}	


/**************************************
起始信号
**************************************/
void BH1750_Start()
{
    S_SDA = 1;                    //拉高数据线
    S_SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    S_SDA = 0;                    //产生下降沿
    Delay5us();                 //延时
    S_SCL = 0;                    //拉低时钟线
}

/**************************************
停止信号
**************************************/
void BH1750_Stop()
{
    S_SDA = 0;                    //拉低数据线
    S_SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    S_SDA = 1;                    //产生上升沿
    Delay5us();                 //延时
}

/**************************************
发送应答信号
入口参数:ack (0:ACK 1:NAK)
**************************************/
void BH1750_SendACK(bit ack)
{
    S_SDA = ack;                  //写应答信号
    S_SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    S_SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时
}

/**************************************
接收应答信号
**************************************/
bit BH1750_RecvACK()
{
    S_SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    CY = S_SDA;                   //读应答信号
    S_SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时

    return CY;
}

/**************************************
向IIC总线发送一个字节数据
**************************************/
void BH1750_SendByte(BYTE dat)
{
    char i;

    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;              //移出数据的最高位
        S_SDA = CY;               //送数据口
        S_SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        S_SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    BH1750_RecvACK();
}

/**************************************
从IIC总线接收一个字节数据
**************************************/
BYTE BH1750_RecvByte()
{
    char i;
    char dat = 0;

    S_SDA = 1;                    //使能内部上拉,准备读取数据,
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;
        S_SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        dat |= S_SDA;             //读数据               
        S_SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    return dat;
}

//*********************************

void Single_Write_BH1750(uchar REG_Address)
{
    BH1750_Start();                  //起始信号
    BH1750_SendByte(S_SlaveAddress);   //发送设备地址+写信号
    BH1750_SendByte(REG_Address);    //内部寄存器地址，
  //  BH1750_SendByte(REG_data);       //内部寄存器数据，
    BH1750_Stop();                   //发送停止信号
}

//*********************************************************
//
//连续读出BH1750内部数据
//
//*********************************************************
void Multiple_read_BH1750(void)
{   uchar i;	
    BH1750_Start();                          //起始信号
    BH1750_SendByte(S_SlaveAddress+1);         //发送设备地址+读信号
	
	 for (i=0; i<3; i++)                      //连续读取2个地址数据，存储中BUF
    {
        S_BUF[i] = BH1750_RecvByte();          //S_BUF[0]存储0x32地址中的数据
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

/********************************************************/
void Conut(void)
{
 time=TH0*256+TL0;
 TH0=0;
 TL0=0;
 S=(time*1.87)/100;     //算出来是CM
 if(flag==1)		    //超出测量
 {
  flag=0;
  printf("-----\n"); 
 }

  printf("S=%f\n",S); 
}
/********************************************************/
void zd0() interrupt 1 		 //T0中断用来计数器溢出,超过测距范围
{
  flag=1;							 //中断溢出标志
}
/********************************************************/
void  StartModule() 		         //T1中断用来扫描数码管和计800MS启动模块
{
	TX=1;			                 //800MS  启动一次模块
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_();
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_();
	TX=0;
}


/**************************************
起始信号
**************************************/
void ADXL345_Start()
{
    Z_SDA = 1;                    //拉高数据线
    Z_SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    Z_SDA = 0;                    //产生下降沿
    Delay5us();                 //延时
    Z_SCL = 0;                    //拉低时钟线
}

/**************************************
停止信号
**************************************/
void ADXL345_Stop()
{
    Z_SDA = 0;                    //拉低数据线
    Z_SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    Z_SDA = 1;                    //产生上升沿
    Delay5us();                 //延时
}

/**************************************
发送应答信号
入口参数:ack (0:ACK 1:NAK)
**************************************/
void ADXL345_SendACK(bit ack)
{
    Z_SDA = ack;                  //写应答信号
    Z_SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    Z_SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时
}

/**************************************
接收应答信号
**************************************/
bit ADXL345_RecvACK()
{
    Z_SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    CY = Z_SDA;                   //读应答信号
    Z_SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时

    return CY;
}

/**************************************
向IIC总线发送一个字节数据
**************************************/
void ADXL345_SendByte(BYTE dat)
{
    char i;

    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;              //移出数据的最高位
        Z_SDA = CY;               //送数据口
        Z_SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        Z_SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    ADXL345_RecvACK();
}

/**************************************
从IIC总线接收一个字节数据
**************************************/
BYTE ADXL345_RecvByte()
{
    char i;
    char dat = 0;

    Z_SDA = 1;                    //使能内部上拉,准备读取数据,
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;
        Z_SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        dat |= Z_SDA;             //读数据               
        Z_SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    return dat;
}

//******单字节写入*******************************************

void Single_Write_ADXL345(uchar REG_Address,uchar REG_data)
{
    ADXL345_Start();                  //起始信号
    ADXL345_SendByte(Z_SlaveAddress);   //发送设备地址+写信号
    ADXL345_SendByte(REG_Address);    //内部寄存器地址，请参考中文pdf22页 
    ADXL345_SendByte(REG_data);       //内部寄存器数据，请参考中文pdf22页 
    ADXL345_Stop();                   //发送停止信号
}

//********单字节读取*****************************************
uchar Single_Read_ADXL345(uchar REG_Address)
{  uchar REG_data;
    ADXL345_Start();                          //起始信号
    ADXL345_SendByte(Z_SlaveAddress);           //发送设备地址+写信号
    ADXL345_SendByte(REG_Address);            //发送存储单元地址，从0开始	
    ADXL345_Start();                          //起始信号
    ADXL345_SendByte(Z_SlaveAddress+1);         //发送设备地址+读信号
    REG_data=ADXL345_RecvByte();              //读出寄存器数据
	ADXL345_SendACK(1);   
	ADXL345_Stop();                           //停止信号
    return REG_data; 
}
//*********************************************************
//
//连续读出ADXL345内部加速度数据，地址范围0x32~0x37
//
//*********************************************************
void Multiple_read_ADXL345(void)
{   uchar i;
    ADXL345_Start();                          //起始信号
    ADXL345_SendByte(Z_SlaveAddress);           //发送设备地址+写信号
    ADXL345_SendByte(0x32);                   //发送存储单元地址，从0x32开始	
    ADXL345_Start();                          //起始信号
    ADXL345_SendByte(Z_SlaveAddress+1);         //发送设备地址+读信号
	 for (i=0; i<6; i++)                      //连续读取6个地址数据，存储中BUF
    {
        Z_BUF[i] = ADXL345_RecvByte();          //BUF[0]存储0x32地址中的数据
        if (i == 5)
        {
           ADXL345_SendACK(1);                //最后一个数据需要回NOACK
        }
        else
        {
          ADXL345_SendACK(0);                //回应ACK
       }
   }
    ADXL345_Stop();                          //停止信号
    Delay5ms();
}


//*****************************************************************

//初始化ADXL345，根据需要请参考pdf进行修改************************
void Init_ADXL345()
{
   Single_Write_ADXL345(0x31,0x0B);   //测量范围,正负16g，13位模式
   Single_Write_ADXL345(0x2C,0x08);   //速率设定为12.5 参考pdf13页
   Single_Write_ADXL345(0x2D,0x08);   //选择电源模式   参考pdf24页
   Single_Write_ADXL345(0x2E,0x80);   //使能 DATA_READY 中断
   Single_Write_ADXL345(0x1E,0x00);   //X 偏移量 根据测试传感器的状态写入pdf29页
   Single_Write_ADXL345(0x1F,0x00);   //Y 偏移量 根据测试传感器的状态写入pdf29页
   Single_Write_ADXL345(0x20,0x05);   //Z 偏移量 根据测试传感器的状态写入pdf29页
}
//***********************************************************************
//显示x轴
void display_x()
{   float temp;
    dis_data=(Z_BUF[1]<<8)+Z_BUF[0];  //合成数据   
	if(dis_data<0){
	dis_data=-dis_data;
    DisplayOneChar(10,0,'-');      //显示正负符号位
	}
	else DisplayOneChar(10,0,' '); //显示空格

    temp=(float)dis_data*3.9;  //计算数据和显示,查考ADXL345快速入门第4页
    conversion(temp);          //转换出显示需要的数据
	DisplayOneChar(8,0,'X');
    DisplayOneChar(9,0,':'); 
    DisplayOneChar(11,0,qian); 
	DisplayOneChar(12,0,'.'); 
    DisplayOneChar(13,0,bai); 
    DisplayOneChar(14,0,shi); 
	DisplayOneChar(15,0,' '); 
}

//***********************************************************************
//显示y轴
void display_y()
{   
	float temp;
    dis_data=(Z_BUF[3]<<8)+Z_BUF[2];  //合成数据   
	if(dis_data<0){
	dis_data=-dis_data;
    DisplayOneChar(2,1,'-');      //显示正负符号位
	}
	else DisplayOneChar(2,1,' '); //显示空格

    temp=(float)dis_data*3.9;  //计算数据和显示,查考ADXL345快速入门第4页
    conversion(temp);          //转换出显示需要的数据
	DisplayOneChar(0,1,'Y');   //第1行，第0列 显示y
    DisplayOneChar(1,1,':'); 
    DisplayOneChar(3,1,qian); 
	DisplayOneChar(4,1,'.'); 
    DisplayOneChar(5,1,bai); 
    DisplayOneChar(6,1,shi);  
	DisplayOneChar(7,1,' ');  
}

//***********************************************************************
//显示z轴
void display_z()
{
    float temp;
    dis_data=(Z_BUF[5]<<8)+Z_BUF[4];    //合成数据   
	if(dis_data<0){
	dis_data=-dis_data;
    DisplayOneChar(10,1,'-');       //显示负符号位
	}
	else DisplayOneChar(10,1,' ');  //显示空格

    temp=(float)dis_data*3.9;  //计算数据和显示,查考ADXL345快速入门第4页
    conversion(temp);          //转换出显示需要的数据
	DisplayOneChar(8,1,'Z');  //第0行，第10列 显示Z
    DisplayOneChar(9,1,':'); 
    DisplayOneChar(11,1,qian); 
	DisplayOneChar(12,1,'.'); 
    DisplayOneChar(13,1,bai); 
    DisplayOneChar(14,1,shi); 
	DisplayOneChar(15,1,' ');  
}


//=============================================
main()
{
	float temp;
	uchar devid;
	delay(500);	                   	//上电延时

	TMOD=0x21;		   //设T0为方式1，GATE=1；
	SCON=0x50;
	TH1=0xFD;
	TL1=0xFD;
	TH0=0;
	TL0=0; 
	TR0=1;  
	ET0=1;             //允许T0中断
	TR1=1;			   //开启定时器
	TI=1;

	EA=1;			   //开启总中断
   		
	InitLcd();                      //液晶初始化ADXL345
    Init_BH1750();       //初始化BH1750
 
	DisplayOneChar(0,0,'A');
	DisplayOneChar(1,0,'D'); 
	DisplayOneChar(2,0,'X'); 
	DisplayOneChar(3,0,'L'); 
	DisplayOneChar(4,0,'3'); 
	DisplayOneChar(5,0,'4');  
	DisplayOneChar(6,0,'5'); 

 	while(1)              //循环
 	{ 

	    Single_Write_BH1750(0x01);   // power on
	    Single_Write_BH1750(0x10);   // H- resolution mode
	
	    delay_nms(180);              //延时180ms
	
	    Multiple_read_BH1750();       //连续读出数据，存储在S_BUF中
	
	    dis_data=S_BUF[0];
	    dis_data=(dis_data<<8)+S_BUF[1];//合成数据，即光照数据
	    
	    temp=(float)dis_data/1.2;
	
	    conversion(temp);         //计算数据和显示
		printf("Light: %c%c%c%c%c     ",wan,qian,bai,shi,ge);
		StartModule();
		while(!RX);		//当RX为零时等待
		TR0=1;			    //开启计数
		while(RX);			//当RX为1计数并等待
		TR0=0;				//关闭计数
	    Conut();			//计算
		delay_nms(100);		//100MS  
		 
		DisplayOneChar(0,0,'A');
		DisplayOneChar(1,0,'D'); 
		DisplayOneChar(2,0,'X'); 
		DisplayOneChar(3,0,'L'); 
		DisplayOneChar(4,0,'3'); 
		DisplayOneChar(5,0,'4');  
		DisplayOneChar(6,0,'5'); 
			
		Init_ADXL345();                 	//初始化ADXL345
		devid=Single_Read_ADXL345(0X00);	//读出的数据为0XE5,表示正确
		if(devid!=0XE5)
		{			
			DisplayOneChar(8,0,'N');
			DisplayOneChar(9,0,'o');		
			DisplayOneChar(10,0,' ');		
			DisplayOneChar(11,0,'F');
			DisplayOneChar(12,0,'i');
			DisplayOneChar(13,0,'n');
			DisplayOneChar(14,0,'d');		
			DisplayOneChar(15,0,'!');		
			//
			DisplayOneChar(0,1,' ');   //第1行，第0列 显示y
			DisplayOneChar(1,1,' '); 
			DisplayOneChar(2,1,' '); 	
			DisplayOneChar(3,1,' '); 
			DisplayOneChar(4,1,' '); 
			DisplayOneChar(5,1,' '); 
			DisplayOneChar(6,1,' ');  
			DisplayOneChar(7,1,' ');  
			DisplayOneChar(8,1,' ');  //第0行，第10列 显示Z
			DisplayOneChar(9,1,' '); 
			DisplayOneChar(11,1,' '); 
			DisplayOneChar(12,1,' '); 
			DisplayOneChar(13,1,' '); 
			DisplayOneChar(14,1,' '); 
			DisplayOneChar(15,1,' ');  			
				
		}
		else
		{			
			Multiple_read_ADXL345();       	//连续读出数据，存储在BUF中
			display_x();                   	//---------显示X轴
			display_y();                   	//---------显示Y轴
			display_z();                   	//---------显示Z轴			    
		}      
		delay(350);                    	//延时  	 
		       
	  }
}
