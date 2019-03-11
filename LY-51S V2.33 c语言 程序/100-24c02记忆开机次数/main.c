/*-----------------------------------------------
  名称：IIC协议 24c02存储开机次数
  论坛：www.doflye.net
  编写：shifang
  修改：无
  内容：每次开机，开机数值加1并存储到24c02
------------------------------------------------*/  
#include <reg52.h>                
#include "i2c.h"
#include "delay.h"
#include "display.h"


main()
{
 unsigned char num=0;
 Init_Timer0();
 IRcvStr(0xae,50,&num,1);   //从24c02读出数据                             
 num++;                 
 ISendStr(0xae,50,&num,1);  //写入24c02 
 DelayMs(10);
 TempData[0]=dofly_DuanMa[num/100];    
 TempData[1]=dofly_DuanMa[(num%100)/10];
 TempData[2]=dofly_DuanMa[(num%100)%10];

  while(1)
     {
	 }
}
