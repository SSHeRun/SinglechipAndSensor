#ifndef __HX711_H__
#define __HX711_H__


#include <reg52.h>
#include <intrins.h>

//IO设置
sbit HX711_DOUT=P1^2; 
sbit HX711_SCK=P1^3; 

//函数或者变量声明
extern void Delay__hx711_us(void);
extern unsigned long HX711_Read(void);

#endif