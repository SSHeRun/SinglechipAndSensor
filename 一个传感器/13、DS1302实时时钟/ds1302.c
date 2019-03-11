/*******************************************************************************
*   �������ʾʱ��                                                             *
* ͨ���������˽� DS1302ʱ��оƬ�Ļ���ԭ���ʹ�� ,��Ⲣ����DS1302ʱ��оƬ	   *
* ��������ı�д�Լ�ʵ�������ַ���������е���ʾ��							   *
********************************************************************************/
#include<reg52.h> //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���
#include <intrins.h>

sbit SCK=P3^6;	//ʱ��	
sbit SDA=P3^4;	//����	
sbit RST = P3^5;// DS1302��λ

sbit LS138A=P2^0;  
sbit LS138B=P2^1;
sbit LS138C=P2^2; 

bit ReadRTC_Flag;//�����DS1302��־

unsigned char l_tmpdate[7]={0,0,12,15,5,3,8};//���ʱ��������08-05-15 12:00:00
unsigned char l_tmpdisplay[8];

code unsigned char write_rtc_address[7]={0x80,0x82,0x84,0x86,0x88,0x8a,0x8c}; //���ʱ�������� ���λ��дλ
code unsigned char read_rtc_address[7]={0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};  


code unsigned char table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40}; 
			         //��������� 0-9  '-' 'Ϩ�𡮱�

/******************************************************************/
/*                    ��������                                    */
/******************************************************************/												
void Write_Ds1302_byte(unsigned char temp); 
void Write_Ds1302( unsigned char address,unsigned char dat );
unsigned char Read_Ds1302 ( unsigned char address );
void Read_RTC(void);//read RTC 
void Set_RTC(void); //set RTC 
void InitTIMER0(void);//inital timer0
/******************************************************************/
/*                   ������                                       */
/******************************************************************/
void main(void)    
{
    InitTIMER0();          //��ʼ����ʱ��0
 	//Set_RTC();             //д��ʱ��ֵ�����ʹ�ñ��õ��ʱ�򣬲���Ҫûÿ���ϵ�д�룬�˳���Ӧ������

	while(1)
	{
	  if(ReadRTC_Flag)
	  {
	    ReadRTC_Flag=0;
		Read_RTC();
  	
			l_tmpdisplay[0]=l_tmpdate[2]/16;			//���ݵ�ת���������ǲ��������0~9����ʾ,�����ݷֿ�
			l_tmpdisplay[1]=l_tmpdate[2]&0x0f;
			l_tmpdisplay[2]=10;					    	//����"-"
			l_tmpdisplay[3]=l_tmpdate[1]/16;
			l_tmpdisplay[4]=l_tmpdate[1]&0x0f;
			l_tmpdisplay[5]=10;
			l_tmpdisplay[6]=l_tmpdate[0]/16;
			l_tmpdisplay[7]=l_tmpdate[0]&0x0f;
	
	 }
	}
}
/******************************************************************/
/*                   ��ʱ��0��ʼ��                                */
/******************************************************************/
void InitTIMER0(void)
{
 TMOD|=0x01;//��ʱ������ 16λ
 TH0=0xef;//��ʼ��ֵ
 TL0=0xf0;
 ET0=1;
 TR0=1;
 EA=1;
}


/******************************************************************/
/*                   дһ���ֽ�                                   */
/******************************************************************/
void Write_Ds1302_Byte(unsigned  char temp) 
{
 unsigned char i;
 for (i=0;i<8;i++)     	//ѭ��8�� д������
  { 
   SCK=0;
     SDA=temp&0x01;     //ÿ�δ�����ֽ� 
     temp>>=1;  		//����һλ
     SCK=1;
   }
}   
/******************************************************************/
/*                  д��DS1302                                    */
/******************************************************************/
void Write_Ds1302( unsigned char address,unsigned char dat )     
{
 	RST=0;
	_nop_();
 	SCK=0;
	_nop_();
 	RST=1;	
   	_nop_();                    //����
 	Write_Ds1302_Byte(address);	//���͵�ַ
 	Write_Ds1302_Byte(dat);		//��������
 	RST=0;  		            //�ָ�
}
/******************************************************************/
/*                   ����DS1302����                               */
/******************************************************************/
unsigned char Read_Ds1302 ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	RST=0;
	_nop_();
	_nop_();
 	SCK=0;
	_nop_();
	_nop_();
 	RST=1;
	_nop_();
	_nop_();
 	Write_Ds1302_Byte(address);
 	for (i=0;i<8;i++) 		//ѭ��8�� ��ȡ����
 	{		
 		if(SDA)
 		temp|=0x80;			//ÿ�δ�����ֽ�
		SCK=0;
		temp>>=1;			//����һλ
		_nop_();
	   _nop_();
	   _nop_();
 		SCK=1;
	} 
 	RST=0;
	_nop_();	          	//����ΪDS1302��λ���ȶ�ʱ��
 	_nop_();
	RST=0;
	SCK=0;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	SCK=1;
	_nop_();
	_nop_();
	SDA=0;
	_nop_();
	_nop_();
	SDA=1;
	_nop_();
	_nop_();
	return (temp);			//����
}
/******************************************************************/
/*                   ��ʱ������                                   */
/******************************************************************/
void Read_RTC(void)	        //��ȡ ����
{
 unsigned char i,*p;
 p=read_rtc_address; 	    //��ַ����
 for(i=0;i<7;i++)		    //��7�ζ�ȡ ���ʱ��������
 {
  l_tmpdate[i]=Read_Ds1302(*p);
  p++;
 }
}
/******************************************************************/
/*                  �趨ʱ������                                  */
/******************************************************************/
void Set_RTC(void)		    //�趨 ����
{
	unsigned char i,*p,tmp;
	for(i=0;i<7;i++){       //BCD����
		tmp=l_tmpdate[i]/10;
		l_tmpdate[i]=l_tmpdate[i]%10;
		l_tmpdate[i]=l_tmpdate[i]+tmp*16;
	}  
 	Write_Ds1302(0x8E,0X00);
	
 	p=write_rtc_address;	//����ַ	
 	for(i=0;i<7;i++)		//7��д�� ���ʱ��������
 	{
		  Write_Ds1302(*p,l_tmpdate[i]);
 		 p++;  
	 }
	 Write_Ds1302(0x8E,0x80);
}
/******************************************************************/
/*                   ��ʱ���жϺ���                               */
/******************************************************************/
void tim(void) interrupt 1 using 1
                            //�жϣ����������ɨ��
{

    static unsigned char i,num;
    TH0=0xf5;
    TL0=0xe0;

 	P0=table[l_tmpdisplay[i]];		//����õ�Ҫ��ʾ���ֵ������

    switch(i)					  
	     {	    
			case 0:LS138A=0; LS138B=0; LS138C=0; break;         
	        case 1:LS138A=1; LS138B=0; LS138C=0; break;             	
	        case 2:LS138A=0; LS138B=1; LS138C=0; break; 
	        case 3:LS138A=1; LS138B=1; LS138C=0; break; 
			case 4:LS138A=0; LS138B=0; LS138C=1; break;
			case 5:LS138A=1; LS138B=0; LS138C=1; break;
			case 6:LS138A=0; LS138B=1; LS138C=1; break;
			case 7:LS138A=1; LS138B=1; LS138C=1; break;
			
	     }
	i++;
	if(i==8)
	  {
	   i=0;
	   num++;
	   if(10==num)       //����ʱ���ȡ1302�����ݡ�ʱ�������Ե���
	     {
		 ReadRTC_Flag=1; //ʹ�ñ�־λ�ж�
		 num=0;
		 }
	  }
 }


