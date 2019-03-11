
 /*************************�˲���Ϊ74HC595����������ʹ��SPI��������*************************************/

#include<reg52.h>
#include <intrins.h>
#include <Function.h>

#define  NOP()   _nop_()   /* �����ָ�� */
#define  _Nop()  _nop_()   /*�����ָ��*/


//SPI IO
sbit    MOSIO =P1^5;
sbit    R_CLK =P1^6;
sbit    S_CLK =P1^7;
sbit    IN_PL =P3^4;    //74HC165  shift load   �����ݼ��ص���������
sbit    IN_Dat=P3^5;    //74HC165  output       �����Ƴ�
sbit    OE    =P3^6;



/*********************************************************************************************************
** ��������: HC595SendData
** ��������: ��SPI���߷�������
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
  R_CLK=1; //Ƭѡ
  OE=0;

}

/*********************************************************************************************************
** ��������: ReHC74165
** ��������: 
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
