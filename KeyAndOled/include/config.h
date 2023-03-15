#ifndef		__CONFIG_H
#define		__CONFIG_H


/*********************************************************/

//#define MAIN_Fosc		22118400L	//定义主时钟
//#define MAIN_Fosc		12000000L	//定义主时钟
//#define MAIN_Fosc		11059200L	//定义主时钟
//#define MAIN_Fosc		5529600L	//定义主时钟
//#define MAIN_Fosc		24000000L	//定义主时钟
#define MAIN_Fosc		11059200L	//定义主时钟


/*********************************************************/

#include	"STC15Fxxxx.H"

#define LED0						P37
#define LED1						P41
#define LED2						P42
#define LED3						P43


#define KEY0						P32
#define KEY1						P33
#define KEY2						P34
#define KEY3						P50
#define KEY4						P51

#define KEY5						P35
#define KEY6						P36
#define Buzzer                      P71
#endif
