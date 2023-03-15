#ifndef		__UART1_H
#define		__UART1_H

#include	"config.h"

extern unsigned char uart1_buf[128];
extern unsigned char UART1_LEN, UART1_STA;


void Uart1_Init(void);
// void Uart1SendData(char *s, unsigned short len);
void SendString(char *s);
void SendData(unsigned char dat);
void Uart1SendData(char *s, unsigned short len);


#endif
