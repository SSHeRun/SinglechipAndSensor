/*************************************************************************************
*          步进电机试验															 *																 *
*    通过本例程了解步进马达使用及驱动程序编写										 *
*    单双八拍工作方式：                                                           	 *
*    A-AB-B-BC-C-CD-D-DA (即一个脉冲,转 3.75 度)      								 *               	                          									 *
**************************************************************************************/  

#include "reg52.h"
//步进电机驱动引脚定义
sbit A = P1^0;	  
sbit b = P1^1;
sbit C = P1^2;
sbit D = P1^3;

 ///////////////////////////////////////
//步进电机驱动
unsigned char MotorStep=0;
unsigned int  MotorTimer = 0;
unsigned int  MotorDelay,Speed=1,TIM,CT;

#define speed 5   // 调整速度 数值不要设的太低 低了会引起震动。

/*****************************************************
*	    初始化马达							         *
******************************************************/
void InitMotor()
{
  A = 1;
  b = 1;
  C = 1;
  D = 1;
}


 
void SetMotor()
{
  //  if(Speed == 0) return;

    MotorDelay=Speed;	//调整速度
    switch(MotorStep)
    {
       case 0:
          if(TIM)   // A
          {				    
            A = 0;		    //0xf1  
            b = 1;
			C = 1;
            D = 1;
            MotorStep = 1;
			TIM=0;
          }
       break;

       case 1:		 // AB
         if(TIM)
          {	
            A = 0;		   //0xf3 
            b = 0;
           	C = 1;
            D = 1;
            MotorStep = 2;
			TIM=0;
          }
       break;

       case 2:	   //B
         if(TIM)
          {
           
          	A = 1;
            b = 0;		  //0xf2 
            C = 1;
            D = 1;
           
            MotorStep = 3;
			TIM=0;
          }
       break;


       case 3:		//BC
         if(TIM)
          {	
            A = 1;
            b = 0;		    //0xf6
            C = 0;
            D = 1;
            MotorStep = 4;
			TIM=0;
          }
       break;
     case 4:		 //C
         if(TIM)
          {	
            A = 1;
            b = 1;		    //0xf4
            C = 0;
            D = 1;
            MotorStep = 5;
			TIM=0;
           }
       break;
	case 5:			  //CD
         if(TIM)
          {	
            A = 1;
            b = 1;		    //0xfc
            C = 0;
            D = 0;
            MotorStep = 6;
			TIM=0;
          }
       break;
	case 6:			  //D
         if(TIM)
          {	
            A = 1;
            b = 1;		    //0xf8
            C = 1;
            D = 0;
            MotorStep = 7;
			TIM=0;
          }
       break;
	case 7:			//DA
         if(TIM)
          {	
            A = 0;
            b = 1;		    //0xf9
            C = 1;
            D = 0;
            MotorStep = 0;
			TIM=0;
          }
       break;
	
	}

}


void system_Ini()
{
    TMOD|= 0x11;
    TH0=0xDC; //11.0592M
    TL0=0x00;	
	IE = 0x8A;	
    TR0  = 1;


}

main() 
 { system_Ini();
   InitMotor();
    while(1)
	{
	 SetMotor();
	 }

  }

/************************************************
*												*
*	      定时中断延时						    *
************************************************/
  void Tzd(void) interrupt 1
{
   TH0 = 0xfe;    //11.0592
	 TL0 = 0x33;

	if( CT++==speed) 
	  {TIM=1;
	   CT=0;
	  }
 }
