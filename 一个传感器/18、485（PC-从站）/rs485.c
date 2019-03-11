 /*******************************************************************************************************
*  标题:                  RS485串口通信试验(接受与发送)                                                 *
*									                              	*
*  1.通过本例程了解串口的基本原理及使用 ,理解并掌握对串口进行初始化	                              	*
*													*
*  2.请使用串口调试助手（Baud 9600、数据位8、停止位1、效验位无）做为上位机(发送)数据和(接受)数据，	*
*    请在字符串输入框中输入字符和数字(HEX),按发送按钮。观看接受窗显示情况.也可以用自动循环发送。 	*
*	                                                                                                *
*  3.试验RS485需要用RS232转485的转换头。开板A对485转换头A  B对应485转换头B.接法大家可以参考原理图。	*
********************************************************************************************************/

#include <reg52.h>
#include <intrins.h>
 
sbit RS485_DIR = P1^7;  //RS485方向选择引脚

bit flagOnceTxd = 0;  //单次发送完成标志，即发送完一个字节
 
bit cmdArrived = 0;   //命令到达标志，即接收到上位机下发的命令

unsigned char cntRxd = 0;

unsigned char pdata bufRxd[40]; //串口接收缓冲区

  
void ConfigUART(unsigned int baud)  //串口配置函数，baud为波特率
{
    RS485_DIR = 0; //RS485设置为接收方向
    SCON = 0x50;   //配置串口为模式1
    TMOD &= 0x0F;  //清零T1的控制位
    TMOD |= 0x20;  //配置T1为模式2
    TH1 = 256 - (11059200/12/32) / baud;  //计算T1重载值
    TL1 = TH1;     //初值等于重载值
    ET1 = 0;       //禁止T1中断
    ES  = 1;       //使能串口中断
    TR1 = 1;       //启动T1
}

unsigned char UartRead(unsigned char *buf, unsigned char len) //串口数据读取函数，数据接收指针buf，读取数据长度len，返回值为实际读取到的数据长度
{
    unsigned char i;
    if (len > cntRxd) //读取长度大于接收到的数据长度时，
    {
        len = cntRxd; //读取长度设置为实际接收到的数据长度
    }
     for (i=0; i<len; i++) //拷贝接收到的数据
     {
        *buf = bufRxd[ i];
        buf++;
    }
    cntRxd = 0;  //清零接收计数器
    
    return len;  //返回实际读取长度
}

void DelayX10us(unsigned char t)  //软件延时函数，延时时间(t*10)us
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

void UartWrite(unsigned char *buf, unsigned char len) //串口数据写入函数，即串口发送函数，待发送数据指针buf，数据长度len
 {
    RS485_DIR = 1;  //RS485设置为发送
     while (len--)   //发送数据
     {
        flagOnceTxd = 0;
        SBUF = *buf;
        buf++;
        while (!flagOnceTxd);
    }
    DelayX10us(5);  //等待最后的停止位完成，延时时间由波特率决定
    RS485_DIR = 0;  //RS485设置为接收
}



void UartDriver() //串口驱动函数，检测接收到的命令并执行相应动作
{
    unsigned char len;
    unsigned char buf[30];
    if (cmdArrived) //有命令到达时，读取处理该命令
    {
        cmdArrived = 0;
        len = UartRead(buf, sizeof(buf)-2); //将接收到的命令读取到缓冲区中
       // buf[len++] = '\r';   //在接收到的数据帧后添加换车换行符后发回
        //buf[len++] = '\n';
        UartWrite(buf, len);
     }
}



void UartRxMonitor(unsigned char ms)  //串口接收监控函数
{
    static unsigned char cntbkp = 0;
    static unsigned char idletmr = 0;
    if (cntRxd > 0)  //接收计数器大于零时，监控总线空闲时间
    {
        if (cntbkp != cntRxd)  //接收计数器改变，即刚接收到数据时，清零空闲计时
        {
            cntbkp = cntRxd;
             idletmr = 0;
        }
        else
        {
            if (idletmr < 30)  //接收计数器未改变，即总线空闲时，累积空闲时间
            {
                idletmr += ms;
                if (idletmr >= 30)  //空闲时间超过30ms即认为一帧命令接收完毕
                {
                    cmdArrived = 1; //设置命令到达标志
                 }
            }
        }
    }
    else
    {
        cntbkp = 0;
    }
}

void InterruptUART() interrupt 4  //UART中断服务函数
{
    if (RI)  //接收到字节
    {
        RI = 0;   //手动清零接收中断标志位
        if (cntRxd < sizeof(bufRxd)) //接收缓冲区尚未用完时，
          {
               bufRxd[cntRxd++] = SBUF; //保存接收字节，并递增计数器
         }
    }
    if (TI)  //字节发送完毕
    {
                TI = 0;   //手动清零发送中断标志位
        flagOnceTxd = 1;  //设置单次发送完成标志
    }

}

/***********************main.c文件程序源代码*************************/

#include <reg52.h>

unsigned char T0RH = 0;  //T0重载值的高字节
unsigned char T0RL = 0;  //T0重载值的低字节

void ConfigTimer0(unsigned int ms);
extern void ConfigUART(unsigned int baud);
extern void UartRxMonitor(unsigned char ms);
extern void UartDriver();

void main ()
{
    EA = 1;           //开总中断
    ConfigTimer0(1);  //配置T0定时1ms
    ConfigUART(9600); //配置波特率为9600
    while(1)
    {
        UartDriver();
    }
}



void ConfigTimer0(unsigned int ms)  //T0配置函数
{
    unsigned long tmp;

    tmp = 11059200 / 12;      //定时器计数频率
    tmp = (tmp * ms) / 1000;  //计算所需的计数值
    tmp = 65536 - tmp;        //计算定时器重载值
    tmp = tmp + 34;           //修正中断响应延时造成的误差

    T0RH = (unsigned char)(tmp >> 8);  //定时器重载值拆分为高低字节
    T0RL = (unsigned char)tmp;
    TMOD &= 0xF0;   //清零T0的控制位
    TMOD |= 0x01;   //配置T0为模式1
    TH0 = T0RH;     //加载T0重载值
    TL0 = T0RL;
    ET0 = 1;        //使能T0中断
    TR0 = 1;        //启动T0
}

void InterruptTimer0() interrupt 1  //T0中断服务函数
{
    TH0 = T0RH;  //定时器重新加载重载值
    TL0 = T0RL;
    UartRxMonitor(1);  //串口接收监控
}