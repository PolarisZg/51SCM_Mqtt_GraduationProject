#ifndef	__MOTOR_H
#define	__MOTOR_H

#include "config.h"

#define PWM_OUT							P74
#define SPEED_INT						P76

#define BJ_OUT_A							P00
#define BJ_OUT_B							P02
#define BJ_OUT_C							P04
#define BJ_OUT_D							P06



extern unsigned int g_rpm;

void PWM1(void);
void PWM_Duty(unsigned char duty);

void Motor_speed(void);
void Motor_fall(void);
void Bj_Motor(unsigned char speed, unsigned char pn);



#endif
