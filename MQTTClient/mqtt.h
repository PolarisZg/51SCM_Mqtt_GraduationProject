#pragma once

#include<stdio.h>
#include<string.h>

/* ���ʱ�ȴ�ʱ�� ��λ���� */
#define MQTT_MAX_TIMES_OUT (1000U)

/* ���ӱ���ʱ�� ��λ�� */
#define MQTT_KEEP_ALIVE (300U)

/* ��������С ��λ�ֽ� */
#define MQTT_BUFF_SIZE (200U)

#define MQTT_RESULT_CODE_INIT (0x0F)

/* ��������ö�� */
typedef enum
{
	MQTT_NUM = -1,				//������ʵ��
	MQTT_CHAR = 1,				//�������ַ�
}DataType_t;

/* ��������ö�� */
typedef enum
{
	MQTT_CONNECT = 0x10U,				//����
	MQTT_CONNACK = 0x20U,			//������Ӧ
	MQTT_PUBLISH = 0x30U,				//������Ϣ
	MQTT_PUBACK = 0x40U,				//������Ϣ��Ӧ QoS = 1
	MQTT_PUBREC = 0x50U,					//������Ϣ��Ӧ QoS = 2
	MQTT_PUBREL = 0x60U,
	MQTT_PUBCOMP = 0x70U,
	MQTT_SUBSCRIBE = 0x82U,			//����
	MQTT_SUBACK = 0x90U,				//������Ӧ
	MQTT_UNSUBSCRIBE = 0xA2U,		//ȡ������
	MQTT_UNSUBACK = 0xB0U,			//ȡ��������Ӧ
	MQTT_PINGREQ = 0xC0U,				//����
	MQTT_PINGRESP = 0xD0U,				//������Ӧ
	MQTT_DISCONNECT = 0xE0U		//�Ͽ�����
}MessageType_t;

/* MQTT�ṹ�� */
typedef struct
{
	MessageType_t messageType;		//���ĵ�����
	char* sendBuff;								//ָ�����ݷ��ͻ�������ָ��
	char* sendBuffPointNow;				//ָ�����ݷ��ͻ�������ǰλ�õ�ָ��
	char resultCode;								//������ش���
	unsigned int remainLength;			//�����õ�ʣ�೤��
	void* returnData;								//ָ�����˷��صĶ�Ӧ�ı��Ľṹ���ָ��
	int (*MQTTSendDataToServer)(const char* data, unsigned int length);		//��������-----�ӿ�
	void (*MQTTDelayms)(unsigned int ms);													//��ʱ���뺯��-----�ӿ�
	void (*MQTTReceiveDadaFromServer)(unsigned char* data, unsigned int length);	//�ӷ��������յ�����-----�ӿ�
}MQTT_t;

extern MQTT_t mqtt;

/* ���ӱ��Ľṹ�� */
typedef struct
{
	unsigned char connectFlag;		//���ӱ�־
	unsigned short keepAliveTime;	//����ʱ��
	const char* clientID;					//�ͻ���ID
	const char* userName;				//�û���
	const char* password;					//����
}MQTTConnectStruct_t;

/* ȷ����������ṹ�� */
typedef struct
{
	char sp;						//��ǰ�Ự��־
	unsigned char code;	//���ӷ�����
}MQTTConnACKStruct_t;

/* ������Ϣ�ṹ�� */
typedef struct
{
	char RETAIN;
	char QoS;
	char DUP;
	unsigned short messageID;	//���ı�ʶ��
	const char* topic;				//����
	const char* payload;			//��Ч�غ�
}MQTTPublishStruct_t;

/* ����ȷ�Ͻṹ�� */
//PUBACK �����Ƕ� QoS 1/2 �ȼ��� PUBLISH ���ĵ���Ӧ��
typedef struct
{
	unsigned short messageID;//���ı�ʶ��
}MQTTPubACKStruct_t;

/* ��������ṹ�� */
typedef struct
{
	unsigned short messageID;//���ı�ʶ��(�ͻ��˱�ʶ��)
	char QoS;
	const char* payload;			//��Ч�غ�
}MQTTSubscribeStruct_t;


/* ����ȷ�Ͻṹ�� */
typedef struct
{
	unsigned short messageID;//���ı�ʶ��
	char code;//������
}MQTTSubACKStruct_t;


/* ȡ�����Ľṹ�� */
typedef struct
{
	unsigned short messageID;//���ı�ʶ��
	const char* payload;			//��Ч�غ�
}MQTTUnsubscribeStruct_t;


/* ȡ������ȷ�Ͻṹ�� */
typedef struct
{
	unsigned short messageID;//���ı�ʶ��
}MQTTUnsubACKStruct_t;