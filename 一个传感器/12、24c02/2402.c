 
 /*************************此部分为AT2402的驱动程序使用I2C总线连接*************************************/

#include<Function.h>

//AT2402的功能函数
/*******************************************************************
                    向有子地址器件发送多字节数据函数               
函数原型: bit  ISendStr(UCHAR sla,UCHAR suba,ucahr *s,UCHAR no);  
功能:     从启动总线到发送地址，子地址,数据，结束总线的全过程,从器件
          地址sla，子地址suba，发送内容是s指向的内容，发送no个字节。
          如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
bit ISendStr(unsigned char  sla,unsigned char  suba,unsigned char  *s,unsigned char  no)
{
   unsigned char i;

   Start_I2c();               /*启动总线*/
   SendByte(sla);             /*发送器件地址*/
   if(ack==0)return(0);
   SendByte(suba);            /*发送器件子地址*/
   if(ack==0)return(0);

   for(i=0;i<no;i++)
   {   
     SendByte(*s);            /*发送数据*/
     if(ack==0)return(0);
     s++;
   } 
   Stop_I2c();                /*结束总线*/ 

   return(1);
}

/*******************************************************************
                    向有子地址器件读取多字节数据函数               
函数原型: bit  RecndStr(UCHAR sla,UCHAR suba,ucahr *s,UCHAR no);  
功能:     从启动总线到发送地址，子地址,读数据，结束总线的全过程,从器件
          地址sla，子地址suba，读出的内容放入s指向的存储区，读no个字节。
           如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
bit IRcvStr(unsigned char  sla,unsigned char  suba,unsigned char  *s,unsigned char  no)
{
   unsigned char i;

   Start_I2c();                  /*启动总线*/
   SendByte(sla);                /*发送器件地址*/
   if(ack==0)return(0);
   SendByte(suba);               /*发送器件子地址*/
   if(ack==0)return(0);

   Start_I2c();                 /*重新启动总线*/
   SendByte(sla+1);
   if(ack==0)return(0);
   for(i=0;i<no-1;i++)
   {   
     *s=RcvByte();               /*发送数据*/
      Ack_I2c(0);                /*发送就答位*/  
     s++;
   } 
   *s=RcvByte();
   Ack_I2c(1);                   /*发送非应位*/
   Stop_I2c();                   /*结束总线*/ 
   return(1);
}



