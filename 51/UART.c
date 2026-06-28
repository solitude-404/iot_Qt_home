#include <REGX52.H>
#include "UART.h"
#include "AT24C02.h"
#include "Delay.h"
#include "oled.h"
// UART.c �ļ���ͷ����
extern unsigned char S;
extern unsigned char saveCount;
extern unsigned char historyCount;
extern unsigned int T_MAX;
extern unsigned int R_MAX;
extern unsigned int T_MIN;
extern unsigned int R_MIN;
extern unsigned char historyIndex;
extern unsigned char displayMode;
extern unsigned char uart_command;
extern bit uart_cmd_received;



// ���ڳ�ʼ��������������9600
void UART_Init(void)
{
     PCON &= 0x7F;		// ²¨ÌØÂÊ²»±¶ËÙ
    SCON = 0x50;		// 8Î»Êý¾Ý,¿É±ä²¨ÌØÂÊ
    TMOD &= 0x0F;		// Çå³ý¶¨Ê±Æ÷1Ä£Ê½Î»
    TMOD |= 0x20;		// Éè¶¨¶¨Ê±Æ÷1Îª8Î»×Ô¶¯ÖØ×°·½Ê½
    TL1 = 0xFD;			// Éè¶¨¶¨Ê±³õÖµ
    TH1 = 0xFD;			// Éè¶¨¶¨Ê±Æ÷ÖØ×°Öµ
    ET1 = 0;			// ½ûÖ¹¶¨Ê±Æ÷1ÖÐ¶Ï
    TR1 = 1;			// Æô¶¯¶¨Ê±Æ÷1
    ES = 1;				// ÔÊÐí´®¿ÚÖÐ¶Ï
    EA = 1;				// ¿ªÆô×ÜÖÐ¶Ï
}

// ���ڷ���һ���ֽ�
void UART_SendByte(uint8_t Byte)
{
    SBUF = Byte;
    while(TI == 0);
    TI = 0;
}

// ���ڷ����ַ���
void UART_SendString(char *str)
{
    while(*str != '\0')
    {
        UART_SendByte(*str);
        str++;
    }
}

// ���ڷ�������
void UART_SendNum(uint16_t num)
{
    uint8_t buf[5];
    uint8_t i = 0;
    
    if(num == 0)
    {
        UART_SendByte('0');
        return;
    }
    
    while(num)
    {
        buf[i++] = num % 10 + '0';
        num /= 10;
    }
    
    while(i)
    {
        UART_SendByte(buf[--i]);
    }
}

// �����жϷ�����
void UART_Routine(void) interrupt 4
{
   if(RI == 1)
    {
        
			 RI = 0;        // ��������жϱ�־λ
        uart_command = SBUF; // ������յ�������
        uart_cmd_received = 1; // ����������ձ�־
    
        
        // ��������жϱ�־λ������У�
        if(TI) {
            TI = 0;
        }
			}
}
		
		
//***************
		// ͨ�����ڷ���������ʷ����
void UART_SendHistory(void)
{
    unsigned char i;
    
    UART_SendString("====== All History Data ======\r\n");
    
    for(i = 0; i < historyCount; i++)
    {
        // ����洢��ַ
        unsigned char address = 1 + i * 4;
        
        // ��ȡ��ʷ����
        unsigned char tempH = At24c02_Read_Add(address);
        unsigned char tempL = At24c02_Read_Add(address + 1);
        unsigned char humiH = At24c02_Read_Add(address + 2);
        unsigned char humiL = At24c02_Read_Add(address + 3);
        
        // ��������
        UART_SendString("Data ");
        UART_SendNum(i+1);
        UART_SendString(": T=");
        UART_SendNum(tempH);
        UART_SendByte('.');
        UART_SendNum(tempL);
        UART_SendString("C, H=");
        UART_SendNum(humiH);
        UART_SendByte('.');
        UART_SendNum(humiL);
        UART_SendString("%\r\n");
        
        Delay_ms(10);  // ������ʱ�����⴮�ڶ���
    }
    
    UART_SendString("====== End of History Data ======\r\n");
}
