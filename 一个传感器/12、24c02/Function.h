
/****h函数声明部分***********/

extern bit ack;

//起动总线函数
extern void Start_I2c();

//结束总线函数  
extern void Stop_I2c();

//应答子函数
extern void Ack_I2c(bit a);

//字节数据发送函数
extern void  SendByte(unsigned char  c);

//有子地址发送多字节数据函数               
extern bit ISendStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no) ;

//无子地址发送多字节数据函数   
extern bit ISendStrExt(unsigned char sla,unsigned char *s,unsigned char no);

//无子地址读字节数据函数               
extern unsigned char RcvByte();

//有子地址读取多字节数据函数               
extern bit IRcvStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no);

//无子地址读取多字节数据函数               
extern bit IRcvStrExt(unsigned char sla,unsigned char *s,unsigned char no);

extern void HC595SendData(unsigned int SendVal);

extern unsigned long ReHC74165(void);


