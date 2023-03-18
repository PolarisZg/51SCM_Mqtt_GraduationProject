#ifndef		__MYKEY_H
#define		__MYKEY_H

#include	"config.h"

extern unsigned char myKeyData;

void myKeyInit(void);
unsigned char getMyKeyData(void);

// if(KEY0)
// 		{
// 			if(k0 != KEY0)
// 			{
// 				printf("no push KEY0\r\n");
// 				k0 = KEY0;
// 			}
// 		}else
// 		{
// 			if(k0 != KEY0)
// 			{
// 				printf("push KEY0\r\n");
// 				k0 = KEY0;
// 			}
// 		}