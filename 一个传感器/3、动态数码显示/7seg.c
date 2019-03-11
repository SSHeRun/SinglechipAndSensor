/*******************************************************************************
*  数码管显示（译码器位选 共阴极）                                             *
********************************************************************************/

#include <reg51.h> 

sbit LS138A = P2^0;  	//定义138译码器的输入A脚由P2.0控制 
sbit LS138B = P2^1;	    //定义138译码器的输入脚B由P2.1控制
sbit LS138C = P2^2; 	//定义138译码器的输入脚C由P2.2控制

void delay(unsigned int i);      //函数声名

char DelayCNT;
 
//此表为 LED 的字模, 共阴数码管   0-9  - 
unsigned char code Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40}; 

/************主函数**********************/ 
main()
{  
   unsigned int i,LedNumVal ;
   unsigned int LedOut[10];
   
   DelayCNT=0;

  while(1)  //进入循环状态
  {	 
     if(++DelayCNT>=50)
	 {
	 	DelayCNT=0;		   //延时计数 每扫描一次加一次
	    ++LedNumVal;       //每隔50个扫描周期加一次
	 }

	 LedOut[0]=Disp_Tab[LedNumVal%10000/1000];
     LedOut[1]=Disp_Tab[LedNumVal%1000/100]|0x80;
     LedOut[2]=Disp_Tab[LedNumVal%100/10];
     LedOut[3]=Disp_Tab[LedNumVal%10];
	 
	 LedOut[4]=Disp_Tab[LedNumVal%10000/1000];	   //千位
     LedOut[5]=Disp_Tab[LedNumVal%1000/100]|0x80;  //百位带小数点
     LedOut[6]=Disp_Tab[LedNumVal%100/10];		   //十位
     LedOut[7]=Disp_Tab[LedNumVal%10];             //个位	 

	
	 for( i=0; i<8; i++)  //实现8位动态扫描循环
	 {	 
	  P0 = LedOut[i];  //将字模送到P0口显示
	  		
	  switch(i)	  //使用switch 语句控制位选  也可以是用查表的方式 学员可以试着自己修改				  
	     {	    
			case 0:LS138A=0; LS138B=0; LS138C=0;  break;         
	        case 1:LS138A=1; LS138B=0; LS138C=0;  break;             	
	        case 2:LS138A=0; LS138B=1; LS138C=0;  break; 
	        case 3:LS138A=1; LS138B=1; LS138C=0;  break; 
			case 4:LS138A=0; LS138B=0; LS138C=1;  break;
			case 5:LS138A=1; LS138B=0; LS138C=1;  break;
			case 6:LS138A=0; LS138B=1; LS138C=1;  break;
			case 7:LS138A=1; LS138B=1; LS138C=1;  break;
		  
	     }
		 
		delay(150);
	  }
	 
  }   
}	

/***************************************************************************
*																		   *
*		                    延时程序						               *
****************************************************************************/
void delay(unsigned int i)
{
    char j;
    for(i; i > 0; i--)
        for(j = 200; j > 0; j--);
}


