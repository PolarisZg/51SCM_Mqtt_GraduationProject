#include "uart.h"
#include "stdio.h"
#include "delay.h"

#define BAUD 115200UL             //串口波特率
#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7
#define STI		0x02              //S4CON.1

bit busy;

unsigned char uart1_buf[128] = {0};
unsigned char UART1_LEN = 0, UART1_STA = 0;

void Uart1_Init(void)//串口初始化函数
{
	ACC = P_SW1;
	ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0(P3.0/RxD, P3.1/TxD)
	P_SW1 = ACC;
	SCON  = (SCON & 0x3f) | 0x40;                //8位可变波特率
	
	T2L = (65536 - (MAIN_Fosc/4/BAUD));   //设置波特率重装值
	T2H = (65536 - (MAIN_Fosc/4/BAUD))>>8;

	AUXR = 0x14;                //T2为1T模式, 并启动定时器2
	AUXR |= 0x01;               //选择定时器2为串口1的波特率发生器
	PS = 1;	//高优先级中断
	
	ES = 1;     //使能串口1中断
	REN = 1;	//允许接收
	EA = 1;		//允许全局中断
}

//串口中断函数      不管程序运行到哪里，只要串口收到数据，程序就会停止 并 跳转到这里，执行完中断函数后  再回到原来停止的地方 继续执行
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
		busy = 0;
	}
}

//串口发送一个字节
void SendData(unsigned char dat)  //dat是要发送的字节
{
	while (busy)
		; //等待前面的数据发送完成
	busy = 1;
	SBUF = dat; //写数据到UART1数据寄存器
}

//串口发送数组函数
void Uart1SendData(char *s, unsigned short len)   //*s是一个指针   指向要发送的数组    len是要发送数组的长度（单位字节）
{
    while (len)               // 当len等于0  就借宿{}中的循环
    {
		SendData(*s++);         //发送当前字节  发送完成后  指针++  指向下一个字节
		len--;									//发送长度减1
    }
}

// printf 函数用到的串口发送函数   如果没有  就不能用printf 这个函数
char putchar(char c)
{
	ES = 0;                 //关闭串口1中断
	
	SBUF = c;
	
	while(!TI);
	TI = 0;         //清除TI位
	
	ES = 1;                 //使能串口1中断
	
	return c;
}

// 发送字符串
void SendString(char *s)
{
	while (*s) //检测字符串结束标志
	{
		SendData(*s++); //发送当前字符
	}
}

