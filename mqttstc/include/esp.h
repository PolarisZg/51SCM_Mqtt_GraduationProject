/*

    要求必须已经可以使用printf函数
    本esp使用 uart4 串口输出，使用 定时器4 初始化 uart4

*/

#ifndef	__ESP_H
#define	__ESP_H

#include "config.h"
#include "delay.h"
#include "uart.h"

void espInit(char *SSID, char *passwd, char *SIP, char *SPORT);
//void espSend(unsigned int length, char *wifi_send_buff);
//void Uart4SendData(unsigned char dat);
unsigned char MQTTConnect();
unsigned char MQTTPublish(char *mqttdata);
unsigned char MQTTPing();
unsigned char MQTTDisconnect();

#endif