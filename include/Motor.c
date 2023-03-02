#include	"Motor.h"
#include "timer.h"


unsigned int g_rpm = 0;
static unsigned char temp;
static unsigned int rpm = 0;

static void delay(unsigned char s)
{
	unsigned char i = 10;
	
	while(s--){
		while(i--)
		{
			NOP1();
			NOP1();
			NOP1();
		}
	}
}

void PWM_Duty(unsigned char duty)
{
	temp = duty;
}

//1000ms更新一次
void Motor_speed(void)
{
	g_rpm = rpm;//1000ms 多少转
	rpm = 0;
}

//1ms检测一次
void Motor_fall(void)
{
	static unsigned char temp1 = 0, temp2 = 0;
	
	//1ms进来一次
	if(SPEED_INT == 0)
	{
		temp1 = 0;
	}else
	{
		temp1 = 1;
	}
	
	if(temp1 < temp2)
	{
		rpm++;
	}
	
	temp2 = temp1;
}

//1ms改变一次
void PWM1(void)
{
	if(timer0_cnt < temp)
	{
		PWM_OUT = 1;
	}else
	{
		PWM_OUT = 0;
	}
}

void Bj_Motor(unsigned char speed, unsigned char pn)
{
	if(pn == 0)
	{
		BJ_OUT_A = 1;
		BJ_OUT_B = 0;
		BJ_OUT_C = 0;
		BJ_OUT_D = 1;
		delay(speed);
		
		BJ_OUT_A = 1;
		BJ_OUT_B = 1;
		BJ_OUT_C = 0;
		BJ_OUT_D = 0;
		delay(speed);
		
		BJ_OUT_A = 0;
		BJ_OUT_B = 1;
		BJ_OUT_C = 1;
		BJ_OUT_D = 0;
		delay(speed);
		
		BJ_OUT_A = 0;
		BJ_OUT_B = 0;
		BJ_OUT_C = 1;
		BJ_OUT_D = 1;
		delay(speed);
	}else if(pn == 1)
	{
		BJ_OUT_D = 1;
		BJ_OUT_C = 0;
		BJ_OUT_B = 0;
		BJ_OUT_A = 1;
		delay(speed);
		
		BJ_OUT_D = 1;
		BJ_OUT_C = 1;
		BJ_OUT_B = 0;
		BJ_OUT_A = 0;
		delay(speed);
		
		BJ_OUT_D = 0;
		BJ_OUT_C = 1;
		BJ_OUT_B = 1;
		BJ_OUT_A = 0;
		delay(speed);
		
		BJ_OUT_D = 0;
		BJ_OUT_C = 0;
		BJ_OUT_B = 1;
		BJ_OUT_A = 1;
		delay(speed);
	}else
	{
		BJ_OUT_A = 0;
		BJ_OUT_B = 0;
		BJ_OUT_C = 0;
		BJ_OUT_D = 0;
	}
}
