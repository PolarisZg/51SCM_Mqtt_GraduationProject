#ifndef		__UART_H
#define		__UART_H

#include "config.h"

/* uart1 buff */
extern unsigned char uart1_buf[128];
extern unsigned char UART1_LEN, UART1_STA;

/* uart4 buff*/
extern unsigned char uart4_race_buf[100];
extern unsigned int uart4_race_count;

/* uart1 func */
void Uart1Init(void);
void SendString(char *s);
void SendData(unsigned char dat);
void Uart1SendData(char *s, unsigned short len);

/* uart4 func */
void Uart4Init(void);
void Uart4SendData(unsigned char dat);
void Uart4SendString(char *s);

#endif