
#ifndef _AT24C02_H_
#define _AT24C02_H_

/**********************************
包含头文件
**********************************/
#include <REGX52.H>
#include "TypeDef.h"

/**********************************
PIN口定义
**********************************/
sbit AT24C02_SCL = P2^1;  // AT24C02时钟引脚
sbit AT24C02_SDA = P2^0;  // AT24C02数据引脚

/**********************************
函数声明
**********************************/
void At24c02_Init(void);  // At24c02初始化函数
void At24c02_Write_Add(unsigned char address, unsigned char info);  // At24c02写入数据函数
unsigned char At24c02_Read_Add(unsigned char address);  // At24c02读取数据函数

#endif