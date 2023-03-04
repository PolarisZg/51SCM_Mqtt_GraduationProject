#include "uart1.h"
#include "stdio.h"

typedef unsigned char BYTE;

#define BAUD 115200UL //串口波特率

#define S1_S0 0x40 //P_SW1.6
#define S1_S1 0x80 //P_SW1.7

#define STI 0x02 //S4CON.1

bit busy_uart1;

unsigned char uart1_buf[128] = {0};
unsigned char UART1_LEN = 0, UART1_STA = 0;

void Uart1_Init(void)
{
	ACC = P_SW1;
	ACC &= ~(S1_S0 | S1_S1); //S1_S0=0 S1_S1=0(P3.0/RxD, P3.1/TxD)
	P_SW1 = ACC;
	SCON = (SCON & 0x3f) | 0x40; //8位可变波特率

	T2L = (65536 - (MAIN_Fosc / 4 / BAUD)); //设置波特率重装值
	T2H = (65536 - (MAIN_Fosc / 4 / BAUD)) >> 8;

	AUXR = 0x14;  //T2为1T模式, 并启动定时器2
	AUXR |= 0x01; //选择定时器2为串口1的波特率发生器
	PS = 1;		  //高优先级中断

	ES = 1;	 //使能串口1中断
			 //	TI = 1;         //S4TI位
	REN = 1; //允许接收
	EA = 1;	 //允许全局中断
}

void Uart1() interrupt 4 using 1
{
	unsigned char Res;

	if (RI)
	{
		RI = 0;

		Res = SBUF;
		if (UART1_LEN < 128)
		{
			uart1_buf[UART1_LEN++] = Res;
		}

		if (Res == '\r') //按下回车键
		{
			UART1_STA = 1;
			UART1_LEN = 0;
		}
	}
	if (TI)
	{
		TI = 0;
		busy_uart1 = 0;
	}
}

/*----------------------------
发送串口数据
// ----------------------------*/

void SendData(BYTE dat)
{
	while (busy_uart1)
		; //等待前面的数据发送完成
	busy_uart1 = 1;
	SBUF = dat; //写数据到UART1数据寄存器
}

/*----------------------------
发送字符串
----------------------------*/
// void Uart1SendData(char *s, unsigned short len)
// {
// 	while (len) //检测字符串结束标志
// 	{
// 		SendData(*s++); //发送当前字符
// 		len--;
// 	}
// }

char putchar(char c)
{
	ES = 0; //使能串口中断

	SBUF = c;

	while (!TI)
		;
	TI = 0; //清除TI位

	ES = 1; //使能串口中断

	return c;
}

/*----------------------------
发送字符串
----------------------------*/
void SendString(char *s)
{
	while (*s) //检测字符串结束标志
	{
		SendData(*s++); //发送当前字符
	}
}
