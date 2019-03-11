 /*******************************************************************************************************
*  ����:                  RS485����ͨ������(�����뷢��)                                                 *
*									                              	*
*  1.ͨ���������˽⴮�ڵĻ���ԭ��ʹ�� ,��Ⲣ���նԴ��ڽ��г�ʼ��	                              	*
*													*
*  2.��ʹ�ô��ڵ������֣�Baud 9600������λ8��ֹͣλ1��Ч��λ�ޣ���Ϊ��λ��(����)���ݺ�(����)���ݣ�	*
*    �����ַ���������������ַ�������(HEX),�����Ͱ�ť���ۿ����ܴ���ʾ���.Ҳ�������Զ�ѭ�����͡� 	*
*	                                                                                                *
*  3.����RS485��Ҫ��RS232ת485��ת��ͷ������A��485ת��ͷA  B��Ӧ485ת��ͷB.�ӷ���ҿ��Բο�ԭ��ͼ��	*
********************************************************************************************************/

#include <reg52.h>
#include <intrins.h>
 
sbit RS485_DIR = P1^7;  //RS485����ѡ������

bit flagOnceTxd = 0;  //���η�����ɱ�־����������һ���ֽ�
 
bit cmdArrived = 0;   //������־�������յ���λ���·�������

unsigned char cntRxd = 0;

unsigned char pdata bufRxd[40]; //���ڽ��ջ�����

  
void ConfigUART(unsigned int baud)  //�������ú�����baudΪ������
{
    RS485_DIR = 0; //RS485����Ϊ���շ���
    SCON = 0x50;   //���ô���Ϊģʽ1
    TMOD &= 0x0F;  //����T1�Ŀ���λ
    TMOD |= 0x20;  //����T1Ϊģʽ2
    TH1 = 256 - (11059200/12/32) / baud;  //����T1����ֵ
    TL1 = TH1;     //��ֵ��������ֵ
    ET1 = 0;       //��ֹT1�ж�
    ES  = 1;       //ʹ�ܴ����ж�
    TR1 = 1;       //����T1
}

unsigned char UartRead(unsigned char *buf, unsigned char len) //�������ݶ�ȡ���������ݽ���ָ��buf����ȡ���ݳ���len������ֵΪʵ�ʶ�ȡ�������ݳ���
{
    unsigned char i;
    if (len > cntRxd) //��ȡ���ȴ��ڽ��յ������ݳ���ʱ��
    {
        len = cntRxd; //��ȡ��������Ϊʵ�ʽ��յ������ݳ���
    }
     for (i=0; i<len; i++) //�������յ�������
     {
        *buf = bufRxd[ i];
        buf++;
    }
    cntRxd = 0;  //������ռ�����
    
    return len;  //����ʵ�ʶ�ȡ����
}

void DelayX10us(unsigned char t)  //�����ʱ��������ʱʱ��(t*10)us
{
    do {
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        } while (--t);
 }

void UartWrite(unsigned char *buf, unsigned char len) //��������д�뺯���������ڷ��ͺ���������������ָ��buf�����ݳ���len
 {
    RS485_DIR = 1;  //RS485����Ϊ����
     while (len--)   //��������
     {
        flagOnceTxd = 0;
        SBUF = *buf;
        buf++;
        while (!flagOnceTxd);
    }
    DelayX10us(5);  //�ȴ�����ֹͣλ��ɣ���ʱʱ���ɲ����ʾ���
    RS485_DIR = 0;  //RS485����Ϊ����
}



void UartDriver() //�������������������յ������ִ����Ӧ����
{
    unsigned char len;
    unsigned char buf[30];
    if (cmdArrived) //�������ʱ����ȡ���������
    {
        cmdArrived = 0;
        len = UartRead(buf, sizeof(buf)-2); //�����յ��������ȡ����������
       // buf[len++] = '\r';   //�ڽ��յ�������֡����ӻ������з��󷢻�
        //buf[len++] = '\n';
        UartWrite(buf, len);
     }
}



void UartRxMonitor(unsigned char ms)  //���ڽ��ռ�غ���
{
    static unsigned char cntbkp = 0;
    static unsigned char idletmr = 0;
    if (cntRxd > 0)  //���ռ�����������ʱ��������߿���ʱ��
    {
        if (cntbkp != cntRxd)  //���ռ������ı䣬���ս��յ�����ʱ��������м�ʱ
        {
            cntbkp = cntRxd;
             idletmr = 0;
        }
        else
        {
            if (idletmr < 30)  //���ռ�����δ�ı䣬�����߿���ʱ���ۻ�����ʱ��
            {
                idletmr += ms;
                if (idletmr >= 30)  //����ʱ�䳬��30ms����Ϊһ֡����������
                {
                    cmdArrived = 1; //����������־
                 }
            }
        }
    }
    else
    {
        cntbkp = 0;
    }
}

void InterruptUART() interrupt 4  //UART�жϷ�����
{
    if (RI)  //���յ��ֽ�
    {
        RI = 0;   //�ֶ���������жϱ�־λ
        if (cntRxd < sizeof(bufRxd)) //���ջ�������δ����ʱ��
          {
               bufRxd[cntRxd++] = SBUF; //��������ֽڣ�������������
         }
    }
    if (TI)  //�ֽڷ������
    {
                TI = 0;   //�ֶ����㷢���жϱ�־λ
        flagOnceTxd = 1;  //���õ��η�����ɱ�־
    }

}

/***********************main.c�ļ�����Դ����*************************/

#include <reg52.h>

unsigned char T0RH = 0;  //T0����ֵ�ĸ��ֽ�
unsigned char T0RL = 0;  //T0����ֵ�ĵ��ֽ�

void ConfigTimer0(unsigned int ms);
extern void ConfigUART(unsigned int baud);
extern void UartRxMonitor(unsigned char ms);
extern void UartDriver();

void main ()
{
    EA = 1;           //�����ж�
    ConfigTimer0(1);  //����T0��ʱ1ms
    ConfigUART(9600); //���ò�����Ϊ9600
    while(1)
    {
        UartDriver();
    }
}



void ConfigTimer0(unsigned int ms)  //T0���ú���
{
    unsigned long tmp;

    tmp = 11059200 / 12;      //��ʱ������Ƶ��
    tmp = (tmp * ms) / 1000;  //��������ļ���ֵ
    tmp = 65536 - tmp;        //���㶨ʱ������ֵ
    tmp = tmp + 34;           //�����ж���Ӧ��ʱ��ɵ����

    T0RH = (unsigned char)(tmp >> 8);  //��ʱ������ֵ���Ϊ�ߵ��ֽ�
    T0RL = (unsigned char)tmp;
    TMOD &= 0xF0;   //����T0�Ŀ���λ
    TMOD |= 0x01;   //����T0Ϊģʽ1
    TH0 = T0RH;     //����T0����ֵ
    TL0 = T0RL;
    ET0 = 1;        //ʹ��T0�ж�
    TR0 = 1;        //����T0
}

void InterruptTimer0() interrupt 1  //T0�жϷ�����
{
    TH0 = T0RH;  //��ʱ�����¼�������ֵ
    TL0 = T0RL;
    UartRxMonitor(1);  //���ڽ��ռ��
}