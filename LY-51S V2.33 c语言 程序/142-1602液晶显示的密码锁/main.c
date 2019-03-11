/*-----------------------------------------------
  名称：液晶显示密码锁
  论坛：www.doflye.net
  编写：shifang
  日期：2009.5
  修改：无
  内容：第一行显示固定密码，第二行显示输入密码，密码输入完毕后会有正确或者错误提示,
        为了演示方便，提示密码，输入密码部分也没有用星号代替。可以自行更换成星号
------------------------------------------------*/
#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
#include<stdio.h>
#include"1602.h"
#include"delay.h"
#include"keyboard.h"


unsigned char code password[8]={1,2,3,4,5,6,7,8};//可以更改此密码做多组测试

/*------------------------------------------------
                    主程序
------------------------------------------------*/
main()
{
 unsigned char num,i,j;                  
 unsigned char temp[8];
 bit Flag;

 LCD_Init();         //初始化液晶屏
 DelayMs(10);        //延时用于稳定，可以去掉
 LCD_Clear();        //清屏
 LCD_Write_String(0,0,"password12345678");    //写入第一行信息，主循环中不再更改此信息，所以在while之前写入
 LCD_Write_String(0,1,"Input  password!");    //写入第二行信息，提示输入密码
while (1)         //主循环
  {


 num=KeyPro();  //扫描键盘
 if(num!=0xff)  //如果扫描是按键有效值则进行处理
   { 
    if(i==0)    //输入是第一个字符的时候需要把改行清空，方便观看密码
	  LCD_Write_String(0,1,"                ");//清除该行
    if(i<8)     //密码是8位，大于8位时不再输入按键值
	 {
     temp[i]=num;
     LCD_Write_Char(i,1,num+'0');
	 }
	i++;   //输入数值累加
	if(i==9)//8位后的按键不输入数值，相当于确认按键（任意按键即可）
	  {
	  i=0;  //计数器复位
	  Flag=1;//先把比较位置1
	  for(j=0;j<8;j++)//循环比较8个数值，如果有一个不等 则最终Flag值为0
	     Flag=Flag&&(temp[j]==password[j]);//比较输入值和已有密码

      if(Flag)//如果比较全部相同，标志位置1
	   {
        LCD_Write_String(0,1,"Right Open!");//密码正确显示的信息
		}
	  else 
	    {
		LCD_Write_String(0,1,"Wrong Retry!");//密码错误，提示重新输入
		}

	  }
    }	
  }
}

