#ifndef __DHT11_H__
#define __DHT11_H__

#include <REGX52.H>
#include <intrins.h>
#include "Delay.h"
#include "TypeDef.h"  // 添加这行
// 引脚定义
sbit DHT11_DQ = P3^6;


//typedef  unsigned char  uint8_t;

extern uint8_t Humi_H,Humi_L,Temp_H,Temp_L;

uint8_t DHT11_Init(void);
uint8_t DHT11_ReceiveByte(void);
uint8_t DHT11_GetDate(uint8_t *Humi_H,uint8_t *Humi_L,uint8_t *Temp_H,uint8_t *Temp_L);




#endif
