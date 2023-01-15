#pragma once

#include<stdio.h>
#include<string.h>

/* 最大超时等待时间 单位毫秒 */
#define MQTT_MAX_TIMES_OUT (1000U)

/* 链接保活时间 单位秒 */
#define MQTT_KEEP_ALIVE (300U)

/* 缓冲区大小 单位字节 */
#define MQTT_BUFF_SIZE (200U)

#define MQTT_RESULT_CODE_INIT (0x0F)

/* 数据类型枚举 */
typedef enum
{
	MQTT_NUM = -1,				//数据是实数
	MQTT_CHAR = 1,				//数据是字符
}DataType_t;

/* 报文类型枚举 */
typedef enum
{
	MQTT_CONNECT = 0x10U,				//连接
	MQTT_CONNACK = 0x20U,			//连接响应
	MQTT_PUBLISH = 0x30U,				//发布消息
	MQTT_PUBACK = 0x40U,				//发布消息响应 QoS = 1
	MQTT_PUBREC = 0x50U,					//发布消息响应 QoS = 2
	MQTT_PUBREL = 0x60U,
	MQTT_PUBCOMP = 0x70U,
	MQTT_SUBSCRIBE = 0x82U,			//订阅
	MQTT_SUBACK = 0x90U,				//订阅响应
	MQTT_UNSUBSCRIBE = 0xA2U,		//取消订阅
	MQTT_UNSUBACK = 0xB0U,			//取消订阅响应
	MQTT_PINGREQ = 0xC0U,				//保活
	MQTT_PINGRESP = 0xD0U,				//保活响应
	MQTT_DISCONNECT = 0xE0U		//断开连接
}MessageType_t;

/* MQTT结构体 */
typedef struct
{
	MessageType_t messageType;		//报文的类型
	char* sendBuff;								//指向数据发送缓冲区的指针
	char* sendBuffPointNow;				//指向数据发送缓冲区当前位置的指针
	char resultCode;								//结果返回代码
	unsigned int remainLength;			//解码获得的剩余长度
	void* returnData;								//指向服务端返回的对应的报文结构体的指针
	int (*MQTTSendDataToServer)(const char* data, unsigned int length);		//发送数据-----接口
	void (*MQTTDelayms)(unsigned int ms);													//延时毫秒函数-----接口
	void (*MQTTReceiveDadaFromServer)(unsigned char* data, unsigned int length);	//从服务器接收到数据-----接口
}MQTT_t;

extern MQTT_t mqtt;

/* 连接报文结构体 */
typedef struct
{
	unsigned char connectFlag;		//连接标志
	unsigned short keepAliveTime;	//保活时间
	const char* clientID;					//客户端ID
	const char* userName;				//用户名
	const char* password;					//密码
}MQTTConnectStruct_t;

/* 确认连接请求结构体 */
typedef struct
{
	char sp;						//当前会话标志
	unsigned char code;	//连接返回码
}MQTTConnACKStruct_t;

/* 发布消息结构体 */
typedef struct
{
	char RETAIN;
	char QoS;
	char DUP;
	unsigned short messageID;	//报文标识符
	const char* topic;				//主题
	const char* payload;			//有效载荷
}MQTTPublishStruct_t;

/* 发布确认结构体 */
//PUBACK 报文是对 QoS 1/2 等级的 PUBLISH 报文的响应。
typedef struct
{
	unsigned short messageID;//报文标识符
}MQTTPubACKStruct_t;

/* 订阅主题结构体 */
typedef struct
{
	unsigned short messageID;//报文标识符(客户端标识符)
	char QoS;
	const char* payload;			//有效载荷
}MQTTSubscribeStruct_t;


/* 订阅确认结构体 */
typedef struct
{
	unsigned short messageID;//报文标识符
	char code;//返回码
}MQTTSubACKStruct_t;


/* 取消订阅结构体 */
typedef struct
{
	unsigned short messageID;//报文标识符
	const char* payload;			//有效载荷
}MQTTUnsubscribeStruct_t;


/* 取消订阅确认结构体 */
typedef struct
{
	unsigned short messageID;//报文标识符
}MQTTUnsubACKStruct_t;