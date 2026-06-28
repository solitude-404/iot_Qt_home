#include "oled.h"
#include "oledWENZI.h"  
#include "TypeDef.h"
#include "Delay.h"
#include "UART.h"
#include "AT24C02.h"



 //延时
void IIC_delay(void)
{
	u8 t=1;
	while(t--);
}
 //起始信号
void I2C_Start(void)
{
	OLED_SDA_Set();
	OLED_SCL_Set();
	IIC_delay();
	OLED_SDA_Clr();
	IIC_delay();
	OLED_SCL_Clr();	 
}
//结束信号
void I2C_Stop(void)
{
	OLED_SDA_Clr();
	OLED_SCL_Set();
	IIC_delay();
	OLED_SDA_Set();
}
//等待信号响应
void I2C_WaitAck(void) 
{
	OLED_SDA_Set();
	IIC_delay();
	OLED_SCL_Set();
	IIC_delay();
	OLED_SCL_Clr();
	IIC_delay();
}
void Send_Byte(u8 dat) //写入一个字节
{
	u8 i;
	for(i=0;i<8;i++)
	{
		OLED_SCL_Clr();	//将时钟信号设置为低电平
		if(dat&0x80)   //将dat的8位从最高位依次写入
		{
			OLED_SDA_Set();
        }
		else
		{
			OLED_SDA_Clr();
        }
		IIC_delay();
		OLED_SCL_Set();
		IIC_delay();
		OLED_SCL_Clr();
		dat<<=1;
  }
}
void OLED_WR_Byte(u8 dat,u8 mode)
{
	I2C_Start();
	Send_Byte(0x78);
	I2C_WaitAck();
	if(mode){Send_Byte(0x40);}
  else{Send_Byte(0x00);}
	I2C_WaitAck();
	Send_Byte(dat);
	I2C_WaitAck();
	I2C_Stop();
}
void OLED_Set_Pos(u8 x, u8 y) //坐标设置
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD);
}     

void OLED_Clear(void) 	   //清屏函数
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);     
		OLED_WR_Byte (0x00,OLED_CMD);      
		OLED_WR_Byte (0x10,OLED_CMD);          
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	}  
}

void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 sizey)
{      	
	u8 c=0,sizex=sizey/2;
	u32 i=0,size1;
	if(sizey==8)size1=6;
	else size1=(sizey/8+((sizey%8)?1:0))*(sizey/2);
	c=chr-' ';  
	OLED_Set_Pos(x,y);
	for(i=0;i<size1;i++)
	{
		if(i%sizex==0&&sizey!=8) OLED_Set_Pos(x,y++);
		if(sizey==8) OLED_WR_Byte(asc2_0806[c][i],OLED_DATA); 
		else if(sizey==16) OLED_WR_Byte(asc2_1608[c][i],OLED_DATA);  
		else return;
	}
}

  /**
  求m的n次幂
  **/

u32 oled_pow(u8 m,u8 n)
{
	u16 result=1;	 
	while(n--)result*=m;    
	return result;
}	
/**
 prief 显示数字
 param  x  起始行位置 范围：0-127
 param	y  起始列位置 范围：0-7
 prarm num 要显示数字范围0~65535
            同列显示超出5位需要更改行位置
 prarm len 数字长度
 prarm sizey 要显示数字大小
 **/
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 sizey)
{         	
	u8 t,temp,m=0;
	u8 enshow=0;
	if(sizey==8)m=2;
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(sizey/2+m)*t,y,' ',sizey);
				continue;
			}else enshow=1;
		}
	 	OLED_ShowChar(x+(sizey/2+m)*t,y,temp+'0',sizey);
	}
}


 /**
 prief 显示一个字符串  以8个为最小单位
 param  x  起始行位置 范围：0-127
 param	y  起始列位置 范围：0-7
 prarm *chr 要显示的字符串，可显示字模库在oledfont.h文件
 prarm sizex 要显示字体大小
 **/
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 sizey)
{
	u8 j=0;
	while (chr[j]!='\0')
	{		
		OLED_ShowChar(x,y,chr[j++],sizey);
		if(sizey==8)x+=6;
		else x+=sizey/2;
	}
}
/**
 prief 显示汉字
 param  x  起始行位置 范围：0-127
 param	y  起始列位置 范围：0-7
 prarm no 要显示的汉字排序号，0是第一个，1是第二个。。。字模库在oledfont.h文件中
 prarm sizey 要显示字体的大小
 **/ 
void OLED_ShowChinese(u8 x,u8 y,u8 no,u8 sizey)
{
	u32 i,size1=(sizey/8+((sizey%8)?1:0))*sizey;
	for(i=0;i<size1;i++)
	{
		if(i%sizey==0) OLED_Set_Pos(x,y++);
		if(sizey==16) OLED_WR_Byte(Hzk[no][i],OLED_DATA);

		else return;
	}				
}
 
  /**
  oled初始化
  **/
void OLED_Init(void)
{	
	OLED_WR_Byte(0xAE,OLED_CMD);//关闭oled面板	
	OLED_WR_Byte(0x00,OLED_CMD);//设置低地址	
	OLED_WR_Byte(0x10,OLED_CMD);//设置高地址		
	OLED_WR_Byte(0x40,OLED_CMD);//设置起始地址  设置映射RAM显示起始行（0x00-0x3F）
	OLED_WR_Byte(0x81,OLED_CMD);//设置对比度控制寄存器
	OLED_WR_Byte(0xCF,OLED_CMD);//设置SEG输出电流高度	
	OLED_WR_Byte(0xA1,OLED_CMD);//	
	OLED_WR_Byte(0xC8,OLED_CMD);//	
	OLED_WR_Byte(0xA6,OLED_CMD);//	
	OLED_WR_Byte(0xA8,OLED_CMD);//	
	OLED_WR_Byte(0x3f,OLED_CMD);//	
	OLED_WR_Byte(0xD3,OLED_CMD);//	
	OLED_WR_Byte(0x00,OLED_CMD);//	
	OLED_WR_Byte(0xd5,OLED_CMD);//	 
	OLED_WR_Byte(0x80,OLED_CMD);//
	OLED_WR_Byte(0xD9,OLED_CMD);// 
	OLED_WR_Byte(0xF1,OLED_CMD);// 
	OLED_WR_Byte(0xDA,OLED_CMD);//	 
	OLED_WR_Byte(0x12,OLED_CMD);//  
	OLED_WR_Byte(0xDB,OLED_CMD);//	 
	OLED_WR_Byte(0x40,OLED_CMD);//	 
	OLED_WR_Byte(0x20,OLED_CMD);// 
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);// 
	OLED_WR_Byte(0x14,OLED_CMD);//	 
	OLED_WR_Byte(0xA4,OLED_CMD);// 
	OLED_WR_Byte(0xA6,OLED_CMD);// 
	OLED_Clear();               	 
	OLED_WR_Byte(0xAF,OLED_CMD); 	 
}
