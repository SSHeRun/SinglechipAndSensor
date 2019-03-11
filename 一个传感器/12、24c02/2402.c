 
 /*************************�˲���ΪAT2402����������ʹ��I2C��������*************************************/

#include<Function.h>

//AT2402�Ĺ��ܺ���
/*******************************************************************
                    �����ӵ�ַ�������Ͷ��ֽ����ݺ���               
����ԭ��: bit  ISendStr(UCHAR sla,UCHAR suba,ucahr *s,UCHAR no);  
����:     ���������ߵ����͵�ַ���ӵ�ַ,���ݣ��������ߵ�ȫ����,������
          ��ַsla���ӵ�ַsuba������������sָ������ݣ�����no���ֽڡ�
          �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
bit ISendStr(unsigned char  sla,unsigned char  suba,unsigned char  *s,unsigned char  no)
{
   unsigned char i;

   Start_I2c();               /*��������*/
   SendByte(sla);             /*����������ַ*/
   if(ack==0)return(0);
   SendByte(suba);            /*���������ӵ�ַ*/
   if(ack==0)return(0);

   for(i=0;i<no;i++)
   {   
     SendByte(*s);            /*��������*/
     if(ack==0)return(0);
     s++;
   } 
   Stop_I2c();                /*��������*/ 

   return(1);
}

/*******************************************************************
                    �����ӵ�ַ������ȡ���ֽ����ݺ���               
����ԭ��: bit  RecndStr(UCHAR sla,UCHAR suba,ucahr *s,UCHAR no);  
����:     ���������ߵ����͵�ַ���ӵ�ַ,�����ݣ��������ߵ�ȫ����,������
          ��ַsla���ӵ�ַsuba�����������ݷ���sָ��Ĵ洢������no���ֽڡ�
           �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
bit IRcvStr(unsigned char  sla,unsigned char  suba,unsigned char  *s,unsigned char  no)
{
   unsigned char i;

   Start_I2c();                  /*��������*/
   SendByte(sla);                /*����������ַ*/
   if(ack==0)return(0);
   SendByte(suba);               /*���������ӵ�ַ*/
   if(ack==0)return(0);

   Start_I2c();                 /*������������*/
   SendByte(sla+1);
   if(ack==0)return(0);
   for(i=0;i<no-1;i++)
   {   
     *s=RcvByte();               /*��������*/
      Ack_I2c(0);                /*���;ʹ�λ*/  
     s++;
   } 
   *s=RcvByte();
   Ack_I2c(1);                   /*���ͷ�Ӧλ*/
   Stop_I2c();                   /*��������*/ 
   return(1);
}



