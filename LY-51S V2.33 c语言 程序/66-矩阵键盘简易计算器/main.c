/*-----------------------------------------------
  名称：液晶显示计算器
  论坛：www.doflye.net
  编写：shifang
  日期：2009.5
  修改：无
  内容：整数之间运算，没有小数所有除法得出的结果不正确，有负号运算，
------------------------------------------------*/
#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
#include<stdio.h>
#include"display.h"
#include"delay.h"
#include"keyboard.h"


/*------------------------------------------------
                    主程序
------------------------------------------------*/
main()
{
 unsigned char num,i,sign;                   
 unsigned char temp[8];        //最大输入8个
 bit firstflag;
 int a=0,b=0;
 unsigned char s;

 Init_Timer0();   //初始化定时器0
while (1)         //主循环
  {


 num=KeyPro();  //扫描键盘
 if(num!=0xff)  //如果扫描是按键有效值则进行处理
   { 
    if(i==0)    //输入是第一个字符的时候需要把改行清空，方便观看
	  {
	  for(s=0;s<8;s++) //赋值完成后把缓冲区清零，防止下次输入影响结果
		  TempData[s]=0;
	  }
	if(('+'==num)|| (i==8) || ('-'==num) || ('x'==num)|| ('/'==num) || ('='==num))//输入数字最大值8，输入符号表示输入结束
	  {
	  i=0;  //计数器复位

	 if(firstflag==0)  //如果是输入的第一个数据，赋值给a，并把标志位置1，到下一个数据输入时可以跳转赋值给b
	     {
	     sscanf(temp,"%d",&a);//从一个字符串输入到变量
         firstflag=1;
		 }
	  else  
	     sscanf(temp,"%d",&b);
	  for(s=0;s<8;s++) //赋值完成后把缓冲区清零，防止下次输入影响结果
		    temp[s]=0;
      
	  ///////////////////////
	  if(num!='=')      //判断当前符号位并做相应处理
	     sign=num;      //如果不是等号记下标志位
	  else
	     {  
		 firstflag=0;   //检测到输入=号，判断上次读入的符合
         switch(sign)
	       {
		    case '+':a=a+b;
			break;
			case '-':a=a-b;
			break;
			case 'x':a=a*b;
			break;
			case '/':a=a/b;
			break;
			default:break;
		   }
		 sprintf(temp,"%d",a);  //打印十进制到临时缓冲区
 		 for(s=0;s<8;s++)       //由于打印的是ASCII码值
		    { 
			if(temp[s]==0)      //所以需要转换，如果为0表示null 数码管上则不能显示，所以赋值0
                TempData[s]=0;
			else if(temp[s]==0x2d)//表示负号，数码管显示负号 0x40
			     TempData[s]=0x40;
	    	else 
		         TempData[s]=dofly_DuanMa[temp[s]-'0'];//其他0-9负号则进行ASCII 到 数字处理，如当前是'3'，用'3'-'0'=3
				                                       //'3'的16进制是0x33,'0'的16进制是0x30			     
			}
		 sign=0;a=b=0;            //用完后所有数据清零
		 for(s=0;s<8;s++)
		    temp[s]=0;
		 }
	  }

   else	if(i<16)
	  {

         temp[i]=num+'0'; 
         TempData[i]=dofly_DuanMa[num];//输出数据
	     i++;   //输入数值累加
	   }
  
    }	
  }
}

