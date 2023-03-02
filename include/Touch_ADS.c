#include <Touch_ADS.h>
#include "DS1302.h"
#include "math.h"
#include "EEPROM.h"
#include "Uart_4.h"
#include "stdio.h"



tsPen_Point Pen_Point =
{
	0,
	0,
	0,
	0,
	0,
	
	0.12,
	0.086,
	0,
	0,
};

/************************start**************************/
static void delay_m(unsigned int ms)
{
	while(ms--);
}

unsigned char SPI_ReadWriteByte(unsigned char dat)
{
	unsigned char i, temp, res = 0;
	
	temp = dat;
	for(i = 0; i < 8; i++)
	{
		if(temp & 0x80)
		{
			T_MOSIH;
		}else
		{
			T_MOSIL;
		}
		
		T_CLKH;
		
		temp <<= 1;
		T_CLKL;
		
		res <<= 1;
		if(T_MISO)
		{
			res = res | 0x01;
		}
	}
	
	return res;
}

/***********************Touch*************************/
/*********************************************************************  
*
* 函 数 名: ADS7843_WR
* 功能描述: 
* 函数说明: 
* 调用函数: 
* 全局变量: 无
* 输    入: 无
* 返    回: 无
* 设 计 者：          日期：
* 修 改 者：       		日期：
* 版    本：VER 1.0
***********************************************************************/
uint16_t ADS7843_WR(uint8_t cmd)
{
	uint16_t ADS_RxH = 0;
	uint8_t ADS_RxL = 0;
	
	SPI_ReadWriteByte(cmd);
	while(0 == T_BUSY);	//等待转换完毕，当BUSY为低时转换完毕可以接收数据。
	ADS_RxH = SPI_ReadWriteByte(T_NOP);
	ADS_RxL = SPI_ReadWriteByte(T_NOP);
	
	ADS_RxH = ADS_RxH << 8;
	ADS_RxH |= ADS_RxL;
	
	return ADS_RxH >> 4;
}
/*********************************************************************  
*
* 函 数 名: 
* 功能描述: 读取一个坐标值
* 函数说明: 连续读取READ_TIMES次数据,对这些数据升序排列,然后去掉最低和最高,LOST_VAL个数,取平均值 。
* 调用函数: 
* 全局变量: 无
* 输    入: xy
* 返    回: 无
* 设 计 者：          日期：
* 修 改 者：       		日期：
* 版    本：VER 1.0
***********************************************************************/
uint16_t ADS_Read_XY(uint8_t xy)
{
	uint8_t i,j;
	uint16_t buf[READ_TIMES];
	uint32_t temp;
	
	T_CSL;
	for(i=0; i<100; i++);	//纯属延时使用
	
	for(i=0;i<READ_TIMES;i++)
	{
		buf[i]=ADS7843_WR(xy);
	}
	T_CSH;
	
	for(i=0;i<READ_TIMES-1; i++)//排序
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//升序排列
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}
	
	temp=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)
		temp += buf[i];
	
	temp = temp / (READ_TIMES - 2 * LOST_VAL);
	
	return temp;
}
/*********************************************************************  
*
* 函 数 名: 
* 功能描述: 
* 函数说明: 
* 调用函数: 
* 全局变量: 无
* 输    入: 无
* 返    回: 无
* 设 计 者：          日期：
* 修 改 者：       		日期：
* 版    本：VER 1.0
***********************************************************************/
//2次读取ADS7846,连续读取2次有效的AD值,且这两次的偏差不能超过
//50,满足条件,则认为读数正确,否则读数错误.	   
//该函数能大大提高准确度
uint8_t Read_ADS(uint16_t *x,uint16_t *y) 
{
	uint16_t x1,y1;
 	uint16_t x2,y2;
	
	x1 = ADS_Read_XY(CHX);
	y1 = ADS_Read_XY(CHY);
	if((x1 < 80) || (y1 < 80))return(0);
	
	delay_m(1);
	
	x2 = ADS_Read_XY(CHX);
	y2 = ADS_Read_XY(CHY);
	if((x2 < 80) || (y2 < 80))return(0);
	
	if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//前后两次采样在+-50内
	&&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
	{
		*x=(x1+x2)/2;
		*y=(y1+y2)/2;
		return 1;
	}else 
	{
		return 0;
	}
}
/*********************************************************************  
*
* 函 数 名: ADS7843_disp
* 功能描述: 触摸屏画点，采集X,Y坐标。
* 函数说明: 测试使用
* 调用函数: 
* 全局变量: 无
* 输    入: 无
* 返    回: 无
* 设 计 者：          日期：
* 修 改 者：       		日期：
* 版    本：VER 1.0
***********************************************************************/
#if(1)
void ADS7843_disp(void)
{
	uint16_t x = 0, y = 0;
	
	if(!T_IRQ)
	{
		if(Read_ADS(&x,&y) == 0) return ;
		
//		printf("Touch x:%d\r\n", x);
//		printf("Touch x:%d\r\n", y);
//		LCD_Color_with_ADD(0xffff, 100, 200, 150, 340);
//		LCD_ShowNum(100,200,y,6,16, 0x0000);
//		LCD_ShowNum(100,220,x,6,16, 0x0000);
		
		x = Pen_Point.xfac * x - Pen_Point.xoff;
		y = Pen_Point.yfac * y - Pen_Point.yoff;
		
		//XPT2046不需要
//		x = 320 - x;
//		y = 480 - y;
		
//		LCD_Color_with_ADD(0xffff, 100, 300, 150, 340);
//		LCD_ShowNum(100,300,y,6,16, 0x0000);
//		LCD_ShowNum(100,320,x,6,16, 0x0000);
		
		if (x>320){x=320;}
		if (y>480){y=480;}
	}
	LCD_Color_with_ADD(0x0000,  x, y, (x+1), (y+1));
}

#endif

/*********************************************************************  
*
* 函 数 名: 
* 功能描述: 
* 函数说明: 
* 调用函数: 
* 全局变量: 无
* 输    入: 无
* 返    回: 无
* 设 计 者：          日期：
* 修 改 者：       		日期：
* 版    本：VER 1.0
***********************************************************************/
void Save_Adjdata(void)
{
	int32_t temp;
	uint8_t Pen_Point_Data[13];
	
	//保存校正结果!
	temp = 0;
	temp=Pen_Point.xfac*100000000;//保存x校正因素      
	Pen_Point_Data[0] = temp&0xFF;
	Pen_Point_Data[1] = (temp>>8)&0xFF;
	Pen_Point_Data[2] = (temp>>16)&0xFF;
	Pen_Point_Data[3] = (temp>>24)&0xFF;
	
	temp = 0;
	temp=Pen_Point.yfac*100000000;//保存y校正因素    
	Pen_Point_Data[4] = temp&0xFF;
	Pen_Point_Data[5] = (temp>>8)&0xFF;
	Pen_Point_Data[6] = (temp>>16)&0xFF;
	Pen_Point_Data[7] = (temp>>24)&0xFF;
	
	temp = 0;
  temp = Pen_Point.xoff;	//保存x偏移量
	Pen_Point_Data[8] = temp&0xFF;
	Pen_Point_Data[9] = (temp>>8)&0xFF;
	
	temp = 0;
	temp = Pen_Point.yoff;	//保存y偏移量
	Pen_Point_Data[10] = temp&0xFF;
	Pen_Point_Data[11] = (temp>>8)&0xFF;
	
	temp = 0;
	temp |= 0xA3;//标记校准过了	
	Pen_Point_Data[12] = temp&0xFF;
	
	for(temp = 0; temp < 13; temp++)
	{
		IapProgramByte(IAP_ADDRESS + temp, Pen_Point_Data[temp]);
	}
}
//得到保存在EEPROM里面的校准值
//返回值：1，成功获取数据
//        0，获取失败，要重新校准
uint8_t Get_Adjdata(void)
{
	int32_t temp;
	uint8_t Pen_Point_Get_Data[13];
	
	for(temp = 0; temp < 13; temp++)
	{
		Pen_Point_Get_Data[temp] = IapReadByte(IAP_ADDRESS + temp);
	}
	
	if((Pen_Point_Get_Data[12] & 0xFF)==0xA3) //触摸屏已经校准过了			   
	{
		temp = 0;
		temp = Pen_Point_Get_Data[3];
		temp <<= 8;
		temp |= Pen_Point_Get_Data[2];
		temp <<= 8;
		temp |= Pen_Point_Get_Data[1];
		temp <<= 8;
		temp |= Pen_Point_Get_Data[0];
		Pen_Point.xfac = (float)temp/100000000;//得到x校准参数
		
		temp = 0;
		temp = Pen_Point_Get_Data[7];
		temp <<= 8;
		temp |= Pen_Point_Get_Data[6];
		temp <<= 8;
		temp |= Pen_Point_Get_Data[5];
		temp <<= 8;
		temp |= Pen_Point_Get_Data[4];         
		Pen_Point.yfac = (float)temp/100000000;//得到y校准参数
	  
		temp = 0;
		temp = Pen_Point_Get_Data[9];	//得到x偏移量
		temp <<= 8;
		temp |= Pen_Point_Get_Data[8];			   	  
		Pen_Point.xoff=temp;
	  
		temp = 0;
		temp = Pen_Point_Get_Data[11];	//得到y偏移量
		temp <<= 8;
		temp |= Pen_Point_Get_Data[10];
		Pen_Point.yoff=temp;
		return 1;
	}
	return 0;
}
/*********************************************************************  
*
* 函 数 名: 
* 功能描述: 触摸屏校准代码
* 函数说明: 得到四个校准参数
* 调用函数: 
* 全局变量: 无
* 输    入: 无
* 返    回: 无
* 设 计 者：          日期：
* 修 改 者：       		日期：
* 版    本：VER 1.0
***********************************************************************/
void Touch_Adjust(void)
{
	uint8_t  cnt=0;
	uint16_t d1=0,d2=0;
	uint16_t pos_temp[4][2];//坐标缓存值
	uint32_t temx,temy;
	float fac;
	
	LCD_Clear(0xFFFF);//清屏 
	LCD_Color_with_ADD(0x001f, 0, 10, 20, 10);
	LCD_Color_with_ADD(0x001f, 10, 0, 10, 20);//画点1
	
	for(;;)
	{
		LED0 = 0;
		if(!T_IRQ)//按键taiqi
		{
			LED0 = 1;
			while(0 == T_IRQ)
			{
				Read_ADS(&d1, &d2);
				pos_temp[cnt][0] = d1;
				pos_temp[cnt][1] = d2;
			}
			cnt++;
			switch(cnt)
			{
				case 1:
					LCD_Clear(0xFFFF);//清屏 
					LCD_Color_with_ADD(0x001f, 0, 469, 20, 469);
					LCD_Color_with_ADD(0x001f, 10, 459, 10, 479);//画点2
					break;
				case 2:
					LCD_Clear(0xFFFF);//清屏 
					LCD_Color_with_ADD(0x001f, 299, 469, 319, 469);
					LCD_Color_with_ADD(0x001f, 309, 459, 309, 479);//画点3
					break;
				case 3:
					LCD_Clear(0xFFFF);//清屏 
					LCD_Color_with_ADD(0x001f, 299, 10, 319, 10);
					LCD_Color_with_ADD(0x001f, 309, 0, 309, 20);//画点4
					break;
				case 4:	 //全部四个点已经得到
					
	    		//对边相等
					temx=abs(pos_temp[0][0]-pos_temp[1][0]);//x1-x2
					temy=abs(pos_temp[0][1]-pos_temp[1][1]);//y1-y2
					temx*=temx;
					temy*=temy;
					d1=sqrt(temx+temy);//得到1,2的距离
					
					temx=abs(pos_temp[2][0]-pos_temp[3][0]);//x3-x4
					temy=abs(pos_temp[2][1]-pos_temp[3][1]);//y3-y4
					temx*=temx;
					temy*=temy;
					d2=sqrt(temx+temy);//得到3,4的距离
				
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)//不合格
					{
						cnt=0;
						LCD_Clear(0xFFFF);//清屏 
						LCD_Color_with_ADD(0x001f, 0, 10, 20, 10);
						LCD_Color_with_ADD(0x001f, 10, 0, 10, 20);//画点1 
						continue;
					}
					
					temx=abs(pos_temp[0][0]-pos_temp[2][0]);//x1-x3
					temy=abs(pos_temp[0][1]-pos_temp[2][1]);//y1-y3
					temx*=temx;
					temy*=temy;
					d1=sqrt(temx+temy);//得到1,3的距离
					
					temx=abs(pos_temp[1][0]-pos_temp[3][0]);//x2-x4
					temy=abs(pos_temp[1][1]-pos_temp[3][1]);//y2-y4
					temx*=temx;
					temy*=temy;
					d2=sqrt(temx+temy);//得到2,4的距离
					
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)//不合格
					{
						cnt=0;
						LCD_Clear(0xFFFF);//清屏 
						LCD_Color_with_ADD(0x001f, 0, 10, 20, 10);
						LCD_Color_with_ADD(0x001f, 10, 0, 10, 20);//画点1 
						continue;
					}//正确了
					//对角线相等
					
					temx=abs(pos_temp[1][0]-pos_temp[2][0]);//x1-x3
					temy=abs(pos_temp[1][1]-pos_temp[2][1]);//y1-y3
					temx*=temx;
					temy*=temy;
					d1=sqrt(temx+temy);//得到1,4的距离
	
					temx=abs(pos_temp[0][0]-pos_temp[3][0]);//x2-x4
					temy=abs(pos_temp[0][1]-pos_temp[3][1]);//y2-y4
					temx*=temx;
					temy*=temy;
					d2=sqrt(temx+temy);//得到2,3的距离
					
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)//不合格
					{
						cnt=0;
						LCD_Clear(0xFFFF);//清屏 
						LCD_Color_with_ADD(0x001f, 0, 10, 20, 10);
						LCD_Color_with_ADD(0x001f, 10, 0, 10, 20);//画点1 
						continue;
					}//正确了
					
					//计算结果
					Pen_Point.xfac=(float)300/(pos_temp[3][0]-pos_temp[0][0]);//得到xfac		 
					Pen_Point.xoff=((Pen_Point.xfac*(pos_temp[3][0]+pos_temp[0][0])) - 320)/2;//得到xoff
					
					Pen_Point.yfac=(float)460/(pos_temp[1][1]-pos_temp[0][1]);//得到yfac
					Pen_Point.yoff=((Pen_Point.yfac*(pos_temp[1][1]+pos_temp[0][1])) - 480)/2;//得到yoff
					
					Save_Adjdata();
					return;
			}
		}
	}
}
/*********************************************************************  
*
* 函 数 名: 
* 功能描述: 
* 函数说明: 
* 调用函数: 
* 全局变量: 无
* 输    入: 无
* 返    回: 无
* 设 计 者：          日期：
* 修 改 者：       		日期：
* 版    本：VER 1.0
***********************************************************************/

