/*-----------------------------------------------
  ���ƣ��������
  ��̳��www.doflye.net
  ��д��shifang
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ�
------------------------------------------------*/
#include<reg52.h> //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���
#include"keyboard.h"
#include"delay.h"

#define KeyPort P1

/*------------------------------------------------
����ɨ�躯��������ɨ���ֵ
------------------------------------------------*/
unsigned char KeyScan(void)  //����ɨ�躯����ʹ�����з�תɨ�跨
{
 unsigned char cord_h,cord_l;//����ֵ�м����
 KeyPort=0x0f;            //�������ȫΪ0
 cord_h=KeyPort&0x0f;     //��������ֵ
 if(cord_h!=0x0f)    //�ȼ�����ް�������
 {
  DelayMs(10);        //ȥ��
  if((KeyPort&0x0f)!=0x0f)
  {
    cord_h=KeyPort&0x0f;  //��������ֵ
    KeyPort=cord_h|0xf0;  //�����ǰ����ֵ
    cord_l=KeyPort&0xf0;  //��������ֵ

    while((KeyPort&0xf0)!=0xf0);//�ȴ��ɿ������

    return(cord_h+cord_l);//������������ֵ
   }
  }return(0xff);     //���ظ�ֵ
}
/*------------------------------------------------
          ����ֵ������������ɨ��ֵ
           ���Ը�����Ҫ�ı䷵��ֵ

  			| 1 | 2 | 3 | + |  
  			| 4 | 5 | 6 | - |  
  			| 7 | 8 | 9 | * |  
  			| 0 | . | = | / | 
------------------------------------------------*/
unsigned char KeyPro(void)
{
 switch(KeyScan())
 {
  case 0x7e:return 1  ;break;//0 ������Ӧ�ļ���ʾ���Ӧ����ֵ
  case 0x7d:return 2  ;break;//1
  case 0x7b:return 3  ;break;//2
  case 0x77:return '+';break;//3

  case 0xbe:return 4  ;break;//4
  case 0xbd:return 5  ;break;//5
  case 0xbb:return 6  ;break;//6
  case 0xb7:return '-';break;//7

  case 0xde:return 7  ;break;//8
  case 0xdd:return 8  ;break;//9
  case 0xdb:return 9  ;break;//a
  case 0xd7:return 'x';break;//b

  case 0xee:return 0  ;break;//c
  case 0xed:return '.';break;//d
  case 0xeb:return '=';break;//e
  case 0xe7:return '/';break;//f
  default:return 0xff;break;
 }
}