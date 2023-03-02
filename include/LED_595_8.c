//实验三  独立按键和流水灯实验
#include	<LED_595_8.h>
static void delay_ns(void)
{
	unsigned char i = 100;
	
	while(i--);
}

void LED_595_Fun(unsigned char IO_dat)
{
	unsigned char i;
	
	for(i=0; i<8; i++)
	{
		if(IO_dat & (0x80 >> i))
		{
			LED_595_DAT = 1;
		}
		else
		{
			LED_595_DAT = 0;
		}
		
		LED_595_CLK = 0;
		delay_ns();
		LED_595_CLK = 1;
		delay_ns();
	}
	
	LED_595_RCK = 1;
	delay_ns();
	LED_595_RCK = 0;
	delay_ns();
}
