#include<reg52.h>
#include<intrins.h>
#define uchar unsigned char
#define uint  unsigned int

/********************************************************************
* ���� : Delay()
* ���� : ��ʱ,��ʱʱ��Ϊ 10ms * del
* ���� : del
* ��� : ��
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
			P1 = _cror_(P1, 1);		//_crol_()���������intrins.h�ж����ˣ�����ѭ������
			Delay(15);
		}

	}
}