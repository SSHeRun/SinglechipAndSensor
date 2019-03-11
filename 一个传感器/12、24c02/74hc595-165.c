
 /*************************此部分为74HC595的驱动程序使用SPI总线连接*************************************/

#include<reg52.h>
#include <intrins.h>
#include <Function.h>

#define  NOP()   _nop_()   /* 定义空指令 */
#define  _Nop()  _nop_()   /*定义空指令*/


//SPI IO
sbit    MOSIO =P1^5;
sbit    R_CLK =P1^6;
sbit    S_CLK =P1^7;
sbit    IN_PL =P3^4;    //74HC165  shift load   把数据加载到锁存器中
sbit    IN_Dat=P3^5;    //74HC165  output       数据移出
sbit    OE    =P3^6;



/*********************************************************************************************************
** 函数名称: HC595SendData
** 功能描述: 向SPI总线发送数据
*********************************************************************************************************/
void HC595SendData(unsigned int SendVal)
{  
  unsigned char i;
		
  for(i=0;i<16;i++) 
   {
	if((SendVal<<i)&0x8000) MOSIO=1; // set dataline high
	else MOSIO=0;
 
	S_CLK=0;
	NOP();
	NOP();
	S_CLK=1;	
   }
   
	
  R_CLK=0; //set dataline low
  NOP();
  NOP();
  R_CLK=1; //片选
  OE=0;

}

/*********************************************************************************************************
** 函数名称: ReHC74165
** 功能描述: 
*********************************************************************************************************/
unsigned long ReHC74165(void)
{  
  unsigned char i;
  unsigned long indata;
		
   IN_PL=0;
   NOP();
   IN_PL=1;
   NOP();            
   
   indata=0;     
   for(i=0; i<25; i++)
    { 
	  indata=indata<<1;
      if(IN_Dat==1)indata=indata+1;
	  S_CLK=1;
      NOP();
      S_CLK=0;	  
	} 
   
   return(~indata);
 
}
