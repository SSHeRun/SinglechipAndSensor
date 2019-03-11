#ifndef __LCD1602_H__
#define __LCD1602_H__


#include <reg52.h>

//LCD1602 IO����
#define LCD1602_PORT P0
sbit LCD1602_RS = P2^5;
sbit LCD1602_RW = P2^4;
sbit LCD1602_EN = P2^3;

//�������߱�������
extern void LCD1602_delay_ms(unsigned int n);
extern void LCD1602_write_com(unsigned char com);
extern void LCD1602_write_data(unsigned char dat);
extern void LCD1602_write_word(unsigned char *s);
extern void Init_LCD1602();

#endif