#include "led_display.h"
#include "string.h"

//微秒延时
static void delay(void)
{
	unsigned char i = 250;

	while (i--)
	{
		;
	}
}

void Display_16_16(unsigned char *str)
{
	unsigned char i, m;

	//上半屏
	for (m = 0; m < 8; m++)
	{
		//送入列数据
		for (i = 0; i < 16; i++)
		{
			if (str[15 - i] & (0x01 << m))
			{
				S_DATA = 0;
			}
			else
			{
				S_DATA = 1;
			}

			S_CLK = 0;
			S_CLK = 1;
		}
		S_LSTB = 1;
		S_LSTB = 0;

		//开起行是能
		for (i = 0; i < 16; i++)
		{
			if ((0x0001 << m) & (0x0001 << i))
			{
				S_DATA = 0;
			}
			else
			{
				S_DATA = 1;
			}

			S_CLK = 0;
			S_CLK = 1;
		}
		S_HSTB = 1;
		S_HSTB = 0;

		delay();
		delay();
		delay();

		//消隐
		S_DATA = 1;
		for (i = 0; i < 16; i++)
		{
			S_CLK = 0;
			S_CLK = 1;
		}
		S_HSTB = 1;
		S_HSTB = 0;
	}

	//下半屏
	for (m = 0; m < 8; m++)
	{
		//送入列数据
		for (i = 0; i < 16; i++)
		{
			if (str[15 - i + 16] & (0x01 << m))
			{
				S_DATA = 0;
			}
			else
			{
				S_DATA = 1;
			}

			S_CLK = 0;
			S_CLK = 1;
		}
		S_LSTB = 1;
		S_LSTB = 0;

		//开起行是能
		for (i = 0; i < 16; i++)
		{
			if ((0x0100 << m) & (0x0001 << i))
			{
				S_DATA = 0;
			}
			else
			{
				S_DATA = 1;
			}

			S_CLK = 0;
			S_CLK = 1;
		}
		S_HSTB = 1;
		S_HSTB = 0;

		delay();
		delay();
		delay();

		//消隐
		S_DATA = 1;
		for (i = 0; i < 16; i++)
		{
			S_CLK = 0;
			S_CLK = 1;
		}
		S_HSTB = 1;
		S_HSTB = 0;
	}
}

void L_Display(unsigned char str[][16], unsigned char speed, unsigned char cnt)
//cnt，显示的字符数量，speed，显示的速度
//字符向左滚动
{
	unsigned char temp[32];
	unsigned char i, j, offset;
	unsigned char speed_cnt;

	for (j = 0; j < (cnt - 1); j++)
	{
		offset = 2 * j; //每个字占2行

		for (i = 0; i < 16; i++) //每个字符显示16次，实现向左滚动的效果
		{
			//使用memcpy将显示字符从代码区复制到RAM中
			//点阵的上半部分
			//每次往左移动一位
			memcpy(&temp[0], &str[offset][i], (16 - i));
			memcpy(&temp[16 - i], &str[offset + 2][0], i); //第二个汉字
			//下半部分
			memcpy(&temp[16], &str[offset + 1][i], (16 - i));
			memcpy(&temp[16 + 16 - i], &str[offset + 3][0], i); //第二个汉字

			speed_cnt = speed;
			while (speed_cnt--) //显示速度
			{
				Display_16_16(temp);
			}
		}
	}

	// speed_cnt = speed * 10;
	// while(speed_cnt--)
	// {
	// 	Display_16_16(temp);
	// }
}

void R_Display(unsigned char str[][16], unsigned char speed, unsigned char cnt)
//cnt，显示的字符数量，speed，显示的速度
//字符向右滚动
{
	unsigned char temp[32] = 0;
	unsigned char i, j, offset;
	unsigned char speed_cnt;

	for (j = 1; j < (cnt - 1); j++)
	{
		offset = 2 * j;

		for (i = 0; i < 16; i++)
		{
			//上半部分
			memcpy(&temp[0], &str[offset][15 - i], i + 1);
			memcpy(&temp[i+1], &str[offset - 2][0], 15-i);
			//下半部分
			memcpy(&temp[16],&str[offset + 1][15-i],i+1);
			memcpy(&temp[i+16 +1], &str[offset - 1][0], 15-i);

			speed_cnt = speed;
			while (speed_cnt--)
			{
				if (KEY0 == 0) //按下K0暂停显示
					while (1)
					{
						Display_16_16(temp);
						if (KEY0 == 1)
							break;
					}
				Display_16_16(temp);
			}
		}
	}
}

void S_Display(unsigned char str[][16], unsigned char speed, unsigned char cnt)
//cnt，显示的字符数量，speed，显示的速度
//静态显示
{
	unsigned char temp[32];
	unsigned char j, offset;
	unsigned char speed_cnt;

	for (j = 0; j < (cnt - 1); j++)
	{
		offset = 2 * j; //每个字占2行

		//使用memcpy将显示字符从代码区复制到RAM中
		memcpy(&temp[0], &str[offset][0], 16);		//复制第一行，上半部分
		memcpy(&temp[16], &str[offset + 1][0], 16); //复制第二行，下半部分

		speed_cnt = speed * 10;
		while (speed_cnt--) //显示速度
		{
			Display_16_16(temp);
		}
	}
}

void UP_Display(unsigned char str[][16], unsigned char speed, unsigned char cnt)
//cnt，显示的字符数量，speed，显示的速度
//向上滚动显示
{
	unsigned char temp[32];
	unsigned char j, offset;
	unsigned char speed_cnt;

	for (j = 0; j < (cnt - 1); j++)
	{
		offset = 2 * j; //每个字占2行

		//使用memcpy将显示字符从代码区复制到RAM中
		memcpy(&temp[0], &str[offset][0], 16);		//复制第一行，上半部分
		memcpy(&temp[16], &str[offset + 1][0], 16); //复制第二行，下半部分

		speed_cnt = speed * 10;
		while (speed_cnt--) //显示速度
		{
			Display_16_16(temp);
		}
	}
}