#include	"config.h"
#include "stdio.h"
#include	"GPIO.h"
#include	"delay.h"
//#include "uart1.h"
#include "string.h"
#include "myKey.h"
#include "oled.h"
//#include "uart_4.h"
#include "uart.h"

unsigned char wifi_send_buf[] = "#######";
unsigned char ctrlZ = 0x1a;

void delay_10s()
{
	delay_ms(1000);	//延时
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
}

void main()
{
	unsigned char d, tempd;
	unsigned int i;
	unsigned int offset = 0;
	char str[] = "hello\r\n";
	i = 0;
	d = tempd = 0x00;
	
	myKeyInit();
	Uart1Init();
	
	printf("go go go \r\n");
	
	OLED_Init();			//初始化OLED
	OLED_Clear();
	OLED_ShowString(0,0,"now push",12);
	printf("oled end \r\n");
	
	/* 初始化esp8266 */
	Uart4Init();
	printf("u4 init end\r\n");
	Uart4SendString("AT+CWJAP=\"WF406\",\"406406ncepu\"\r\n");
	printf("AT+CWJAP=\"Mi 11\",\"qqqqqqqq\"\r\n");
	delay_10s();
	printf("race:\r\n");//串口1发送信息
	Uart1SendData(uart4_race_buf,uart4_race_count);
	delay_ms(1000);//延时1秒
	
	uart4_race_count = 0;//串口4接收计数清零
	Uart4SendString("AT+CWMODE=1\r\n");
	printf("AT+CWMODE=1\r\n");
	delay_10s();
	printf("race:\r\n");//串口1发送信息
	Uart1SendData(uart4_race_buf,uart4_race_count);//将串口4收到的数据通过串口1发送给电脑
	delay_ms(2000);
	
	uart4_race_count = 0;//串口4接收计数清零
	Uart4SendString("AT+CIPMUX=0\r\n");//串口4发送AT命令给WIFI模块
	printf("AT+CIPMUX=0\r\n");
	delay_10s();//
	printf("race:\r\n");//串口1发送信息
	Uart1SendData(uart4_race_buf,uart4_race_count);//将串口4收到的数据通过串口1发送给电脑
	delay_ms(2000);
	
	uart4_race_count = 0;//串口4接收计数清零
	Uart4SendString("send:\r\nAT+CIPSTART=\"TCP\",\"192.168.1.107\",12345\r\n");
	printf("send:\r\nAT+CIPSTART=\"TCP\",\"192.168.1.107\",12345\r\n");
	delay_10s();//
	printf("race:\r\n");//串口1发送信息
	Uart1SendData(uart4_race_buf,uart4_race_count);//将串口4收到的数据通过串口1发送给电脑
	delay_ms(2000);//
	
	printf("esp init end \r\n");
	
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
					wifi_send_buf[i] = '0' + i;
					OLED_ShowChar(i*8,4,'0'+i,12);
				}
				else
				{
					wifi_send_buf[i] = '#';
					OLED_ShowChar(i*8,4,' ',12);
				}
			}
			printf("\r\nwifisendbuff is %s",wifi_send_buf);
			printf("\r\n");
	
			uart4_race_count = 0;//串口4接收计数清零
			Uart4SendString("send:\r\nAT+CIPSEND=7\r\n");//
			while(1){
				if(uart4_race_count - 2 >= 0 && uart4_race_buf[uart4_race_count - 2] == '>')
					break;
			}

			// printf("send:\r\nAT+CIPSEND=7\r\n");
			// delay_10s();//
			printf("\r\n------race:\r\n");//
			Uart1SendData(uart4_race_buf,uart4_race_count);//将串口4收到的数据通过串口1发送给电脑
			// while(uart4_race_count > 0)
			// {
			// 	printf("\r\n%d : %c \r\n",uart4_race_count,uart4_race_buf[uart4_race_count]);
			// 	uart4_race_count--;
			// }

			uart4_race_count = 0;//串口4接收计数清零
			Uart4SendString(wifi_send_buf);//串口4发送需要发送的数据   数据在wifi_send_buf 数组中   数组的定义在最上面
			delay_ms(1000);//

			Uart4SendString("+++");
			delay_ms(2000);
		}
	}
}