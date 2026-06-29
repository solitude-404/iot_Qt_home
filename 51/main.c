#include <REGX52.H>
#include <stdio.h>
#include "Delay.h"
#include "DHT11.h"
#include "UART.h"
#include "at24c02.h"
#include "oled.h" 


sbit Beep = P2^3;

// 在main.c文件开头添加继电器引脚定义
sbit HEATER_RELAY = P1^5;  // 加热继电器连接P1.5
sbit COOLER_RELAY = P1^4;  // 降温继电器连接P1.4

// ================== 新增 ==================
bit AUTO_MODE = 1;     // 1:自动  0:手动
// =========================================
beep_flag = 0;

unsigned char uart_command = 0;
bit uart_cmd_received = 0;
unsigned char S = 0;
unsigned char saveCount = 0;
unsigned char historyCount = 0;
unsigned int T_MAX = 31;
unsigned int R_MAX = 99;
unsigned int T_MIN = 29;
unsigned int R_MIN = 0;
unsigned char historyIndex = 0;
unsigned char displayMode = 0;

void Display_History(void);
void Display_RealTime(void);
void Save_Data(void);
void ProcessUARTCommand(void);
void Display_UART(void);
//void SoftTurnOffRelay(bit relay);


void main()
{
    // 初始化各模块
    OLED_Init();  // 使用OLED初始化替代LCD_Init()
    UART_Init();
	 // 初始化蜂鸣器引脚为高电平（不响）
    Beep = 1;
		HEATER_RELAY = 1;
    COOLER_RELAY = 1;
    //Test_EEPROM();
    // 尝试初始化DHT11
    while(DHT11_Init() == 1)
    {
        OLED_ShowString(0, 0, "Loading...", 16);  // 使用OLED显示错误信息
        Delay_ms(500);
        OLED_Clear();  // 使用OLED清屏
    }
    
    // 从AT24C02读取历史数据计数
    historyCount = At24c02_Read_Add(0);
    if(historyCount > 50) historyCount = 0;  // 如果数据异常，重置
					
					// 显示初始信息
					// 显示阈值信息
			UART_SendString("Real-time Monitoring Started\r\n");
			Display_RealTime();
    
    
    while(1)
    {
				ProcessUARTCommand();
        // 读取温湿度数据

        if(DHT11_GetDate(&Humi_H, &Humi_L, &Temp_H, &Temp_L) == 0)
        {
											 // 计算完整温湿度值（整数+小数，假设小数部分为1位）
						float temp = Temp_H + Temp_L / 10.0f;  // 完整温度值
						float humi = Humi_H + Humi_L / 10.0f;  // 完整湿度值
			
				//	if(AUTO_MODE)
          //  {
						// 结合整数和小数部分判断是否超出阈值
						if(temp > T_MAX || temp < T_MIN || humi > R_MAX || humi < R_MIN)
						{
								// 触发蜂鸣器报警
							if(beep_flag)
							{
								Beep = 0; // 低电平触发蜂鸣器
							}
							else
							{
								Beep = 1;
							}
								// 在OLED上显示警告
								//_______
							
							
								OLED_Clear();
    
							// 显示警告标题
								OLED_ShowString(30, 2, "Warning!", 16);       // 显示单位 'C' (位置: x=66, y=2行)
										 // 继电器控制逻辑
							if(temp < T_MIN)
							{
									// 温度过低，开启加热器，关闭冷却器
									HEATER_RELAY = 0;  // 开启加热
									COOLER_RELAY = 1;  // 关闭冷却
									 
									UART_SendString("Heater ON - Temperature too low\r\n");
								Display_UART();
								
							}
							else if(temp > T_MAX)
							{
									// 温度过高，开启冷却器，关闭加热器
									HEATER_RELAY = 1;  // 关闭加热
									COOLER_RELAY = 0;  // 开启冷却
									
									UART_SendString("Cooler ON - Temperature too high\r\n");
								Display_UART();
							}
							/*else
							{
								// 温度正常范围内，关闭加热和冷却
									HEATER_RELAY = 1;  // 关闭加热
									COOLER_RELAY = 1;  // 关闭冷却
								
									UART_SendString("Temperature normal - Relays OFF\r\n");
							}*/
        
							if(Humi_H > R_MAX) 
								{
									Display_UART();
							UART_SendString("Humidity Too High! ");
									
								}
							if(Humi_H < R_MIN)
							{
							Display_UART();
							UART_SendString("Humidity Too Low! ");
							}
							UART_SendString("\r\n");
												
							//--------
						}
						else
						{
										
								// 数据正常，关闭蜂鸣器
								Beep = 1;
													 
								HEATER_RELAY = 1;  // 关闭加热
								COOLER_RELAY = 1;  // 关闭冷却（风扇停止）
								UART_SendString("Temperature normal - Relays OFF\r\n");
								// 根据显示模式显示数据
								if(displayMode == 0)
								{
										
										Display_RealTime();
								}
								else
								{
										
										Display_History();
								}
						}/* ===== 手动模式 ===== */
				/*	}
						 else
                
            {

                UART_SendString("MANUAL MODE\r\n");
            }

						*/
						// 保存数据到AT24C02（每10次循环保存一次，避免过于频繁）
						if(saveCount++ >= 10)
						{
								Save_Data();
								saveCount = 0;
						}
        }
        
        
        // 延时
        Delay_ms(200);
    }
}



// 保存数据到AT24C02
void Save_Data(void)
{
    // 计算存储地址（每个数据占用4字节：温度高、温度低、湿度高、湿度低）
    unsigned char address = 1 + historyCount * 4;
    
    // 存储数据
    At24c02_Write_Add(address, Temp_H);
    Delay_ms(10);
    At24c02_Write_Add(address + 1, Temp_L);
    Delay_ms(10);
    At24c02_Write_Add(address + 2, Humi_H);
    Delay_ms(10);
    At24c02_Write_Add(address + 3, Humi_L);
    Delay_ms(10);
    
    // 更新历史数据计数（最多存储50组数据）
    historyCount++;
    if(historyCount >= 50) historyCount = 0;
    
    // 保存历史数据计数到地址0
    At24c02_Write_Add(0, historyCount);
    Delay_ms(10);
}


//串口发送实时数据
void Display_UART(void)
{
	 // 串口发送实时数据
    UART_SendString("RealTime - T:");
    UART_SendNum(Temp_H);
    UART_SendByte('.');
    UART_SendNum(Temp_L);
    UART_SendString("C H:");
    UART_SendNum(Humi_H);
    UART_SendByte('.');
    UART_SendNum(Humi_L);
    UART_SendString("%\r\n");
}	

// 显示实时数据
void Display_RealTime(void)
{
   	// 显示阈值信息
			OLED_ShowChinese(96, 0, 4, 16);  // 显示中文 "阈" (位置: x=96, y=0)
			OLED_ShowChinese(112, 0, 5, 16); // 显示中文 "值" (位置: x=112, y=0)
			OLED_ShowChar(72, 2, ' ', 16);       // 显示小数点 (位置: x=48, y=2行)
			OLED_ShowChar(76, 2, ' ', 16);       // 显示小数点 (位置: x=48, y=2行)
			OLED_ShowNum(84, 2, T_MIN, 2, 16);  // 显示温度阈值 (位置: x=96, y=2行)
			OLED_ShowNum(84, 4, R_MIN, 2, 16);  // 显示湿度阈值 (位置: x=96, y=4行)
			OLED_ShowNum(108, 2, T_MAX, 2, 16);  // 显示温度阈值 (位置: x=96, y=2行)
			OLED_ShowNum(108, 4, R_MAX, 2, 16);  // 显示湿度阈值 (位置: x=96, y=4行)

			// 显示实时标识
			OLED_ShowChinese(4, 0, 6, 16);   // 显示中文 "实" (位置: x=0, y=0)
			OLED_ShowChinese(20, 0, 7, 16);  // 显示中文 "时" (位置: x=16, y=0)

			// 显示温度
			OLED_ShowChinese(0, 2, 0, 16);   // 显示中文 "温" (位置: x=0, y=2行)
			OLED_ShowChinese(16, 2, 1, 16);  // 显示中文 "度" (位置: x=16, y=2行)
			OLED_ShowNum(32, 2, Temp_H, 2, 16);  // 显示温度整数部分 (位置: x=32, y=2行)
			OLED_ShowChar(48, 2, '.', 16);       // 显示小数点 (位置: x=48, y=2行)
			OLED_ShowNum(50, 2, Temp_L, 2, 16);  // 显示温度小数部分 (紧挨小数点后，位置: x=50, y=2行)
			OLED_ShowChar(66, 2, 'C', 16);       // 显示单位 'C' (位置: x=66, y=2行)

			// 显示湿度
			OLED_ShowChinese(0, 4, 2, 16);   // 显示中文 "湿" (位置: x=0, y=4行)
			OLED_ShowChinese(16, 4, 3, 16);  // 显示中文 "度" (位置: x=16, y=4行)
			OLED_ShowNum(32, 4, Humi_H, 2, 16);  // 显示湿度整数部分 (位置: x=32, y=4行)
			OLED_ShowChar(48, 4, '.', 16);       // 显示小数点 (位置: x=48, y=4行)
			OLED_ShowNum(50, 4, Humi_L, 2, 16);  // 显示湿度小数部分 (紧挨小数点后，位置: x=50, y=4行)
			OLED_ShowChar(66, 4, '%', 16);       // 显示单位 '%' (位置: x=66, y=4行)
			Display_UART();
   
}

// 显示历史数据
void Display_History(void)
{
    // 计算存储地址
    unsigned char address = 1 + historyIndex * 4;
    
    // 读取历史数据
    unsigned char tempH = At24c02_Read_Add(address);
    unsigned char tempL = At24c02_Read_Add(address + 1);
    unsigned char humiH = At24c02_Read_Add(address + 2);
    unsigned char humiL = At24c02_Read_Add(address + 3);
		// 使用OLED显示历史数据
		// 历史标题向左移动
		OLED_ShowChinese(0, 0, 12, 16);  // 显示中文 "历" (位置: x=0, y=0)
		OLED_ShowChinese(16, 0, 13, 16);  // 显示中文 "史" (位置: x=16, y=0)

		// 在右上角显示索引信息
		OLED_ShowString(44, 0, "No:", 16);        // 显示 "No:" (位置: x=80, y=0)
		OLED_ShowNum(60, 0, historyIndex+1, 2, 16); // 显示当前索引 (位置: x=96, y=0)
		OLED_ShowString(76, 0, "/", 16);         // 显示 "/" (位置: x=112, y=0)
		OLED_ShowNum(84, 0, historyCount, 2, 16); // 显示总数 (位置: x=120, y=0)

		// 显示温度数据
		OLED_ShowChinese(0, 2, 0, 16);     // 显示中文 "温" (位置: x=0, y=2行)
		OLED_ShowChinese(16, 2, 1, 16);   // 显示中文 "度" (位置: x=16, y=2行)
		OLED_ShowNum(32, 2, tempH, 2, 16); // 显示温度整数部分 (位置: x=32, y=2行)
		OLED_ShowChar(48, 2, '.', 16);      // 显示小数点 (位置: x=48, y=2行)
		OLED_ShowNum(50, 2, tempL, 2, 16); // 显示温度小数部分 (位置: x=50, y=2行)
		OLED_ShowChar(66, 2, 'C', 16);      // 显示单位 'C' (位置: x=66, y=2行)

		// 显示湿度数据
		OLED_ShowChinese(0, 4, 2, 16);     // 显示中文 "湿" (位置: x=0, y=4行)
		OLED_ShowChinese(16, 4, 3, 16);    // 显示中文 "度" (位置: x=16, y=4行)
		OLED_ShowNum(32, 4, humiH, 2, 16); // 显示湿度整数部分 (位置: x=32, y=4行)
		OLED_ShowChar(48, 4, '.', 16);      // 显示小数点 (位置: x=48, y=4行)
		OLED_ShowNum(50, 4, humiL, 2, 16); // 显示湿度小数部分 (位置: x=50, y=4行)
		OLED_ShowChar(66, 4, '%', 16);      // 显示单位 '%' (位置: x=66, y=4行)
    // 串口发送历史数据
    UART_SendString("History ");
    UART_SendNum(historyIndex+1);
    UART_SendString("/");
    UART_SendNum(historyCount);
    UART_SendString(" - T:");
    UART_SendNum(tempH);
    UART_SendByte('.');
    UART_SendNum(tempL);
    UART_SendString("C H:");
    UART_SendNum(humiH);
    UART_SendByte('.');
    UART_SendNum(humiL);
    UART_SendString("%\r\n");
}

// 在主循环中处理命令的函数
void ProcessUARTCommand(void)
{
    if(!uart_cmd_received) return; // 没有新命令，直接返回
    
    // 清除标志位
    uart_cmd_received = 0;
    
    // 处理命令
    switch(uart_command)
    {
        case '1':  // 增加温度阈值上限
            if(T_MAX > 99) T_MAX = 99;
            else T_MAX++;
            // 统一格式：TH:温度上限,TL:温度下限
            UART_SendString("TH:");
            UART_SendNum(T_MAX);
            UART_SendString(",TL:");
            UART_SendNum(T_MIN);
            UART_SendString("\r\n");
            break;
            
        case '2':  // 减少温度阈值上限
            if(T_MAX > T_MIN) T_MAX--;
            UART_SendString("TH:");
            UART_SendNum(T_MAX);
            UART_SendString(",TL:");
            UART_SendNum(T_MIN);
            UART_SendString("\r\n");
            break;
            
        case '3':  // 增加湿度阈值上限
            if(R_MAX >= 99) R_MAX = 99;
            else R_MAX++;
            UART_SendString("RH:");
            UART_SendNum(R_MAX);
            UART_SendString(",RL:");
            UART_SendNum(R_MIN);
            UART_SendString("\r\n");
            break;
            
        case '4':  // 减少湿度阈值上限
            if(R_MAX > R_MIN) R_MAX--;
            UART_SendString("RH:");
            UART_SendNum(R_MAX);
            UART_SendString(",RL:");
            UART_SendNum(R_MIN);
            UART_SendString("\r\n");
            break;
            
        case '5': 
            // 切换显示模式
            OLED_Clear();  // 使用OLED清屏
            displayMode = !displayMode;
            if(displayMode == 1)
            {
                UART_SendString("Switched to History Mode\r\n");
            }
            else
            {
                UART_SendString("Switched to Real-time Mode\r\n");
            }
            break;
            
        case '6':  // 上一个历史数据
            if(displayMode == 1)  // 仅在历史模式下有效
            {
                if(historyIndex > 0)
                    historyIndex--;
                else
                    historyIndex = historyCount - 1;
                UART_SendString("Previous history data\r\n");
            }
            break;
            
        case '7':  // 下一个历史数据
            if(displayMode == 1)  // 仅在历史模式下有效
            {
                if(historyIndex < historyCount - 1)
                    historyIndex++;
                else
                    historyIndex = 0;
                UART_SendString("Next history data\r\n");
            }
            break;
            
        case '8':  // 发送所有历史数据到串口
            UART_SendHistory();
            break;
            
        case '9':  // 重置系统
            historyCount = 0;
            At24c02_Write_Add(0, 0);  // 重置历史数据计数
            UART_SendString("System reset. History data cleared.\r\n");
            break;
            
        case 'a':  // 增加温度阈值下限
            if(T_MAX > T_MIN) T_MIN++;
            UART_SendString("TH:");
            UART_SendNum(T_MAX);
            UART_SendString(",TL:");
            UART_SendNum(T_MIN);
            UART_SendString("\r\n");
            break;
            
        case 'b':  // 减少温度阈值下限
            if(T_MIN > 0) T_MIN--;
            UART_SendString("TH:");
            UART_SendNum(T_MAX);
            UART_SendString(",TL:");
            UART_SendNum(T_MIN);
            UART_SendString("\r\n");
            break;
            
        case 'c':  // 增加湿度阈值下限
            if(R_MAX > R_MIN) R_MIN++;
            UART_SendString("RH:");
            UART_SendNum(R_MAX);
            UART_SendString(",RL:");
            UART_SendNum(R_MIN);
            UART_SendString("\r\n");
            break;
            
        case 'd':  // 减少湿度阈值
            if(R_MIN > 0) R_MIN--;
            UART_SendString("RH:");
            UART_SendNum(R_MAX);
            UART_SendString(",RL:");
            UART_SendNum(R_MIN);
            UART_SendString("\r\n");
            break;
        case 'e':
						beep_flag = !beep_flag;
						if(beep_flag)
						{
							UART_SendString("Alarm ON\r\n");
						}	
						else
						{
							UART_SendString("Alarm OFF\r\n");
							Beep = 1; // 关闭警报时强制关闭蜂鸣器
						}
       /* case 'e':   // 自动 / 手动切换
						SoftTurnOffRelay(HEATER_RELAY);
						SoftTurnOffRelay(COOLER_RELAY);

            AUTO_MODE = !AUTO_MODE;
            UART_SendString(AUTO_MODE ? "AUTO MODE\r\n" : "MANUAL MODE\r\n");
            break;*/

       /* case 'f':   // 手动开加热
            if(!AUTO_MODE){ HEATER_RELAY=0; COOLER_RELAY=1; }
            break;

        case 'g':   // 手动开制冷
            if(!AUTO_MODE){ HEATER_RELAY=1; COOLER_RELAY=0; }
            break;

        case 'h':   // 手动全关
            if(!AUTO_MODE){ HEATER_RELAY=1; COOLER_RELAY=1; }
            break;
*/
            
        default:
            // 未知指令
            UART_SendString("Unknown command. Available commands: 1-9, a-d\r\n");
            UART_SendString("1: T+  2: T-  3: H+  4: H-  5: Mode\r\n");
            UART_SendString("6: Prev 7: Next 8: Send All 9: Reset\r\n");
            UART_SendString("a: T_min+  b: T_min-  c: H_min+  d: H_min-\r\n");
					//	UART_SendString("e: auto/manual  f: heater  g: cooler  h: all\r\n");
            break;
    }
}

