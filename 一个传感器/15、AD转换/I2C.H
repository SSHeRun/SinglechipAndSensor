extern bit ack;
//�����ߺ���
extern void Start_I2c();
//�������ߺ���  
extern void Stop_I2c();
//Ӧ���Ӻ���
extern void Ack_I2c(bit a);
//�ֽ����ݷ��ͺ���
extern void  SendByte(unsigned char  c);
//���ӵ�ַ���Ͷ��ֽ����ݺ���               
extern bit ISendStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no) ;
//���ӵ�ַ���Ͷ��ֽ����ݺ���   
extern bit ISendStrExt(unsigned char sla,unsigned char *s,unsigned char no);
//���ӵ�ַ���ֽ����ݺ���               
extern unsigned char RcvByte();

            
