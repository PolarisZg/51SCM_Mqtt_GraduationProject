#include "config.h"
#include "GPIO.h"
#include "delay.h"
#include "uart1.h"
#include "uart4.h"
#include "stdio.h"
#include "string.h"


int main()
{
	
	
	Uart1_Init();
	Uart4_Init();
}