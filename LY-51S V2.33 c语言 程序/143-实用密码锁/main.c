/*-----------------------------------------------
  ���ƣ�Һ����ʾ������
  ��̳��www.doflye.net
  ��д��shifang
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ�
------------------------------------------------*/
#include<reg52.h> //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���
#include<stdio.h>
#include"1602.h"
#include"delay.h"
#include"keyboard.h"


unsigned char code password[]={1,2,3,4,5,6,7,8};//���Ը��Ĵ��������������
//�������룬ʵ����������Ҫ����eeprom�У�����ͨ��������ġ����û��������

/*------------------------------------------------
                    ������
------------------------------------------------*/
main()
{
 unsigned char num,i,j;                  
 unsigned char passwordtemp[16];        //�������16��
 unsigned char inputtimes;              //��������������
 unsigned char passwordlength,PLEN;     //�������볤�ȣ�ʵ�����볤��
 bit Flag;
 
 PLEN=sizeof(password)/sizeof(password[0]);//���ڼ����ʵ�����볤��
 
 LCD_Init();         //��ʼ��Һ����
 DelayMs(10);        //��ʱ�����ȶ�������ȥ��
 LCD_Clear();        //����
 LCD_Write_String(0,0,"  Welcome! ");    //д���һ����Ϣ����ѭ���в��ٸ��Ĵ���Ϣ��������while֮ǰд��
 LCD_Write_String(0,1,"Input password!");    //д��ڶ�����Ϣ����ʾ��������
while (1)         //��ѭ��
  {


 num=KeyPro();  //ɨ�����
 if(num!=0xff)  //���ɨ���ǰ�����Чֵ����д���
   { 
    if(i==0)    //�����ǵ�һ���ַ���ʱ����Ҫ�Ѹ�����գ�����ۿ�����
	  LCD_Write_String(0,1,"                ");//�������
	if(i<16)
	  {
       passwordtemp[i]=num;
       LCD_Write_Char(i,1,'*');//�����������"*"����
	   }
	i++;   //������ֵ�ۼ�
	if((15==num)|| (i==16))//���밴��ֵ15�����������뵽���ֵ16����ʾ�����������Ҫ���бȶ�
	  {
      passwordlength=i-1;  //�����������볤��
	  i=0;  //��������λ
	  if(passwordlength==PLEN)//���������Ƚϣ�����ֱ���������
	    {
	     Flag=1;//�ȰѱȽ�λ��1
	     for(j=0;j<PLEN;j++)//ѭ���Ƚ�8����ֵ�������һ������ ������FlagֵΪ0
	        Flag=Flag&&(passwordtemp[j]==password[j]);//�Ƚ�����ֵ����������
         }
      if(Flag)//����Ƚ�ȫ����ͬ����־λ��1
	   {
	    LCD_Write_String(0,1,"                ");//�������
        LCD_Write_String(0,1,"Right Open!>>>>");//������ȷ��ʾ����Ϣ
        inputtimes=0;//������ȷ��������㣬���¼���
        Flag=0;      //�����ȷ��־
		}
	  else 
	    {
		LCD_Write_String(0,1,"                ");//�������
		LCD_Write_String(0,1,"Wrong! Retry!");//���������ʾ��������
		inputtimes++;//�����������������ۼ�
		if(inputtimes==3)
		  {
		   LCD_Write_String(0,1,"                ");//�������
		   LCD_Write_String(0,1,"Wrong 3 times!");//���������ʾ��������
		   while(1);//ֹͣ��λ�ã�������Դ��������룬ʵ��ʵ��������Ҫ�ȵ�һ��ʱ�������ٴ����롣
		  }
		}

	  }
    }	
  }
}

