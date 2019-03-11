/*-----------------------------------------------
  ���ƣ�LCD1602
  ��̳��www.doflye.net
  ��д��shifang
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ�ͨ������������룬������ʾ0-F16���ַ�
  ���Ŷ������£�1-VSS 2-VDD 3-V0 4-RS 5-R/W 6-E 7-14 DB0-DB7 15-BLA 16-BLK
------------------------------------------------*/
#include<reg52.h> //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���
#include<intrins.h>

sbit RS = P2^4;   //����˿� 
sbit RW = P2^5;
sbit EN = P2^6;

#define RS_CLR RS=0 
#define RS_SET RS=1

#define RW_CLR RW=0 
#define RW_SET RW=1 

#define EN_CLR EN=0
#define EN_SET EN=1

#define DataPort P0
#define KeyPort P1

unsigned char code dofly_code[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};//ת����Һ����ʾ���ַ�
/*------------------------------------------------
 uS��ʱ����������������� unsigned char t���޷���ֵ
 unsigned char �Ƕ����޷����ַ���������ֵ�ķ�Χ��
 0~255 ����ʹ�þ���12M����ȷ��ʱ��ʹ�û��,������ʱ
 �������� T=tx2+5 uS 
------------------------------------------------*/
void DelayUs2x(unsigned char t)
{   
 while(--t);
}
/*------------------------------------------------
 mS��ʱ����������������� unsigned char t���޷���ֵ
 unsigned char �Ƕ����޷����ַ���������ֵ�ķ�Χ��
 0~255 ����ʹ�þ���12M����ȷ��ʱ��ʹ�û��
------------------------------------------------*/
void DelayMs(unsigned char t)
{
     
 while(t--)
 {
     //������ʱ1mS
     DelayUs2x(245);
	 DelayUs2x(245);
 }
}
/*------------------------------------------------
              ��æ����
------------------------------------------------*/
 bit LCD_Check_Busy(void) 
 { 
 DataPort= 0xFF; 
 RS_CLR; 
 RW_SET; 
 EN_CLR; 
 _nop_(); 
 EN_SET;
 return (bit)(DataPort & 0x80);
 }
/*------------------------------------------------
              д�������
------------------------------------------------*/
 void LCD_Write_Com(unsigned char com) 
 {  
// while(LCD_Check_Busy()); //æ��ȴ�
 DelayMs(5);
 RS_CLR; 
 RW_CLR; 
 EN_SET; 
 DataPort= com; 
 _nop_(); 
 EN_CLR;
 }
/*------------------------------------------------
              д�����ݺ���
------------------------------------------------*/
 void LCD_Write_Data(unsigned char Data) 
 { 
 //while(LCD_Check_Busy()); //æ��ȴ�
 DelayMs(5);
 RS_SET; 
 RW_CLR; 
 EN_SET; 
 DataPort= Data; 
 _nop_();
 EN_CLR;
 }


/*------------------------------------------------
                ��������
------------------------------------------------*/
 void LCD_Clear(void) 
 { 
 LCD_Write_Com(0x01); 
 DelayMs(5);
 }
/*------------------------------------------------
              д���ַ�������
------------------------------------------------*/
 void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) 
 {     
 if (y == 0) 
 	{     
	 LCD_Write_Com(0x80 + x);     
 	}
 else 
 	{     
 	LCD_Write_Com(0xC0 + x);     
 	}        
 while (*s) 
 	{     
 LCD_Write_Data( *s);     
 s ++;     
 	}
 }
/*------------------------------------------------
              д���ַ�����
------------------------------------------------*/
 void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data) 
 {     
 if (y == 0) 
 	{     
 	LCD_Write_Com(0x80 + x);     
 	}    
 else 
 	{     
 	LCD_Write_Com(0xC0 + x);     
 	}        
 LCD_Write_Data( Data);  
 }
/*------------------------------------------------
              ��ʼ������
------------------------------------------------*/
 void LCD_Init(void) 
 {
   LCD_Write_Com(0x38);    /*��ʾģʽ����*/ 
   DelayMs(5); 
   LCD_Write_Com(0x38); 
   DelayMs(5); 
   LCD_Write_Com(0x38); 
   DelayMs(5); 
   LCD_Write_Com(0x38);  
   LCD_Write_Com(0x08);    /*��ʾ�ر�*/ 
   LCD_Write_Com(0x01);    /*��ʾ����*/ 
   LCD_Write_Com(0x06);    /*��ʾ����ƶ�����*/ 
   DelayMs(5); 
   LCD_Write_Com(0x0C);    /*��ʾ�����������*/
   }
 
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
------------------------------------------------*/
unsigned char KeyPro(void)
{
 switch(KeyScan())
 {
  case 0x7e:return 0;break;//0 ������Ӧ�ļ���ʾ���Ӧ����ֵ
  case 0x7d:return 1;break;//1
  case 0x7b:return 2;break;//2
  case 0x77:return 3;break;//3
  case 0xbe:return 4;break;//4
  case 0xbd:return 5;break;//5
  case 0xbb:return 6;break;//6
  case 0xb7:return 7;break;//7
  case 0xde:return 8;break;//8
  case 0xdd:return 9;break;//9
  case 0xdb:return 10;break;//a
  case 0xd7:return 11;break;//b
  case 0xee:return 12;break;//c
  case 0xed:return 13;break;//d
  case 0xeb:return 14;break;//e
  case 0xe7:return 15;break;//f
  default:return 0xff;break;
 }
}

/*------------------------------------------------
                    ������
------------------------------------------------*/ 
void main(void) 
{ 
unsigned char i,j,num; 

LCD_Init(); 
LCD_Write_Com(0x0F);//��꿪�������˸��

LCD_Write_String(0,0,"Press the key !");

while (1) 
 {  
 num=KeyPro();
 if(num!=0xff)
   {
    if((i==0)&&(j==0))//�ص���һ���ַ�ʱ����
	    LCD_Clear();//����
    LCD_Write_Char(0+i,0+j,dofly_code[num]);//������ʾ�����ַ�
	i++;
	if(i==16)//�����һ����ʾ����ת���ڶ���
	  {
	   i=0;j++;
	   if(j==2)//���2�ж���ʾ�������������´ӵ�һ����ʾ
	   {
	      j=0;
		  }
	  }
   }
 }
}

