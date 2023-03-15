#include	"config.h"
#include "stdio.h"
#include	"GPIO.h"
#include	"delay.h"
#include "uart1.h"
#include "string.h"
#include "myKey.h"
#include "oled.h"

void main()
{
	unsigned char d, tempd;
	unsigned int i;
	unsigned int offset = 0;
	char str[] = "hello\r\n";
	i = 0;
	d = tempd = 0x00;
	
	myKeyInit();
	Uart1_Init();
	
	printf("go go go \r\n");
	
	OLED_Init();			//初始化OLED
	OLED_Clear();
	OLED_ShowString(0,0,"now push",12);
	
	while(1)
	{
		delay_ms(100);
		tempd = getMyKeyData();
		if(tempd != d)
		{
			d = tempd;
			
			printf("now push : ");
			
			for(i = 0 ; i < 7 ; i++)
			{
				if((tempd >> i) & 0x01 == 0x01)
				{
					printf("%d",i);
					OLED_ShowChar(i*8,4,'0'+i,12);
				}
				else
					OLED_ShowChar(i*8,4,' ',12);
			}
			printf("\r\n");
		}
	}
}