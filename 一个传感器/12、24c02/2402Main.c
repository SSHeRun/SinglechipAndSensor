/*******************************************************************************
 ���⣺                  EEPROM��д����(��ť����)							   *
********************************************************************************
																			   *
������Ϥ�����գ��洢оƬAT2402���Ĳ������б���Ͷ�ȡ���� 	                   *			   
�˽�����I2C���߽ӿڵĹ���ԭ��һ���̷�����								   *
                                                                               *		  
���߲�����P0��JP10���� ����ܣ�JP12��										   *
          P1��JP8 ���� ����������JP5��										   *
																			   *
���������	��1����ť����      ��2����ť��ȡ							       *
            ��3����ť����      ��4����ť����							       *
																			   *		  	   							  
********************************************************************************/

#include<Function.h>
#include<reg52.h>

#define  AT24C02 0xa0  //AT24C02 ��ַ

//else IO
sbit    LS138A=P2^0;  	//��������
sbit    LS138B=P2^1;
sbit    LS138C=P2^2; 

sbit    K1 = P3^2;	 //����
sbit    K2 = P3^3;	 //��ȡ
sbit    K3 = P3^4;	 //+����
sbit    K4 = P3^5;   //-����

void delay(unsigned int i);


//�˱�Ϊ LED ����ģ, ��������� 0-9  - 
unsigned char code Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40}; 

unsigned char Count1;
unsigned long D[16], LedOut[5],LedNumVal;
unsigned int idata USEC;

  
void  system_ini() 
{    
    TMOD|= 0x11;
    TH1 = 0xfe; //11.0592
	TL1 = 0x33;
	TR1  = 1; 
    IE =0x8A;

}

 main()
 { 	unsigned char i;
	unsigned char pDat[8];

   system_ini();
   while(1)
   {
		 //========================IIC ��ȡ
	if(K2 == 0)	   //�ڶ�����ť��ȡ����
	{ 
	  IRcvStr(AT24C02, 0 , &pDat[0], 8); 
	  for (i=0; i<4; i++)
	  {
		D[14+i]=pDat[i*2+0]+pDat[i*2+1]*0x100;
		Count1 = D[14];
	   }	     
	 }

	   //========================IIC ����
	   if(K1 == 0)    //��һ����ť��������
	   { 
	      D[14]= Count1;
		 for (i=0; i<4; i++)
	     {
		   pDat[i*2+0]=D[14+i];
		   pDat[i*2+1]=D[14+i]>>8;
		 }
	     ISendStr(AT24C02, 0 , &pDat[0], 8); 
       }


	  /********���½�2402�б���������͵�LED�������ʾ*************/
        LedNumVal=Count1;
	  LedOut[0]=Disp_Tab[LedNumVal%10000/1000];
      LedOut[1]=Disp_Tab[LedNumVal%1000/100];
      LedOut[2]=Disp_Tab[LedNumVal%100/10]|0x80;
      LedOut[3]=Disp_Tab[LedNumVal%10]; 
  	  
    for(i=0; i<4; i++) 
	  {		
		
	    P0 =  LedOut[i];
	   	
		switch(i)					  
         {	    
			case 0:LS138A=0; LS138B=0; LS138C=0; break;         
            case 1:LS138A=1; LS138B=0; LS138C=0; break;             	
            case 2:LS138A=0; LS138B=1; LS138C=0; break; 
            case 3:LS138A=1; LS138B=1; LS138C=0; break; 
			case 4:LS138A=0; LS138B=0; LS138C=1; break;
         }
		 
		delay(100);
	  }

	  //  LS138A=1; LS138B=1; LS138C=1;    

   }
}


void delay(unsigned int i)
{
    char j;
    for(i; i > 0; i--)
        for(j = 200; j > 0; j--);
}


/*************************************
 [ t1 (0.5ms)�ж�] �ж����� PWM ���
  ------------1000/(0.02ms*250)=200Hz
*************************************/
void T1zd(void) interrupt 3   //3 Ϊ��ʱ��1���жϺ�  1 ��ʱ��0���жϺ� 0 �ⲿ�ж�1 2 �ⲿ�ж�2  4 �����ж�
{
	 TH1 = 0xfe; //12M	
	 TL1 = 0x33;
   if(USEC++==200)
   {	USEC=0;
	 
    if (K3) Count1++;     //�ı�����
    if (K4&Count1!=0) Count1--;
  
    }  

}


