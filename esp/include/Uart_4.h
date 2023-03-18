#ifndef		__UART_4_H
#define		__UART_4_H

#include	"config.h"


extern unsigned char uart4_race_buf[100];
extern unsigned int uart4_race_count;

void Uart4_Init(void);
void uart4_SendData(unsigned char dat);
void uart4_SendString(char *s);



#endif
