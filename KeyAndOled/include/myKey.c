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
	
	myKeyData = 0x00;
}

unsigned char getMyKeyData(void)
{
	if(KEY0)
	{
		myKeyData = myKeyData & ~(0x01 << 0);
	}else
	{
		myKeyData = myKeyData | (0x01 << 0);
	}
	if(KEY1)
	{
		myKeyData = myKeyData & ~(0x01 << 1);
	}else
	{
		myKeyData = myKeyData | (0x01 << 1);
	}
	if(KEY2)
	{
		myKeyData = myKeyData & ~(0x01 << 2);
	}else
	{
		myKeyData = myKeyData | (0x01 << 2);
	}
	if(KEY3)
	{
		myKeyData = myKeyData & ~(0x01 << 3);
	}else
	{
		myKeyData = myKeyData | (0x01 << 3);
	}
	if(KEY4)
	{
		myKeyData = myKeyData & ~(0x01 << 4);
	}else
	{
		myKeyData = myKeyData | (0x01 << 4);
	}
	if(KEY5)
	{
		myKeyData = myKeyData & ~(0x01 << 5);
	}else
	{
		myKeyData = myKeyData | (0x01 << 5);
	}
	if(KEY6)
	{
		myKeyData = myKeyData & ~(0x01 << 6);
	}else
	{
		myKeyData = myKeyData | (0x01 << 6);
	}
	return myKeyData;
}