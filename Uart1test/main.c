#include	"config.h"
#include "stdio.h"
#include	"GPIO.h"
#include	"delay.h"
#include "uart1.h"
#include "string.h"


void main()
{
	unsigned int i;
	char str[] = "hello\r\n";
	i = 0;
	Uart1_Init();
	while(1)
	{
		delay_ms(200);
		printf("helloworld %d \r\n",i++);
		
		delay_ms(200);
		SendString("hello too\r\n");
		
		delay_ms(200);
		Uart1SendData(str,strlen(str));
	}
}