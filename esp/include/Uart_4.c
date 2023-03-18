//本示例在Keil开发环境下请选择Intel的8058芯片型号进行编译
//若无特别说明,工作频率一般为11.0592MHz
#include "Uart_4.h"

typedef unsigned char BYTE;
typedef unsigned int WORD;

//#define FOSC 11059200L          //系统频率
#define BAUD 115200             //串口波特率

#define NONE_PARITY     0       //无校验
#define ODD_PARITY      1       //奇校验
#define EVEN_PARITY     2       //偶校验
#define MARK_PARITY     3       //标记校验
#define SPACE_PARITY    4       //空白校验

#define PARITYBIT NONE_PARITY   //定义校验位


//sfr AUXR  = 0x8e;               //辅助寄存器
//sfr S4CON = 0x84;               //UART4 控制寄存器
//sfr T2H   = 0xd6;               //定时器2高8位
//sfr T2L   = 0xd7;               //定时器2低8位

#define S4RI  0x01              //S4CON.0
#define S4TI  0x02              //S4CON.1
#define S4RB8 0x04              //S4CON.2
#define S4TB8 0x08              //S4CON.3

#define S4_S0 0x04              //P_SW2.2

bit uart4_busy;


void Uart4_Init(void)
{
	P_SW2 |= S4_S0;             //S4_S0=1 (P5.2/RxD4_2, P5.3/TxD4_2)

	S4CON = 0x10;               //8位可变波特率

	T2L = (65536 - (MAIN_Fosc/4/BAUD));   //设置波特率重装值
	T2H = (65536 - (MAIN_Fosc/4/BAUD))>>8;

	AUXR = 0x14;                //T2为1T模式, 并启动定时器2
	PS = 0;	//高优先级中断
	
	IE2 = 0x10;                 //使能串口4中断
}

/*----------------------------
UART4 中断服务程序
-----------------------------*/
void Uart4() interrupt 18 using 1
{
	
    if (S4CON & S4RI)
    {
		S4CON &= ~S4RI;         //清除S4RI位
			
		uart4_race_buf[uart4_race_count] = S4BUF;	
		uart4_race_count++;
		if(uart4_race_count>=100)
		{
			uart4_race_count = 0;
		}
    }
		
    if (S4CON & S4TI)
    {
        S4CON &= ~S4TI;         //清除S4TI位
        uart4_busy = 0;               //清忙标志
    }
}

/*----------------------------
发送串口数据
----------------------------*/
void uart4_SendData(BYTE dat)
{
    while (uart4_busy);               //等待前面的数据发送完成
    uart4_busy = 1;
    S4BUF = dat;                //写数据到UART4数据寄存器
}

/*----------------------------
发送字符串
----------------------------*/
void uart4_SendString(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        uart4_SendData(*s++);         //发送当前字符
    }
}

void uart4_putchar(char c)
{
	IE2 &= ~0x10;                 //使能串口4中断
	
	S4BUF = c;
	
	while((S4CON & S4TI) == 0);
	S4CON &= ~S4TI;         //清除S4TI位
	
	IE2 = 0x10;                 //使能串口4中断
}
