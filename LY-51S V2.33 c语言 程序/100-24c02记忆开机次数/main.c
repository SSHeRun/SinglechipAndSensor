/*-----------------------------------------------
  ���ƣ�IICЭ�� 24c02�洢��������
  ��̳��www.doflye.net
  ��д��shifang
  �޸ģ���
  ���ݣ�ÿ�ο�����������ֵ��1���洢��24c02
------------------------------------------------*/  
#include <reg52.h>                
#include "i2c.h"
#include "delay.h"
#include "display.h"


main()
{
 unsigned char num=0;
 Init_Timer0();
 IRcvStr(0xae,50,&num,1);   //��24c02��������                             
 num++;                 
 ISendStr(0xae,50,&num,1);  //д��24c02 
 DelayMs(10);
 TempData[0]=dofly_DuanMa[num/100];    
 TempData[1]=dofly_DuanMa[(num%100)/10];
 TempData[2]=dofly_DuanMa[(num%100)%10];

  while(1)
     {
	 }
}
