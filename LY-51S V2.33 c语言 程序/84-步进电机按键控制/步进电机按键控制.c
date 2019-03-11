/*-----------------------------------------------
  ���ƣ��������
  ��̳��www.doflye.net
  ��д��shifang
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ����������ڲ���4�ಽ������������� 
        ʹ��1-2������
        1-2�༤�����������������Ƕȼ��룬��������
        ˳������ a-ab-b-bc-c-cd-d-da   �ֳ�4��8��
 ------------------------------------------------*/

#include <reg52.h>



sbit A1=P1^0; //���岽��������Ӷ˿�
sbit B1=P1^1;
sbit C1=P1^2;
sbit D1=P1^3;


#define Coil_AB1 {A1=1;B1=1;C1=0;D1=0;}//AB��ͨ�磬������ϵ�
#define Coil_BC1 {A1=0;B1=1;C1=1;D1=0;}//BC��ͨ�磬������ϵ�
#define Coil_CD1 {A1=0;B1=0;C1=1;D1=1;}//CD��ͨ�磬������ϵ�
#define Coil_DA1 {A1=1;B1=0;C1=0;D1=1;}//D��ͨ�磬������ϵ�
#define Coil_A1 {A1=1;B1=0;C1=0;D1=0;}//A��ͨ�磬������ϵ�
#define Coil_B1 {A1=0;B1=1;C1=0;D1=0;}//B��ͨ�磬������ϵ�
#define Coil_C1 {A1=0;B1=0;C1=1;D1=0;}//C��ͨ�磬������ϵ�
#define Coil_D1 {A1=0;B1=0;C1=0;D1=1;}//D��ͨ�磬������ϵ�
#define Coil_OFF {A1=0;B1=0;C1=0;D1=0;}//ȫ���ϵ�

unsigned char Speed;
bit Flag;
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
                    ������
------------------------------------------------*/
main()
{
 unsigned int i=512;//��תһ��ʱ��
 
 EA=1;          //ȫ���жϿ�
 EX0=1;         //�ⲿ�ж�0��
 IT0=1;         //1��ʾ���ش���

 Speed=10;
while(1){
 Coil_OFF
 while((i--)&&Flag)  //����
  {  Coil_A1       
     DelayMs(Speed);
     Coil_AB1                //����Coil_AB1  ��{A1=1;B1=1;C1=0;D1=0;}����
     DelayMs(Speed);         //�ı�����������Ե������ת�� ,
                             //����ԽС��ת��Խ��,����ԽС
	 Coil_B1       
     DelayMs(Speed);
     Coil_BC1
     DelayMs(Speed);
	 Coil_C1       
     DelayMs(Speed);
     Coil_CD1
     DelayMs(Speed);
	 Coil_D1       
     DelayMs(Speed);
     Coil_DA1
     DelayMs(Speed);
  }
  Coil_OFF
  i=512;
  while((i--)&&(!Flag))//����
  {  
     Coil_A1       
     DelayMs(Speed);
     Coil_DA1                //����Coil_AB1  ��{A1=1;B1=1;C1=0;D1=0;}����
     DelayMs(Speed);         //�ı�����������Ե������ת�� ,
                             //����ԽС��ת��Խ��,����ԽС
	 Coil_D1       
     DelayMs(Speed);
     Coil_CD1
     DelayMs(Speed);
	 Coil_C1       
     DelayMs(Speed);
     Coil_BC1
     DelayMs(Speed);
	 Coil_B1       
     DelayMs(Speed);
     Coil_AB1
     DelayMs(Speed);
  }
 }
}

/*------------------------------------------------
                 �ⲿ�жϳ���
------------------------------------------------*/
void ISR_INT0(void) interrupt 0 
{
 
 if(!INT0)
   {
   DelayMs(10);//�ڴ˴��������ȥ�������򣬷�ֹ����������ɴ���
   if(!INT0)
   //while(!INT1);//�ȴ������ͷ�
     {
     Flag=!Flag;   
     }
   }
}