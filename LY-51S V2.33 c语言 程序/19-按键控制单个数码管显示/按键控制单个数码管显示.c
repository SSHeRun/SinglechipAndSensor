/*-----------------------------------------------
  名称：单个共阳数码管动态显示变化数字
  论坛：www.doflye.net
  编写：shifang
  日期：2009.5
  修改：无
  内容：通过循环检测P1口按键输入值，然后用数码管输出，
        没有按键按下时原值不变 
------------------------------------------------*/
#include<reg52.h> //包含头文件，一般情况不需要改动，
//头文件包含特殊功能寄存器的定义

unsigned char code dofly_table[10]={0xc0,0xf9,0xa4,0xb0,0x99,
                                    0x92,0x82,0xf8,0x80,0x90,}; 
									// 显示数值表0-9

/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main (void)
{
                  

while (1)         //主循环
  {

  switch(P3)  //P3口作为独立按键输入端，检测端口电平并做如下判断                            
       {
	    case 0xfe:P1=dofly_table[1];break;//0xfe = 1111 1110,
//说明连接在P3.0端口的按键被按下,显示对应的数字然后跳出循环
		case 0xfd:P1=dofly_table[2];break;//调用表中的第三个
//元素 0xa4 下标 0 才表示数组中的第一个元素
		case 0xfb:P1=dofly_table[3];break;
		case 0xf7:P1=dofly_table[4];break;
		case 0xef:P1=dofly_table[5];break;
		case 0xdf:P1=dofly_table[6];break;
		case 0xbf:P1=dofly_table[7];break;
		case 0x7f:P1=dofly_table[8];break;
		default:break;    //如果都没按下，直接跳出
	   }
                     //主循环中添加其他需要一直工作的程序
  }
}