/*******************************************************************************
*  ������ʾ��������λѡ ��������            *
********************************************************************************/

#include <reg51.h> 

sbit LS138A = P2^0;  	//����138������������A����P2.0���� 
sbit LS138B = P2^1;	    //����138�������������B��P2.1����
sbit LS138C = P2^2; 	//����138�������������C��P2.2����

void delay(unsigned int i);      //��������



/************������**********************/ 
main()
{  
   unsigned int i,j ;

  while(1)  //����ѭ��״̬
  {	
	 for( i=0; i<8; i++)  //ʵ��8λ��̬ɨ��ѭ��
	 {	 		
	  switch(i)	  //ʹ��switch ������λѡ  Ҳ�������ò��ķ�ʽ ѧԱ���������Լ��޸�				  
	     {	    
			case 0:LS138A=0; LS138B=0; LS138C=0; P0 = 0X18; break;         
	        case 1:LS138A=1; LS138B=0; LS138C=0; P0 = 0X1C; break;             	
	        case 2:LS138A=0; LS138B=1; LS138C=0; P0 = 0X18; break; 
	        case 3:LS138A=1; LS138B=1; LS138C=0; P0 = 0X18; break; 
			case 4:LS138A=0; LS138B=0; LS138C=1; P0 = 0X18; break;
			case 5:LS138A=1; LS138B=0; LS138C=1; P0 = 0X18; break;
			case 6:LS138A=0; LS138B=1; LS138C=1; P0 = 0X3C; break;
			case 7:LS138A=1; LS138B=1; LS138C=1; P0 = 0X3C;	break;	  
	     }
		  delay(150);
	 }
	 //delay(150000);


/*	 for( j=0; j<8; j++)  //ʵ��8λ��̬ɨ��ѭ��
	 {	 
	  switch(j)	  //ʹ��switch ������λѡ  Ҳ�������ò��ķ�ʽ ѧԱ���������Լ��޸�				  
	     {	    
			case 0:LS138A=0; LS138B=0; LS138C=0; P0 = 0XFF; break;         
	        case 1:LS138A=1; LS138B=0; LS138C=0; P0 = 0XFF; break;             	
	        case 2:LS138A=0; LS138B=1; LS138C=0; P0 = 0XFF; break; 
	        case 3:LS138A=1; LS138B=1; LS138C=0; P0 = 0XFF; break; 
			case 4:LS138A=0; LS138B=0; LS138C=1; P0 = 0XFF; break;
			case 5:LS138A=1; LS138B=0; LS138C=1; P0 = 0XFF; break;
			case 6:LS138A=0; LS138B=1; LS138C=1; P0 = 0XFF; break;
			case 7:LS138A=1; LS138B=1; LS138C=1; P0 = 0XFF;	break;	  
	     }
		  delay(150);
	 }
	delay(150000);			*/

	 
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


