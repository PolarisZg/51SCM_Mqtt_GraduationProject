#include "myKey.h"
#include "GPIO.h"

void myKeyInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//上拉输入
	GPIO_InitStructure.Mode = GPIO_PullUp;		//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	
	GPIO_InitStructure.Pin  = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;		//指定要初始化的IO, GPIO_Pin_2.3.4, Key0.1.2
	GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);	//初始化
	
	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//指定要初始化的IO, GPIO_Pin_0 Key3
	GPIO_Inilize(GPIO_P5, &GPIO_InitStructure);	//初始化
}

int getMyKeyData(char *wifi_send_buf)
{
	int num = 0;
	wifi_send_buf[0] = '#';
	wifi_send_buf[1] = '#';
	wifi_send_buf[2] = '#';
	wifi_send_buf[3] = '#';
	wifi_send_buf[4] = '#';
	wifi_send_buf[5] = '#';
	wifi_send_buf[6] = '#';

	if(KEY0)
	{
		wifi_send_buf[0] = '0';
		num = num + 1 << 0;
	}
	if(KEY1)
	{
		wifi_send_buf[1] = '1';
		num = num + 1 << 1;
	}
	if(KEY2)
	{
		wifi_send_buf[2] = '2';
		num = num + 1 << 2;
	}
	if(KEY3)
	{
		wifi_send_buf[3] = '3';
		num = num + 1 << 3;
	}
	if(KEY4)
	{
		wifi_send_buf[4] = '4';
		num = num + 1 << 4;
	}
	if(KEY5)
	{
		wifi_send_buf[5] = '5';
		num = num + 1 << 5;
	}
	if(KEY6)
	{
		wifi_send_buf[6] = '6';
		num = num + 1 << 6;
	}
	
	return num;
}