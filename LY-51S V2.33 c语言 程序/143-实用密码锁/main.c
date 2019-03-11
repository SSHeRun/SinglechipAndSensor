/*-----------------------------------------------
  名称：液晶显示密码锁
  论坛：www.doflye.net
  编写：shifang
  日期：2009.5
  修改：无
  内容：
------------------------------------------------*/
#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
#include<stdio.h>
#include"1602.h"
#include"delay.h"
#include"keyboard.h"


unsigned char code password[]={1,2,3,4,5,6,7,8};//可以更改此密码做多组测试
//定义密码，实际上密码需要存在eeprom中，可以通过程序更改。请用户自行添加

/*------------------------------------------------
                    主程序
------------------------------------------------*/
main()
{
 unsigned char num,i,j;                  
 unsigned char passwordtemp[16];        //最大输入16个
 unsigned char inputtimes;              //密码输入错误次数
 unsigned char passwordlength,PLEN;     //输入密码长度，实际密码长度
 bit Flag;
 
 PLEN=sizeof(password)/sizeof(password[0]);//用于计算出实际密码长度
 
 LCD_Init();         //初始化液晶屏
 DelayMs(10);        //延时用于稳定，可以去掉
 LCD_Clear();        //清屏
 LCD_Write_String(0,0,"  Welcome! ");    //写入第一行信息，主循环中不再更改此信息，所以在while之前写入
 LCD_Write_String(0,1,"Input password!");    //写入第二行信息，提示输入密码
while (1)         //主循环
  {


 num=KeyPro();  //扫描键盘
 if(num!=0xff)  //如果扫描是按键有效值则进行处理
   { 
    if(i==0)    //输入是第一个字符的时候需要把改行清空，方便观看密码
	  LCD_Write_String(0,1,"                ");//清除该行
	if(i<16)
	  {
       passwordtemp[i]=num;
       LCD_Write_Char(i,1,'*');//输入的密码用"*"代替
	   }
	i++;   //输入数值累加
	if((15==num)|| (i==16))//输入按键值15或者密码输入到最大值16，表示输入结束，需要进行比对
	  {
      passwordlength=i-1;  //计算输入密码长度
	  i=0;  //计数器复位
	  if(passwordlength==PLEN)//长度相等则比较，否则直接输出错误
	    {
	     Flag=1;//先把比较位置1
	     for(j=0;j<PLEN;j++)//循环比较8个数值，如果有一个不等 则最终Flag值为0
	        Flag=Flag&&(passwordtemp[j]==password[j]);//比较输入值和已有密码
         }
      if(Flag)//如果比较全部相同，标志位置1
	   {
	    LCD_Write_String(0,1,"                ");//清除该行
        LCD_Write_String(0,1,"Right Open!>>>>");//密码正确显示的信息
        inputtimes=0;//输入正确则次数清零，重新计数
        Flag=0;      //清除正确标志
		}
	  else 
	    {
		LCD_Write_String(0,1,"                ");//清除该行
		LCD_Write_String(0,1,"Wrong! Retry!");//密码错误，提示重新输入
		inputtimes++;//连续输入错误，则次数累加
		if(inputtimes==3)
		  {
		   LCD_Write_String(0,1,"                ");//清除该行
		   LCD_Write_String(0,1,"Wrong 3 times!");//密码错误，提示重新输入
		   while(1);//停止该位置，重启电源后才能输入，实际实用中则需要等到一定时间后才能再次输入。
		  }
		}

	  }
    }	
  }
}

