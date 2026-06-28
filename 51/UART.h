#ifndef __UART_H__
#define __UART_H__

#include <REGX52.H>
#include "TypeDef.h"  // 添加这行

// 移除这两行:
// typedef unsigned char uint8_t;
// typedef unsigned int uint16_t;


void UART_Init(void);
void UART_SendByte(uint8_t Byte);
void UART_SendString(char *str);
void UART_SendNum(uint16_t num);
void UART_SendHistory(void);

#endif