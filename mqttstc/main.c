#include	"config.h"
#include "stdio.h"
#include	"GPIO.h"
#include	"delay.h"
#include "string.h"
#include "myKey.h"
#include "oled.h"
#include "uart.h"
#include "esp.h"

char wifi_send_buf[] = "#######";

void main()
{
	unsigned int i;
	int keynum;
	int keytemp;
	i = 0;
	keynum = 0;
	
	WDT_CONTR = 0x17;
	myKeyInit();
	WDT_CONTR = 0x17;
	Uart1Init();
	WDT_CONTR = 0x17;
	
	uart1SendString("go go go \r\n");
	
	OLED_Init();			//初始化OLED
	WDT_CONTR = 0x17;
	OLED_Clear();
	OLED_ShowString(0,0,"now push",12);
	uart1SendString("oled end \r\n");
	WDT_CONTR = 0x17;
	
	/* 初始化esp8266 */
	espInit("Mi 11","qqqqqqqq","47.91.8.141","1883");
	WDT_CONTR = 0x17;

	uart1SendString("\r\n now i am delay 10 second \r\n");
	//delay_10s();
	
	/* 连接CONNECT */
	MQTTConnect();
	WDT_CONTR = 0X37;
	delay_ms(100);
	WDT_CONTR = 0X37;
	
	while(1)
	{
		WDT_CONTR = 0X37; //喂狗操作 
		delay_ms(100);
		keytemp = getMyKeyData(wifi_send_buf);
		if(keynum != keytemp){
			keynum = keytemp;
			printf("now push : ");
			
			for(i = 0 ; i < 7 ; i++)
			{
				if(wifi_send_buf[i] == '#')
				{
					OLED_ShowChar(i*8,4,'0' + i,12);
				}
				else
				{
					OLED_ShowChar(i*8,4,' ',12);
				}
			}
			WDT_CONTR = 0X37; //喂狗操作 
			printf("\r\nwifisendbuff is %s",wifi_send_buf);
			printf("\r\n");
			MQTTPublish(wifi_send_buf);
			WDT_CONTR = 0X37; //喂狗操作 
			delay_ms(100);
	}
	}
}