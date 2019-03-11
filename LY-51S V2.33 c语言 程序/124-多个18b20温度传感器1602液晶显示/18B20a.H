#ifndef __DS18B20A_H__
#define __DS18B20A_H__

#include<reg52.h>     //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
#include<intrins.h>

#define uchar unsigned char
#define uint   unsigned int;
/*------------------------------------------------
                    端口定义
------------------------------------------------*/
sbit DQ1=P1^4;//ds18b20 端口
/*------------------------------------------------
                  函数声明
------------------------------------------------*/
unsigned int ReadTemperature_a(void);
bit Init_DS18B20_a(void);
unsigned char ReadOneChar_a(void);
void WriteOneChar_a(unsigned char dat);

#endif
