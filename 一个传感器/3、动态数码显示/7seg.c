/*******************************************************************************
*  �������ʾ��������λѡ ��������                                             *
********************************************************************************/

#include <reg51.h> 

sbit LS138A = P2^0;  	//����138������������A����P2.0���� 
sbit LS138B = P2^1;	    //����138�������������B��P2.1����
sbit LS138C = P2^2; 	//����138�������������C��P2.2����

void delay(unsigned int i);      //��������

char DelayCNT;
 
//�˱�Ϊ LED ����ģ, ���������   0-9  - 
unsigned char code Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40}; 

/************������**********************/ 
main()
{  
   unsigned int i,LedNumVal ;
   unsigned int LedOut[10];
   
   DelayCNT=0;

  while(1)  //����ѭ��״̬
  {	 
     if(++DelayCNT>=50)
	 {
	 	DelayCNT=0;		   //��ʱ���� ÿɨ��һ�μ�һ��
	    ++LedNumVal;       //ÿ��50��ɨ�����ڼ�һ��
	 }

	 LedOut[0]=Disp_Tab[LedNumVal%10000/1000];
     LedOut[1]=Disp_Tab[LedNumVal%1000/100]|0x80;
     LedOut[2]=Disp_Tab[LedNumVal%100/10];
     LedOut[3]=Disp_Tab[LedNumVal%10];
	 
	 LedOut[4]=Disp_Tab[LedNumVal%10000/1000];	   //ǧλ
     LedOut[5]=Disp_Tab[LedNumVal%1000/100]|0x80;  //��λ��С����
     LedOut[6]=Disp_Tab[LedNumVal%100/10];		   //ʮλ
     LedOut[7]=Disp_Tab[LedNumVal%10];             //��λ	 

	
	 for( i=0; i<8; i++)  //ʵ��8λ��̬ɨ��ѭ��
	 {	 
	  P0 = LedOut[i];  //����ģ�͵�P0����ʾ
	  		
	  switch(i)	  //ʹ��switch ������λѡ  Ҳ�������ò��ķ�ʽ ѧԱ���������Լ��޸�				  
	     {	    
			case 0:LS138A=0; LS138B=0; LS138C=0;  break;         
	        case 1:LS138A=1; LS138B=0; LS138C=0;  break;             	
	        case 2:LS138A=0; LS138B=1; LS138C=0;  break; 
	        case 3:LS138A=1; LS138B=1; LS138C=0;  break; 
			case 4:LS138A=0; LS138B=0; LS138C=1;  break;
			case 5:LS138A=1; LS138B=0; LS138C=1;  break;
			case 6:LS138A=0; LS138B=1; LS138C=1;  break;
			case 7:LS138A=1; LS138B=1; LS138C=1;  break;
		  
	     }
		 
		delay(150);
	  }
	 
  }   
}	

/***************************************************************************
*																		   *
*		                    ��ʱ����						               *
****************************************************************************/
void delay(unsigned int i)
{
    char j;
    for(i; i > 0; i--)
        for(j = 200; j > 0; j--);
}


