#include<reg52.h>
#include<intrins.h>
#define uchar unsigned char
#define uint  unsigned int

/********************************************************************
* 名称 : Delay()
* 功能 : 延时,延时时间为 10ms * del
* 输入 : del
* 输出 : 无
***********************************************************************/
void Delay(uint del)
{
	uint i,j;
	for(i=0; i<del; i++)
		for(j=0; j<1827; j++)   
	;
}

void Main(void)
{
	uchar i;
	while(1)
	{
		P1 = 0x1;				
		for(i=0; i<=7; i++)		
		{
			P1 = _cror_(P1, 1);		//_crol_()这个函数在intrins.h中定义了，用于循环右移
			Delay(15);
		}

	}
}