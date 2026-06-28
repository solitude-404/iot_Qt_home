#include "at24c02.h"
#include "TypeDef.h"  // 确保包含类型定义头文件

/**********************************
函数定义
**********************************/

/****
*******AT24C02延时函数
*****/
void At24c02_Delay(void)
{
    unsigned char i;  // 使用 unsigned char 替代 uint8_t
    i = 10;
    while(i--);
}

/****
*******AT24C02延时函数
*****/
void At24c02_Delay1(unsigned char x)  // 参数改为 unsigned char
{
    unsigned int i;
    for(i = 0; i < x; i++);
}

/****
*******总线初始化,拉高释放总线函数
*****/
void At24c02_Init(void)
{
    AT24C02_SCL = 1;
    At24c02_Delay();
    AT24C02_SDA = 1;
    At24c02_Delay();
}

/****
*******开始信号，上升沿
*****/
void At24c02_Start(void)
{
    AT24C02_SDA = 1;
    //发送起始条件的数据信号
    At24c02_Delay();
    AT24C02_SCL = 1;
    //发送起始条件的时钟信号
    At24c02_Delay();
    AT24C02_SDA = 0;
    //发送起始信号
    At24c02_Delay();
    AT24C02_SCL = 0;
    //钳住IIC总线，准备发送或接收数据
    At24c02_Delay();
}

/****
*******结束信号，下降沿
*****/
void At24c02_Stop(void)
{
    AT24C02_SDA = 0;
    //发送结束条件的数据信号
    At24c02_Delay();
    AT24C02_SCL = 1;
    //发送结束条件的时钟信号
    At24c02_Delay();
    AT24C02_SDA = 1;
    //发送结束信号
    At24c02_Delay();
}

/****
*******IIC总线时钟或应答信号
*****/
void At24c02_Clock(void)
{
    unsigned char i = 0;  // 使用 unsigned char 替代 uint8_t
    AT24C02_SCL = 1;
    At24c02_Delay();
    while ((AT24C02_SDA == 1) && (i < 255))
        i++;
    AT24C02_SCL = 0;
    At24c02_Delay();
}

/****
*******写一个字节
*****/
void At24c02_Writex(unsigned char j)  // 参数改为 unsigned char
{
    unsigned char i, temp;  // 使用 unsigned char 替代 uint8_t
    temp = j;
    for (i = 0; i < 8; i++)
    {
        temp = temp << 1;
        AT24C02_SCL = 0;
        At24c02_Delay();
        AT24C02_SDA = CY;
        //最高位移入PSW寄存器CY位中
        At24c02_Delay();
        AT24C02_SCL = 1;
        At24c02_Delay();
    }
    AT24C02_SCL = 0;
    At24c02_Delay();
    AT24C02_SDA = 1;
    At24c02_Delay();
}

/****
*******读取一个字节
*****/
unsigned char At24c02_Readx(void)  // 返回类型改为 unsigned char
{
    unsigned char i, j, k = 0;  // 使用 unsigned char 替代 uint8_t
    AT24C02_SCL = 0;
    At24c02_Delay();
    AT24C02_SDA = 1;
    for (i = 0; i < 8; i++)
    {
        At24c02_Delay();
        AT24C02_SCL = 1;
        At24c02_Delay();
        if (AT24C02_SDA == 1)
            j = 1;
        else
            j = 0;
        k = (k << 1) | j;
        //将8个独立的位放在一个字节中
        AT24C02_SCL = 0;
    }
    At24c02_Delay();
    return(k);
}

/****
*******向AT24C02的地址address中写入一个字节的数据info
*****/
void At24c02_Write_Add(unsigned char address, unsigned char info)  // 参数改为 unsigned char
{
    At24c02_Start();
    At24c02_Writex(0xA0);
    At24c02_Clock();
    At24c02_Writex(address);
    At24c02_Clock();
    At24c02_Writex(info);
    At24c02_Clock();
    At24c02_Stop();
    At24c02_Delay1(50);
}

/****
*******从AT24C02的地址address中读取一个字节的数据
*****/
unsigned char At24c02_Read_Add(unsigned char address)  // 参数和返回类型改为 unsigned char
{
    unsigned char i;  // 使用 unsigned char 替代 uint8_t
    At24c02_Start();
    At24c02_Writex(0xA0);
    At24c02_Clock();
    At24c02_Writex(address);
    At24c02_Clock();
    At24c02_Start();
    At24c02_Writex(0xA1);
    At24c02_Clock();
    i = At24c02_Readx();
    At24c02_Stop();
    At24c02_Delay1(10);
    return(i);
}