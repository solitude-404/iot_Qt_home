#include "DHT11.h"



uint8_t Humi_H,Humi_L,Temp_H,Temp_L;

void Delay30us(void)
{
	unsigned char data i;

	i = 12;
	while (--i);
}

void Delay40us(void)
{
	unsigned char data i;

//	_nop_();
	i = 17;
	while (--i);
}

uint8_t DHT11_Init(void)
{
	DHT11_DQ=1;
	DHT11_DQ=0;
	Delay_ms(20);
	DHT11_DQ=1;
	Delay30us();		//Delay 30us
	if(DHT11_DQ==0)
	{
		while(DHT11_DQ==0);			//等待响应信号完成（释放总线）
		while(DHT11_DQ==1);			//等待开始传输数据
	
		return 0;
	}
	return 1;
}


uint8_t DHT11_ReceiveByte(void)
{
	uint8_t i,Byte=0x00;
	for (i=0;i<8;i++) 
	{
		Byte<<=1; //Byte左移一位
		while(DHT11_DQ==0);	     //从1bit开始，低电平变高电平，等待低电平结束
		Delay40us();
		if(DHT11_DQ==1)
		{
			Byte|=0x01;
		}
		while(DHT11_DQ==1);  //等待高电平结束
	}						    
	return Byte;

}

/**
  * @brief  DHT11获取数据        
  * @param  None
  * @retval 返回值为uint8_t 接收到的字节数据  
  *			
  */
uint8_t DHT11_GetDate(uint8_t *Humi_H,uint8_t *Humi_L,uint8_t *Temp_H,uint8_t *Temp_L)
{
	uint8_t i,Buffer[5];
	if(DHT11_Init()==0)
	{
		
		for(i=0;i<5;i++)
		{
			Buffer[i]=DHT11_ReceiveByte();
			if(i==4)Delay40us();     //Delay 40us
				
		}
		if(Buffer[0]+Buffer[1]+Buffer[2]+Buffer[3]==Buffer[4])
		{
			*Humi_H=Buffer[0];
			*Humi_L=Buffer[1];
			*Temp_H=Buffer[2];	
			*Temp_L=Buffer[3];			
		}
	}
	else return 1;
	return 0;

}

/*
代码示例
while(DHT11_Init()==1);

DHT11_GetDate(&Humi_H,&Humi_L,&Temp_H,&Temp_L);
*/
