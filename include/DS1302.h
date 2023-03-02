#ifndef __DS1302_H
#define __DS1302_H

#include "config.h"

/***********LCD GPIO***********/
#define LCD_CSH P21 = 1 //GPIO_SetBits(GPIOD,GPIO_Pin_3)
#define LCD_CSL P21 = 0 //GPIO_ResetBits(GPIOD,GPIO_Pin_3)

#define LCD_RESH P66 = 1 //GPIO_SetBits(GPIOD,GPIO_Pin_7)
#define LCD_RESL P66 = 0 //GPIO_ResetBits(GPIOD,GPIO_Pin_7)

#define LCD_RSH P23 = 1 //GPIO_SetBits(GPIOD,GPIO_Pin_4)
#define LCD_RSL P23 = 0 //GPIO_ResetBits(GPIOD,GPIO_Pin_4)

#define LCD_WRH P47 = 1 //GPIO_SetBits(GPIOD,GPIO_Pin_5)
#define LCD_WRL P47 = 0 //GPIO_ResetBits(GPIOD,GPIO_Pin_5)

#define LCD_RDH P30 = 1 //GPIO_SetBits(GPIOD,GPIO_Pin_6)
#define LCD_RDL P30 = 0 //GPIO_SetBits(GPIOD,GPIO_Pin_6)

#define LCD_LEDH P67 = 1 //GPIO_SetBits(GPIOD,GPIO_Pin_7)
#define LCD_LEDL P67 = 0 //GPIO_ResetBits(GPIOD,GPIO_Pin_7)

//LCD扫描方向定义
#define L2R_U2D 0x00 //竖屏 从左到右,从上到下
#define L2R_D2U 0x80 //竖屏 从左到右,从下到上
#define R2L_U2D 0xC0 //竖屏 从右到左,从上到下
#define R2L_D2U 0x40 //竖屏 从右到左,从下到上

#define U2D_L2R 0x20 //横屏 从上到下,从左到右
#define U2D_R2L 0x60 //横屏 从上到下,从右到左
#define D2U_L2R 0xA0 //横屏 从下到上,从左到右
#define D2U_R2L 0xE0 //横屏 从下到上,从右到左

#define MAX_CHAR_POSX 312
#define MAX_CHAR_POSY 472

#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned long

#define RTC_LED P02
#define Buzzer_Bit P00
#define DS1302_CLK P72
#define DS1302_IO P74
#define DS1302_RST P76

typedef struct
{
    unsigned char year;
    unsigned char month;
    unsigned char day;

    unsigned char week;

    unsigned char hour;
    unsigned char min;
    unsigned char sec;
} RTC_time;

void ds1302_init(void);
void read_rtc(RTC_time *g_rtc_time);
unsigned char Decimal_BCD(unsigned char Decimal);
unsigned char BCD_Decimal(unsigned char bcd);
void write_1302(unsigned char add, unsigned char dat);

/**************LCD***************/

// void LCD_WR_REG(unsigned short cmd);
// void LCD_INIT_R61581(void);
// void LCD_ShowChar(unsigned short x, unsigned short y, uint8_t num, uint8_t s_size, unsigned short Color);
// void LCD_ShowNum(unsigned short x, unsigned short y, uint32_t num, uint8_t len, uint8_t s_size, unsigned short Color);
// void LCD_ShowNum2(unsigned short x, unsigned short y, uint32_t num, uint8_t len, uint8_t s_size, unsigned short Color);
// void LCD_ShowString(unsigned short x, unsigned short y, char *p, uint8_t s_size, unsigned short Color);
// void LCD_Clear(unsigned short color);
// void LCD_Color_with_ADD(unsigned short dataxy, unsigned short StartX, unsigned short StartY, unsigned short EndX, unsigned short EndY);
// void LCD_DrawPoint(unsigned short color, unsigned short StartX, unsigned short StartY);

void Show_Str(unsigned short x, unsigned short y, char *str, uint8_t size, unsigned short Color);

#endif
