#include "keyboard.h"

//����ɨ�躯�� :    
//���ܣ�ֻɨ��һ������������Ϊ��С����  
//���ؼ�ֵ��
//         7    8    9    10	   //10:������۲����������	
//         4    5    6    11	   //11:����������һλ
//         1    2    3    12	   //12:ȥƤ
//         14   0    15   13	   //14:�޹��ܣ�������չ��15��С����  13��ȷ��

unsigned char Getkeyboard(void)
{
    unsigned char number = 0;
    unsigned char i;
	
	//������͵�ƽ
    ROW1=ROW2=ROW3=ROW4=0;
	//����������ȫΪ�ߣ���˵���м�����
	if ((COL1!=1)||(COL2!=1)||(COL3!=1)||(COL4!=1))
	{
	   //����0111
	   ROW1=0;
	   ROW2=ROW3=ROW4=1;
	   for (i=0;i<20;i++);
	   if (COL1==0) return 7;	   // 1
	   else if (COL2==0) return 8;	//	 2
	   else if (COL3==0) return 9;	 //	3
	   else if (COL4==0) return 10;	 //	 10
	   //����1011
	   ROW2=0;
	   ROW1=ROW3=ROW4=1;
	   for (i=0;i<20;i++);
	   if (COL1==0) return 4;	  //4
	   else if (COL2==0) return 5; //  5
	   else if (COL3==0) return 6;	// 6
	   else if (COL4==0) return 11;	 //	11
	   //����1101
	   ROW3=0;
	   ROW1=ROW2=ROW4=1;
	   for (i=0;i<20;i++);
	   if (COL1==0) return 1;  //7
	   else if (COL2==0) return 2;	  // 8
	   else if (COL3==0) return 3;	  //   9
	   else if (COL4==0) return 12;	  //   12
	   //����0111
	   ROW4=0;
	   ROW1=ROW2=ROW3=1;
	   for (i=0;i<20;i++);
	   if (COL1==0) return 14;
	   else if (COL2==0) return 0;
	   else if (COL3==0) return 15;
	   else if (COL4==0) return 13;
	   //û�м�⵽�е͵�ƽ
	   return 99;
	}
	return 99;
}
