#include "SPI.h"




/************************************************
SPI初始化
入口参数: 无
出口参数: 无
************************************************/
void InitSpi(void)
{
	NRF_CEH;
	NRF_CSH;
	NRF_CLKL;
	NRF_MOSIL;
	NRF_LEDL;
}

/************************************************
使用SPI方式与Flash进行数据交换
入口参数:
    dat : 准备写入的数据
出口参数:
    从Flash中读出的数据
************************************************/
static void delay_m(unsigned int ms)
{
	while(ms--);
}

unsigned char SPI_ReadWriteByte(unsigned char dat)
{
	unsigned char i, temp, res = 0;
	
	temp = dat;
	for(i = 0; i < 8; i++)
	{
		if(temp & 0x80)
		{
			NRF_MOSIH;
		}else
		{
			NRF_MOSIL;
		}
		NRF_CLKH;
		delay_m(5);
		
		res <<= 1;
		if(NRF_MISO)
		{
			res = res | 0x01;
		}
		
		NRF_CLKL;
		delay_m(5);
		
		temp <<= 1;
	}
	
	return res;
}
