#include "DS1302.h"
#include "delay.h"
///*********************************************************************  
//*
//* 函 数 名: Get_HzMat
//* 功能描述: 
//* 函数说明: 
//* 调用函数: 
//* 全局变量: 无
//* 输    入: 无
//* 返    回: 无
//* 设 计 者：          日期：
//* 修 改 者：       		日期：
//* 版    本：VER 1.0
//***********************************************************************/
//void Get_HzMat(char *codes, uint8_t *mat, uint8_t size)
//{
//	uint8_t qh,ql;
//	uint8_t i;					  
//	uint32_t foffset;
//	
//	qh = *codes;
//	ql = *(++codes);
//	
//	if(qh<0x81 || ql<0x40 || ql==0xff || qh==0xff)//非 常用汉字
//	{   		    
//		for(i=0;i<(size*2);i++)
//		{
//			*mat ++= 0x00;//填充满格
//		}
//		
//		return; //结束访问
//	}
//	
//	if(ql < 0x7f)
//	{
//		ql -= 0x40;//注意!
//	}else 
//	{
//		ql -= 0x41;//A2
//	}
//	
//	qh -= 0x81;//43
//	foffset = ((uint32_t)190 * qh + ql) * (size * 2);//得到字库中的字节偏移量  		  412544=0x64B80

//	if(size == 16)
//	{
//		foffset = foffset + ftinfo.f16addr;
//		SPI_Flash_Read(mat, foffset, 32); //458670
//	}else if(size == 12)
//	{
//		SPI_Flash_Read(mat,foffset + ftinfo.f12addr,24);
//	}
//}
///*********************************************************************  
//*
//* 函 数 名: 
//* 功能描述: 
//* 函数说明: 
//* 调用函数: 
//* 全局变量: 无
//* 输    入: 无
//* 返    回: 无
//* 设 计 者：          日期：
//* 修 改 者：       		日期：
//* 版    本：VER 1.0
//***********************************************************************/
//void Show_Font(unsigned short x, unsigned short y, char *font, uint8_t size, unsigned short Color)
//{
//	uint8_t temp,t,t1;
//	uint8_t dzk[32];
//	unsigned short y0=y;
//	
//	if(size != 12&&size != 16) return;//不支持的size
//	Get_HzMat(font, dzk, size);//得到相应大小的点阵数据
//	
//									for(t = 0; t < size * 2; t++)
//									{
//										temp = dzk[t];//得到12数据                          
//										for(t1 = 0; t1 < 8; t1++)
//										{
//											if(temp&0x80)
//											{
//												LCD_SetCursor(x, y);
//												LCD_WR_Data(Color);
//											}
//											temp <<= 1;
//											y++;
//											if((y - y0) == size)
//											{
//												y = y0;
//												x++;
//												break;
//											}
//										}
//									}
//}
/*********************************************************************  
*
* 函 数 名: 
* 功能描述: 
* 函数说明: 
* 调用函数: 
* 全局变量: 无
* 输    入: 无
* 返    回: 无
* 设 计 者：          日期：
* 修 改 者：       		日期：
* 版    本：VER 1.0
***********************************************************************/
// void Show_Str(unsigned short x, unsigned short y, char *str, uint8_t size, unsigned short Color)
// {												  	  
// 	uint8_t bHz = 0;     //字符或者中文 
	
// 	while(*str != 0)//数据未结束
// 	{
// 		if(!bHz)
// 		{
// 			if((uint8_t)*str > 0x80) bHz=1;//中文 
// 			else              //字符
// 			{
// 				if(x>(MAX_CHAR_POSX - size/2))//换行
// 				{				   
// 					y+=size;
// 					x=0;	   
// 				}
				
// 				if(y>(MAX_CHAR_POSY - size)) break;//越界返回
				
// 				if(*str==13)//换行符号
// 				{         
// 					y+=size;
// 					x=0;
// 					str++; 
// 				}
				
// 				else LCD_ShowChar(x, y, *str, size, Color);//有效部分写入 
				
// 				str++;
// 				x+=size/2; //字符,为全字的一半 
// 			}
// 		}else//中文 
// 		{
// 			bHz=0;//有汉字库    
// 			if(x>(MAX_CHAR_POSX - size/2))//换行
// 			{	    
// 				y+=size;
// 				x=0;		  
// 			}
// 			if(y>(MAX_CHAR_POSY - size/2))break;//越界返回  						     
// 			Show_Font(x, y, str, size, Color); //显示这个汉字,空心显示 
// 			str+=2; 
// 			x+=size;//下一个汉字偏移	    
// 		}						 
// 	}   
// }
/*********************************************************************  
*
* 函 数 名: 
* 功能描述: 
* 函数说明: 
* 调用函数: 
* 全局变量: 无
* 输    入: 无
* 返    回: 无
* 设 计 者：          日期：
* 修 改 者：       		日期：
* 版    本：VER 1.0
***********************************************************************/
/////////////////////////END////////////////////////
//----------------------------------------
void write_byte(unsigned char dat)//写一个字节
{
	unsigned char ACC = dat, a;

	for(a=0;a<8;a++)
	{
		DS1302_IO = ACC & 0x01;
		_nop_();

		DS1302_CLK=1;
		_nop_();

		DS1302_CLK=0;
		_nop_();

		ACC = ACC >> 1;
	}
}

unsigned char read_byte(void) //读一个字节
{
	unsigned char temp = 0, a;
	
	for(a=0;a<8;a++)
	{
		temp = temp>>1;
		_nop_();

		if(DS1302_IO)
		{
			temp = temp|0x80;
		}
		_nop_();
		
		DS1302_CLK=1;
		_nop_();
		
		DS1302_CLK=0;
		_nop_();
	}

	return (temp);
}
void write_1302(unsigned char add, unsigned char dat) //向1302芯片写函数，指定写入地址，数据
{
	DS1302_RST=0;
	_nop_();

	DS1302_CLK=0;
	_nop_();
	
	DS1302_RST=1;
	_nop_();

	write_byte(add);

	write_byte(dat);

	DS1302_RST=0;
	_nop_();
}

unsigned char read_1302(unsigned char add) //从1302读数据函数，指定读取数据来源地址
{
	unsigned char temp;

	DS1302_RST=0;
	_nop_();
	
	DS1302_CLK=0;
	_nop_();

	DS1302_RST=1;
	_nop_();

	write_byte(add);

	temp = read_byte();

	DS1302_RST=0;
	_nop_();

	DS1302_IO = 0;
	_nop_();
	
	return (temp);
}

unsigned char BCD_Decimal(unsigned char bcd) //BCD码转十进制函数，输入BCD，返回十进制
{
	unsigned char Decimal;

	bcd = bcd & 0x7F;
	Decimal = bcd>>4;

	return (Decimal=Decimal*10+(bcd&=0x0F));
}

unsigned char Decimal_BCD(unsigned char Decimal) //十进制转BCD码函数，输入十进制，返回BCD
{
	unsigned char bcd;

	bcd = Decimal / 10;
	
	bcd = bcd << 4;
	bcd = bcd | (Decimal % 10);

	return bcd;
}


//--------------------------------------
 /*读取时间：*/
void read_rtc(RTC_time *g_rtc_time)
{
   g_rtc_time->sec = read_1302(0x81);
   g_rtc_time->min = read_1302(0x83);
   g_rtc_time->hour = read_1302(0x85); 
	
   g_rtc_time->week = read_1302(0x8b);
	
   g_rtc_time->year = read_1302(0x8d);
   g_rtc_time->month = read_1302(0x89);
   g_rtc_time->day = read_1302(0x87);
}

void ds1302_init(void) //1302芯片初始化子函数(2010-01-07,12:00:00,week4)
{
	unsigned char temp = 0;
	
	P04 = 0;
	temp = read_1302(0xC1);
	if(temp != 0x01) //
	{
		write_1302(0x8e,0x00); //允许写，禁止写保护

		write_1302(0x80,0x00); //向DS1302内写秒寄存器80H写入初始秒数据00

		write_1302(0x82,0x19); //向DS1302内写分寄存器82H写入初始分数据00

		write_1302(0x84,0x91); //向DS1302内写小时寄存器84H写入初始小时数据12 7bit为12小时模式，5bit为AM

		write_1302(0x8a,0x02); //向DS1302内写周寄存器8aH写入初始周数据4

		write_1302(0x86,0x23); //向DS1302内写日期寄存器86H写入初始日期数据07

		write_1302(0x88,0x05); //向DS1302内写月份寄存器88H写入初始月份数据01

		write_1302(0x8c,0x17); //向DS1302内写年份寄存器8cH写入初始年份数据10

		do{
			write_1302(0xC0,0x01); //
			temp = read_1302(0xC1);
		}while(temp != 0x01);
		
		write_1302(0x90,0xA5); //涓流充电
		
		write_1302(0x8e,0x80); //打开写保护
	}
}