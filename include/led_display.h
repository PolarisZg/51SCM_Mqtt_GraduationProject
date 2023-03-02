#ifndef	__LED_DISPLAY_H
#define	__LED_DISPLAY_H

#include "config.h"



#define S_CLK							P06
#define S_DATA						P04
#define S_HSTB						P02
#define S_LSTB						P00






void L_Display(unsigned char str[][16], unsigned char speed, unsigned char cnt);
void R_Display(unsigned char str[][16], unsigned char speed, unsigned char cnt);
void S_Display(unsigned char str[][16], unsigned char speed, unsigned char cnt);



#endif
