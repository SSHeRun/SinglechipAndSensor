/********************************************************************
*该示例实现了用温度传感器件DS18B20对温度的采集，并在数码管上显示出来。
***********************************************************************/
#include<reg52.h>
#define uchar unsigned char
#define uint  unsigned int

#define jump_ROM 0xCC
#define start 0x44
#define read_EEROM 0xBE

sbit DQ = P3^5;            //DS18B20数据口

unsigned char TMPH,TMPL; 

uchar code table[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

sbit IRIN = P3^2;         //红外接收器数据线

uchar IRCOM[7];

sbit E=P2^3;
sbit RW=P2^4;
sbit RS=P2^5;

#define LCM_Data  P0

uchar code  cdis1[ ] = {"  Red Control  "};
uchar code  cdis2[ ] = {"  IR-CODE:--H  "};
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

//延时设置
void Delay1(unsigned int t)   // delay 40us
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
   Delay1(1);
   E=0;
   Delay1(100);  //delay 40us
}
//写数据
void WriteDataLCM(unsigned char ch)
{  RS=1;
   RW=0;
   E=1;
   LCM_Data=ch;
   Delay1(1);
   E=0;
   Delay1(100); //delay 40us
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




/********************************************************************
* 名称 : Main()
* 功能 : 主函数
* 输入 : 无
* 输出 : 无
***********************************************************************/
void main()
{
/*

	*/
 uchar m;
    
    IRIN=1;                    //I/O口初始化

    Delay1(10);                 //延时
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

} //end main

/**********************************************************/
void IR_IN() interrupt 0 using 0
{
  unsigned char j,k,N=0;
     EX0 = 0;   
	 Delay1(200);
	 if (IRIN==1) 
     { EX0 =1;
	   return;
	  } 
                           //确认IR信号出现
  while (!IRIN)            //等IR变为高电平，跳过9ms的前导低电平信号。
    {Delay1(13);}

 for (j=0;j<4;j++)         //收集四组数据
 { 
  for (k=0;k<8;k++)        //每组数据有8位
  {
   while (IRIN)            //等 IR 变为低电平，跳过4.5ms的前导高电平信号。
     {Delay1(13);}
    while (!IRIN)          //等 IR 变为高电平
     {Delay1(13);}
     while (IRIN)           //计算IR高电平时长
      {
   Delay1(13);
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

     
     DisplayOneChar(10,1,IRCOM[6]);          //第一位数显示         
     DisplayOneChar(11,1,IRCOM[5]);        //第二位数显示

     EX0 = 1; 

	 if(IRCOM[6] == '0' && IRCOM[5] == '8'){
	 	    	//float tt;
	uint temp;
	P2 = 0x00;
	while(1)
	{
		Reset();
		write_byte(jump_ROM);
		write_byte(start);
		Reset();
		write_byte(jump_ROM);
		write_byte(read_EEROM);
		TMPL = read_byte();
		TMPH = read_byte();
		temp = TMPL / 16 + TMPH * 16;
		P0 = table[temp/10%10];
		P2 = 6;
		Delay_1ms(5);
		P0 = table[temp%10];
		P2 = 7;
		Delay_1ms(5);
	}
	}     
} 