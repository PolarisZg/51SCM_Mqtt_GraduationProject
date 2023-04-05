#ifndef		__UART_H
#define		__UART_H

#include "config.h"

/* uart1 buff */
extern unsigned char uart1_buf[128];
extern unsigned char UART1_LEN, UART1_STA;

/* uart1 func */
void Uart1Init(void);
void uart1SendString(char *s);
void uart1SendChar(unsigned char dat);
void Uart1SendData(char *s, unsigned short len);

#endif