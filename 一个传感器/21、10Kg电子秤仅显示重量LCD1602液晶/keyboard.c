#include "keyboard.h"

//按键扫描函数 :    
//功能；只扫描一个按键，按键为号小优先  
//返回键值：
//         7    8    9    10	   //10:清除单价并且清除重量	
//         4    5    6    11	   //11:清除单价最后一位
//         1    2    3    12	   //12:去皮
//         14   0    15   13	   //14:无功能（用于扩展）15：小数点  13：确定

unsigned char Getkeyboard(void)
{
    unsigned char number = 0;
    unsigned char i;
	
	//行输入低电平
    ROW1=ROW2=ROW3=ROW4=0;
	//如果列输出不全为高，则说明有键按下
	if ((COL1!=1)||(COL2!=1)||(COL3!=1)||(COL4!=1))
	{
	   //输入0111
	   ROW1=0;
	   ROW2=ROW3=ROW4=1;
	   for (i=0;i<20;i++);
	   if (COL1==0) return 7;	   // 1
	   else if (COL2==0) return 8;	//	 2
	   else if (COL3==0) return 9;	 //	3
	   else if (COL4==0) return 10;	 //	 10
	   //输入1011
	   ROW2=0;
	   ROW1=ROW3=ROW4=1;
	   for (i=0;i<20;i++);
	   if (COL1==0) return 4;	  //4
	   else if (COL2==0) return 5; //  5
	   else if (COL3==0) return 6;	// 6
	   else if (COL4==0) return 11;	 //	11
	   //输入1101
	   ROW3=0;
	   ROW1=ROW2=ROW4=1;
	   for (i=0;i<20;i++);
	   if (COL1==0) return 1;  //7
	   else if (COL2==0) return 2;	  // 8
	   else if (COL3==0) return 3;	  //   9
	   else if (COL4==0) return 12;	  //   12
	   //输入0111
	   ROW4=0;
	   ROW1=ROW2=ROW3=1;
	   for (i=0;i<20;i++);
	   if (COL1==0) return 14;
	   else if (COL2==0) return 0;
	   else if (COL3==0) return 15;
	   else if (COL4==0) return 13;
	   //没有检测到列低电平
	   return 99;
	}
	return 99;
}
