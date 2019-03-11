#include<reg52.h>

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#define DataPort P0 //定义数据端口 程序中遇到DataPort 则用P0 替换
sbit LATCH1=P2^2;//定义锁存使能端口 段锁存
sbit LATCH2=P2^3;//                 位锁存


extern unsigned char TempData[8]; //存储显示值的全局变量
extern unsigned char code dofly_DuanMa[10];
/*------------------------------------------------
 显示函数，用于动态扫描数码管
 输入参数 FirstBit 表示需要显示的第一位，如赋值2表示从第三个数码管开始显示
 如输入0表示从第一个显示。
 Num表示需要显示的位数，如需要显示99两位数值则该值输入2
------------------------------------------------*/
void Display(unsigned char FirstBit,unsigned char Num);
/*------------------------------------------------
                    定时器初始化子程序
------------------------------------------------*/
void Init_Timer0(void);

#endif
