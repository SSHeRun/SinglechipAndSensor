#ifndef __DS18B20B_H__
#define __DS18B20B_H__

#include<reg52.h>     //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���
#include<intrins.h>

#define uchar unsigned char
#define uint   unsigned int;
/*------------------------------------------------
                    �˿ڶ���
------------------------------------------------*/
sbit DQ=P1^3;//ds18b20 �˿�
/*------------------------------------------------
                  ��������
------------------------------------------------*/
unsigned int ReadTemperature(void);
bit Init_DS18B20(void);
unsigned char ReadOneChar(void);
void WriteOneChar(unsigned char dat);

#endif
