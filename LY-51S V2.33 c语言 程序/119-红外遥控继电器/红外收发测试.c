
/*-----------------------------------------------
  名称：遥控器红外解码控制继电器
  论坛：www.doflye.net
  编写：shifang
  日期：2009.5
  修改：无
  内容：按配套遥控器按键，P1.4接继电器控制端J42（RL1）
        红外接收J27接P3.2 按遥控器CH按键，继电器状态翻转
        不同遥控器可修改预定义的值ReversionCode 其他的参数部分自行研究。
------------------------------------------------*/
#include<reg52.h>
////////////////////////////////////////////////
#define TURE 1
#define FALSE 0

#define ShutCode 		0x45
#define ReversionCode   0x46
#define OpenCode        0x47

////////////////////////////////////////////////

sbit IR=P3^2;//红外接口标志
sbit SSR=P1^4;//继电器输出端口
sbit Indicator_light=P1^7;//指示灯输出端口
sbit KeyCon=P3^7;
sbit LED=P1^5;//试验指示灯



unsigned char  irtime;//红外用全局变量
unsigned char  i;
bit flag;
bit irpro_ok,irok;
unsigned char IRcord[4];
unsigned char   irdata[33];

//////////////////////////////////////////////

void Light(void);//声明函数
void Flash(void);
void UnLight(void);
void Keyscan(void);
void Delay(unsigned char mS);
void Ir_work(void);
void Ircordpro(void);



/*------------------------------------------------
                 吸合继电器
------------------------------------------------*/
void Light(void)//继电器吸合，指示灯亮
{
 SSR=1;
 Indicator_light=0;
 }

/*------------------------------------------------
                取反继电器状态
------------------------------------------------*/
void Flash(void)//继电器反相，指示灯反相
{
  SSR=!SSR;
  Indicator_light=!Indicator_light;
 }

/*------------------------------------------------
                释放继电器
------------------------------------------------*/
void UnLight(void)//继电器释放，指示灯灭
{
 SSR=0;
 Indicator_light=1;
 }
/*------------------------------------------------
         按键扫描检测，这里仅作功能测试
------------------------------------------------*/
void Keyscan(void)//键盘扫描
{
/* if(!KeyLeft)//
     {

     Delay(10);//去抖
     if(!KeyLeft)
       {
        Light();
       }
      }
  else if(!KeyRight)//
     {
      Delay(10);//去抖
      if(!KeyRight)
        {
         Flash();
        } 
     }*/
 if(!KeyCon)//按键按下则开关电气
	{
	 Delay(10);//去抖

     if(!KeyCon)
       {
	    while(!KeyCon)
            {};

        Delay(10);//去抖

        while(!KeyCon)
            {};

            {
             //Indicator_light=!Indicator_light;
             Flash();
            }
       }
    }
 }
////////////////////////////////////////////////////////////////
void Delay(unsigned char mS)//delay mS
{
 unsigned char us,usn;

 while(mS!=0)
  { 
   usn = 4;
   while(usn!=0)
    {
     us=0xf0;
     while (us!=0) {us--;};
     usn--;
    }

   mS--;

  }
}


/*------------------------------------------------
                定时器0中断程序
------------------------------------------------*/  

void tim0_isr (void) interrupt 1 using 1//定时器0中断服务函数
{
  irtime++;
}
/*------------------------------------------------
                外部定时器0处理程序
------------------------------------------------*/
void ex0_isr (void) interrupt 0 using 0//外部中断0服务函数
{
  static unsigned char  i;             //接收红外信号处理
  static bit startflag;                //是否开始处理标志位

if(startflag)                         
   {
    if(irtime<63&&irtime>=33)//引导码 TC9012的头码，9ms+4.5ms
                        i=0;
    		irdata[i]=irtime;//存储每个电平的持续时间，用于以后判断是0还是1
    		irtime=0;
    		i++;
   			 if(i==33)
      			{
	  			 irok=1;
				 i=0;
	  			}
          }
  	 else
		{
		irtime=0;
		startflag=1;
		}
}
/*------------------------------------------------
                定时器0初始化
------------------------------------------------*/
void TIM0init(void)//定时器0初始化
{

  TMOD=0x02;//定时器0工作方式2，TH0是重装值，TL0是初值
  TH0=0x00;//reload value
  TL0=0x00;//initial value
  ET0=1;//开中断
  TR0=1;
}
/*------------------------------------------------
                外部定时初始化
------------------------------------------------*/
void EX0init(void)
{
 IT0 = 1;   // Configure interrupt 0 for falling edge on /INT0 (P3.2)
 EX0 = 1;   // Enable EX0 Interrupt
 EA = 1;  
}
/*------------------------------------------------
               主函数
------------------------------------------------*/
void main(void)
{
 SSR=0;//初始化输出端
 Indicator_light=0;
 
 EX0init(); // 初始化
 TIM0init();

 while(1)//主循环
   {
 
    Keyscan();//按键扫描

    if(irok)
	  {   
	   Ircordpro();
 	   irok=0;
	  }

    if(irpro_ok)//处理遥控信号
	  {
	   Ir_work();
  	  }


   }
   
}
/*------------------------------------------------
                红外处理，此值可修改
------------------------------------------------*/

  void Ir_work(void)
  {
    switch(IRcord[2])//仅判断其中1个数据，如果防止串码，需要同时对比4个字节数据
	      {
           //case ShutCode: UnLight(); break;//此处根据不同的遥控器更改相应的值
	       case ReversionCode: Flash(); break;
	       //case OpenCode: Light(); break;
	       default:break;
	      }
		  irpro_ok=0;
	      IRcord[1]=0;//清除值，防止误码
	      IRcord[2]=0;
  }
/*------------------------------------------------
                区分键值
------------------------------------------------*/
void Ircordpro(void)//红外码值处理函数
{ 
  unsigned char i, j, k;
  unsigned char cord,value;

  k=1;
  for(i=0;i<4;i++)//处理4个字节
     {
      for(j=1;j<=8;j++) //处理1个字节8位
         {
          cord=irdata[k];
          if(cord>0x07)//大于某值为1
		    {
             value=value|0x80;
			}
          else 
		    {
             value=value;
			}
          if(j<8)
		    {
			 value=value>>1;
			}
           k++;
         }
     IRcord[i]=value;
     value=0;     
     } irpro_ok=1;//处理完毕标志位置1
}

