//******************************************************************************/
#ifndef __OLED_H
#define __OLED_H


#include	"config.h"
#include	"GPIO.h"





#define OLED_CMD					0	//写命令
#define OLED_DATA					1	//写数据
#define OLED_MODE					0


#define OLED_SCL						P45//时钟 D0（SCLK?
#define OLED_SDIN						P46//D1（MOSI） 数据






//OLED模式设置
//0:4线串行模式
//1:并行8080模式

#define SIZE						16
#define XLevelL					0x02
#define XLevelH					0x10
#define Max_Column			128
#define Max_Row					64
#define	Brightness			0xFF 
#define X_WIDTH					128
#define Y_WIDTH					64	    						  
//-----------------OLED端口定义----------------  					   



//OLED控制用函数
//void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Clear_S(unsigned char start, unsigned char end);
void OLED_DrawPoint(unsigned char x,unsigned char y,unsigned char t);
void OLED_Fill(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char dot);
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char Char_Size);
void OLED_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size);
void OLED_ShowString(unsigned char x,unsigned char y, unsigned char *p,unsigned char Char_Size);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(unsigned char x,unsigned char y,unsigned char no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
// void Delay_50ms(unsigned int Del_50ms);
// void Delay_1ms(unsigned int Del_1ms);
// void fill_picture(unsigned char fill_Data);
void Picture();
void IIC_Start();
void IIC_Stop();
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);
void IIC_Wait_Ack();
#endif
