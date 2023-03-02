#ifndef		__SHUMAGUAN_H
#define		__SHUMAGUAN_H

#include	"config.h"

#define Data_Bit						P0
#define A138_Bit						P76
#define B138_Bit						P77
#define C138_Bit						P74
#define Buzzer_Bit					P71


void Smg_test(void);
// void Buzzer(void);
void timer1_cnt_fun(void);
void HC138_bit(unsigned char dat)

#endif
