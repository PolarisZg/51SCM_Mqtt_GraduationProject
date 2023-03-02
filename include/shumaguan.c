#include "shumaguan.h"

unsigned char code dis_t[] = {0xFC, 0x90, 0xE5, 0xF1, 0x99, 0x79, 0x7D, 0xD0, 0xFD, 0xF9, 0xDD, 0x3D, 0x6C, 0xB5, 0x6D, 0x4D, 0x02, 0x01};
//                            '0'  '1'  '2'  '3'  '4'  '5'  '6'  '7'  '8'  '9'  'a'  'b'  'c'  'd'  'e'  'f'  '.'   '-'

unsigned char smg_data[8] = {0};
static unsigned char cnt = 0, sec = 0, min = 0;

//局部短延时
static void delay_us(unsigned int us)
{
	while (us--)
		_nop_();
}

//根据数据的bit位将相应的bit位输出到138译码器的3个输入端
void HC138_bit(unsigned char dat)
{
	A138_Bit = (dat & 0x01) ? 1 : 0; //取出相应Bit位，赋值到对应的三八译码器的A引脚
	B138_Bit = (dat & 0x02) ? 1 : 0; //取出相应Bit位，赋值到对应的三八译码器的B引脚
	C138_Bit = (dat & 0x04) ? 1 : 0; //取出相应Bit位，赋值到对应的三八译码器的C引脚
}

//数码管驱动
void Smg_test(void)
{
	unsigned char i;

	for (i = 0; i < 8; i++)
	{
		HC138_bit(i);
		Data_Bit = dis_t[i];
		delay_us(500);
		// Data_Bit = 0x00;
	}
}

void updata_display(void)
{
	smg_data[0] = min / 10; //显示分钟的十位
	smg_data[1] = min % 10; //显示分钟的个位

	smg_data[2] = 17;

	smg_data[3] = sec / 10; //显示秒的十位
	smg_data[4] = sec % 10; //显示秒的个位

	smg_data[5] = 17;

	smg_data[6] = cnt / 10; //显示毫秒的十位
	smg_data[7] = cnt % 10; //显示毫秒的个位
}

void timer1_cnt_fun(void)
{
	if (cnt > 99) //当毫秒位显示大于99时
	{
		cnt = 0;	  //毫秒为置零
		if (sec > 59) //当秒位大于59时
		{
			sec = 0; //秒位置零
			min++;	 //分钟加1
		}
		else
		{
			sec++;
		}
	}
	else
	{
		cnt++;
	}

	updata_display(); //更新显示八段数码管的缓存
}
