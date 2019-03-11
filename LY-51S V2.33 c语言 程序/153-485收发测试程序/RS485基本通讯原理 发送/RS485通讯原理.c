 
/*-----------------------------------------------
  ���ƣ�����ͨ�� rs485
  ��վ��www.doflye.net
  ��д��shifang
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ�485 �� 232 ʹ����ͬ���Э�飬�ǰ�˫������Ҫ��2�׿�����Է����ԣ�����ʹ��
        ����һ��485�豸���ף������ṩ���Գ���
        ���Ͷ˽��߷�ʽ���£�
        P2.0----E(J40) �շ����ƶ�
        TDX ----DI(J40)���ݽ��� ���485����
        RXD ----RO(J40)���ݷ��� ���485����

        A��B�ֱ�������һ�鿪����� A��B

        ���Ͷ�ʹ�ö����������룺
        P1 ----- J26 ��������
------------------------------------------------*/

#include<reg52.h> //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���                        
#include"delay.h"

unsigned char KeyScan(void);
void SendByte(unsigned char dat);
/*------------------------------------------------
                Ӳ���˿ڶ���
------------------------------------------------*/
#define KeyPort P1

sbit Ctrl_EN = P2^0;  //���ͽ��տ��ƶ�
/*------------------------------------------------
                 ��������
------------------------------------------------*/
void SendStr(unsigned char *s);
/*------------------------------------------------
                ���ڳ�ʼ��
------------------------------------------------*/
void InitUART  (void)
{

    SCON  = 0x50;		        // SCON: ģʽ 1, 8-bit UART, ʹ�ܽ���  
    TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit ��װ
    TH1   = 0xFD;               // TH1:  ��װֵ 9600 ������ ���� 11.0592MHz  
    TR1   = 1;                  // TR1:  timer 1 ��                         
    EA    = 1;                  //�����ж�
    //ES    = 1;                  //�򿪴����ж�
}                            
/*------------------------------------------------
                    ������
------------------------------------------------*/
void main (void)
{
unsigned char keynum;
InitUART();

Ctrl_EN=1;  //����ģʽ
//SendStr("UART test��������̳��www.doflye.net thank you!");
while (1)                       
    {
    keynum= KeyScan();
    if(keynum!=0)
       SendByte(keynum);

    }
}

/*------------------------------------------------
                    ����һ���ֽ�
------------------------------------------------*/
void SendByte(unsigned char dat)
{
 SBUF = dat;
 while(!TI);
      TI = 0;
}
/*------------------------------------------------
                    ����һ���ַ���
------------------------------------------------*/
void SendStr(unsigned char *s)
{
 while(*s!='\0')// \0 ��ʾ�ַ���������־��ͨ������Ƿ��ַ���ĩβ
  {
  SendByte(*s);
  s++;
  }
}

/*------------------------------------------------
          ����ɨ�躯��������ɨ���ֵ
------------------------------------------------*/
unsigned char KeyScan(void)
{
 unsigned char keyvalue;
 if(KeyPort!=0xff)
   {
    DelayMs(10);
    if(KeyPort!=0xff)
	   {
	    keyvalue=KeyPort;
	    while(KeyPort!=0xff);
		switch(keyvalue)
		{
		 case 0xfe:return 1;break;
		 case 0xfd:return 2;break;
		 case 0xfb:return 3;break;
		 case 0xf7:return 4;break;
		 case 0xef:return 5;break;
		 case 0xdf:return 6;break;
		 case 0xbf:return 7;break;
		 case 0x7f:return 8;break;
		 default:return 0;break;
		}
	  }
   }
   return 0;
}

