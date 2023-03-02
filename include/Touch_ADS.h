#ifndef __TOUCH_ADS_H
#define __TOUCH_ADS_H		   

#include	"config.h"


#define uint8_t					unsigned char
#define uint16_t				unsigned short
#define int16_t					short
#define uint32_t				unsigned long
#define int32_t					long

typedef struct _tsPen_Point
{
	uint16_t u16xNew;
	uint16_t u16yNew;
	uint16_t u16xOld;
	uint16_t u16yOld;
	uint8_t Head_Flag;
	
	float xfac;
	float yfac;
	int16_t xoff;
	int16_t yoff;
	
}tsPen_Point;


/***********TOUCH GPIO***********/

#define T_CSH										P65 = 1 //
#define T_CSL										P65 = 0 //

#define T_CLKH									P64 = 1 //
#define T_CLKL									P64 = 0 //

#define T_MOSIH									P62 = 1 //
#define T_MOSIL									P62 = 0 //

#define T_BUSY									P63	//

#define T_MISO									P60	//

#define T_IRQ										P61 //






//触摸屏指令集
#define CHY 						0X90  //0B10010000即用差分方式读X坐标
#define CHX							0XD0  //0B11010000即用差分方式读Y坐标   											 
#define TEMP_RD					0XF0  //0B11110000即用差分方式读Y坐标 
#define T_NOP						0X00	//空指令


#define READ_TIMES 					5 //读取次数 15
#define LOST_VAL 						1	 //丢弃值 5
#define ERR_RANGE 					10 //误差范围





void ADS7843_disp(void);
void Touch_Adjust(void);


uint8_t ADS_Touch_Head(uint16_t *x, uint16_t *y);
uint8_t Get_Adjdata(void);



#endif
