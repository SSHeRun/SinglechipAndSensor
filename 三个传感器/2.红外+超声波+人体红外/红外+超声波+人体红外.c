/********************************************************************************
*          lcd1602显示 遥控键值读取器                                          *
*         lcd1602显示 遥控器接p3.2                                             *
********************************************************************************/

#include <reg51.h>
#include <intrins.h>
#include   <STDIO.H>

#define uchar unsigned char
#define uint  unsigned int
sbit RX=P1^1;
sbit  TX=P1^2;

sbit IRIN = P3^2;         //红外接收器数据线

uchar IRCOM[7];

sbit E=P2^3;
sbit RW=P2^4;
sbit RS=P2^5;

#define LCM_Data  P0

uchar code  cdis1[ ] = {"  Red Control  "};
uchar code  cdis2[ ] = {"  IR-CODE:--H  "};

unsigned int  time=0;
unsigned int  timer=0;
float         S=0;
bit           flag =0;

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
/********************************************************/

main()
{
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
	EX0=1;
	//IE = 0x81;                 //允许总中断中断,使能 INT0 外部中断
	//TCON = 0x01;               //触发方式为脉冲负边沿触发

   while(1)
	{
	 StartModule();
	 while(!RX);		//当RX为零时等待
	 TR0=1;			    //开启计数
	 while(RX);			//当RX为1计数并等待
	 TR0=0;				//关闭计数
     Conut();			//计算
	 Delay(100000);		
	}

} //end main

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

           
     DisplayOneChar(10,1,IRCOM[6]);          //第一位数显示         
     DisplayOneChar(11,1,IRCOM[5]);        //第二位数显示

     EX0 = 1; 
} 



