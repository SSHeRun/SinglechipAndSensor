
#ifndef __DS1302_H__
#define __DS1302_H__

#include <reg52.h>
#include<intrins.h>

sbit SCK=P1^4;		
sbit SDA=P1^5;		
sbit RST=P1^6;
//复位脚
#define RST_CLR	RST=0//电平置低
#define RST_SET	RST=1//电平置高


//双向数据
#define IO_CLR	SDA=0//电平置低
#define IO_SET	SDA=1//电平置高
#define IO_R	SDA  //电平读取


//时钟信号
#define SCK_CLR	SCK=0//时钟信号
#define SCK_SET	SCK=1//电平置高


#define ds1302_sec_add			0x80		//秒数据地址
#define ds1302_min_add			0x82		//分数据地址
#define ds1302_hr_add			0x84		//时数据地址
#define ds1302_date_add			0x86		//日数据地址
#define ds1302_month_add		0x88		//月数据地址
#define ds1302_day_add			0x8a		//星期数据地址
#define ds1302_year_add			0x8c		//年数据地址
#define ds1302_control_add		0x8e		//控制数据地址
#define ds1302_charger_add		0x90 					 
#define ds1302_clkburst_add		0xbe

extern unsigned char time_buf1[8];//空年月日时分秒周
extern unsigned char time_buf[8] ;//空年月日时分秒周
/*------------------------------------------------
           向DS1302写入一字节数据
------------------------------------------------*/
void Ds1302_Write_Byte(unsigned char addr, unsigned char d);
/*------------------------------------------------
           从DS1302读出一字节数据
------------------------------------------------*/
unsigned char Ds1302_Read_Byte(unsigned char addr) ;
/*------------------------------------------------
           向DS1302写入时钟数据
------------------------------------------------*/
void Ds1302_Write_Time(void) ;
/*------------------------------------------------
           从DS1302读出时钟数据
------------------------------------------------*/
void Ds1302_Read_Time(void)  ;
/*------------------------------------------------
                DS1302初始化
------------------------------------------------*/
void Ds1302_Init(void);

#endif
