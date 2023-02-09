#include "mqtt.h"

MQTT_t mqtt = { 0 };

static char MQTTSendBuff[MQTT_BUFF_SIZE] = { 0 };

/** \brief 发送数据-----接口  
 *
 * \param   data 指向需要发送的数据的指针
 * \param   length 数据的长度 单位字节
 * \return  返回1则发送成功，返回0则发送失败
 *
 */
static int Demo_MQTTSendDataToServer(const char* data, unsigned int length)
{
	/*
		这里使用了C语言库函数实现该接口
	*/
	unsigned char temp = 0;
	while (length--)
	{
		temp = *data++;
		printf("%02X ", temp);
	}

	return 1;
}

#include<time.h>
/** \brief 延时毫秒函数-----接口  
 *
 * \param   ms 毫秒数
 * \return  无
 *
 */
static void Demo_MQTTDelayms(unsigned int ms)
{
	/*
	这里使用了C语言库函数实现该接口
	如果要自己实现请把下面的代码删除，
	并将
	#include <time.h>
	去除
	*/
	clock_t start = clock();
	while (clock() - start < ms);
}

/** \brief 从服务器接收到数据-----接口  
 *          (未写QoS = 1/2的响应代码和收到来自服务器的数据并解析数据，这个需要结合情景需要修改代码)
 *
 * \param   data 指向接收到的数据的指针
 * \param   length 数据的长度 单位字节
 * \return  无
 *
 */
void Demo_MQTTReceiveDadaFromServer(unsigned char* data, unsigned int length)
{
	switch (*data)
	{
		/* 确认连接请求 */
	case MQTT_CONNACK:
		//避免野指针
		if (mqtt.returnData)
		{
			//获取当前会话标志
			((MQTTConnACKStruct_t*)mqtt.returnData)->sp = *(data + 2);
			//获取连接返回码
			((MQTTConnACKStruct_t*)mqtt.returnData)->code = *(data + 3);
		}
		mqtt.resultCode = ~MQTT_RESULT_CODE_INIT;
		break;

		/* 确认发布消息请求(未写QoS = 1/2的响应代码) */
	case MQTT_PUBACK:
	case MQTT_PUBREC:
	case MQTT_PUBREL:
	case MQTT_PUBCOMP:
		mqtt.resultCode = ~MQTT_RESULT_CODE_INIT;
		break;

		/* 订阅确认 */
	case MQTT_SUBACK:
		//避免野指针
		if (mqtt.returnData)
		{
			((MQTTSubACKStruct_t*)mqtt.returnData)->messageID = *(data + 2) * 256 + *(data + 3);
			((MQTTSubACKStruct_t*)mqtt.returnData)->code = *(data + 4);
		}
		mqtt.resultCode = ~MQTT_RESULT_CODE_INIT;
		break;

		/* 取消订阅确认 */
	case MQTT_UNSUBACK:
		//避免野指针
		if (mqtt.returnData)
		{
			((MQTTUnsubACKStruct_t*)mqtt.returnData)->messageID = *(data + 2) * 256 + *(data + 3);
		}
		mqtt.resultCode = ~MQTT_RESULT_CODE_INIT;
		break;

		/* 心跳响应 */
	case MQTT_PINGRESP:
		mqtt.resultCode = ~MQTT_RESULT_CODE_INIT;
		break;

		/*
			其他服务器端返回消息
			在这里添加自己的业务代码
		*/
	default:
		break;
	}

	//清空缓冲区
	memset((void*)data, 0, length);
}

/** \brief  将数据写入MQTT的发送缓冲区
 * \param   data 指向要写入的数据源，类型强制转换为 void* 指针。
 * \param   n 要被写入的字节数。
 * \param   dataType  说明数据源 data 的数据类型，如果是字符则传入CHAR(1)，不是则传入NUM(0)。
 * \return   无。
 */
static void MQTTSendDataToBuff(void* data, unsigned int n, DataType_t dataType)
{
	if (data == NULL)
		return;
	if (dataType == MQTT_NUM)
		data = (char*)data + n - 1;
	while (n--)
	{
		*mqtt.sendBuffPointNow++ = *(char*)data;
		data = (char*)data + dataType;
	}
}

/** \brief  编码剩余长度并向发送缓冲区写入报文的类型
 *
 * \param   无
 * \return  无
 *
 */
static void codeRemainLengthAndSendMessageType(void)
{
	char temp[4] = { 0 };
	unsigned char length = 0;
	char encodedByte = 0;
	unsigned int X = 0;

	//计算剩余长度
	X = mqtt.sendBuffPointNow - mqtt.sendBuff - 1;
	//编码
	do
	{
		encodedByte = X % 128;
		X = X / 128;
		if (X > 0)
		{
			encodedByte = encodedByte | 128;
		}
		//得到已编码的剩余长度
		temp[length] = encodedByte;
		//记录编码的剩余长度占用的字节数
		length++;
	} while (X > 0);

	//反转
	while (length--)
	{
		*mqtt.sendBuff = temp[length];
		mqtt.sendBuff--;
	}

	//写入报文的类型
	*mqtt.sendBuff = mqtt.messageType;
}

/** \brief  解码剩余长度
 *
 * \param   data 指向已编码的剩余长度数组的首个元素的指针
 * \return  无
 *
 */
void decodeRemainLength(const char* data)
{
	unsigned int multiplier = 1;
	unsigned int value = 0;
	unsigned char encodedByte = 0;
	do {
		encodedByte = *data++;
		value += (encodedByte & 127) * multiplier;
		multiplier *= 128;
		if (multiplier > 128 * 128 * 128) {
			// throw Error(Malformed Remaining Length)
			// error
			return;
		}
	} while ((encodedByte & 128) != 0);

	//得到已解码的剩余长度
	mqtt.remainLength = value;
}

/** \brief  MQTT初始化
 *
 * \param   无
 * \return  无
 *
 */
void MQTTInit(void)
{
	mqtt.messageType = 0;
	mqtt.sendBuff = MQTTSendBuff + 4;
	mqtt.sendBuffPointNow = MQTTSendBuff + 5;
	mqtt.resultCode = MQTT_RESULT_CODE_INIT;
	mqtt.returnData = NULL;

	if (mqtt.MQTTSendDataToServer == NULL)
	{
		mqtt.MQTTSendDataToServer = Demo_MQTTSendDataToServer;
	}
	if (mqtt.MQTTDelayms == NULL)
	{
		mqtt.MQTTDelayms = Demo_MQTTDelayms;
	}
	if (mqtt.MQTTReceiveDadaFromServer == NULL)
	{
		mqtt.MQTTReceiveDadaFromServer = Demo_MQTTReceiveDadaFromServer;
	}
}

/** \brief  MQTT连接报文
 *
 * \param s1 指向连接报文结构体的指针
 * \param s2 指向确认连接请求结构体的指针
 * \return 返回 0 连接成功
 *				返回 1 连接超时，等待连接的时间已经超过最大超时等待时间(MQTT_MAX_TIMES_OUT)
 *
 */
char MQTTConnect(MQTTConnectStruct_t* s1, MQTTConnACKStruct_t* s2)
{
	unsigned short temp = 0;

	MQTTInit();
	mqtt.returnData = s2;

	//报文的类型
	mqtt.messageType = MQTT_CONNECT;

	/* 可变报头 */
	//协议名
	temp = 0x0004;
	MQTTSendDataToBuff(&temp, 2, MQTT_NUM);
	MQTTSendDataToBuff("MQTT", 4, MQTT_CHAR);
	//协议级别
	MQTTSendDataToBuff(&temp, 1, MQTT_NUM);
	//连接标志
	MQTTSendDataToBuff(&s1->connectFlag, 1, MQTT_NUM);
	//保持连接时间
	MQTTSendDataToBuff(&s1->keepAliveTime, 2, MQTT_NUM);

	/* 有效载荷 */
	//客户端标识长度
	temp = strlen(s1->clientID);
	MQTTSendDataToBuff(&temp, 2, MQTT_NUM);
	//客户端标识
	MQTTSendDataToBuff((void*)s1->clientID, temp, MQTT_CHAR);
	//用户名长度
	temp = strlen(s1->userName);
	MQTTSendDataToBuff(&temp, 2, MQTT_NUM);
	//用户名
	MQTTSendDataToBuff((void*)s1->userName, temp, MQTT_CHAR);
	//密码长度
	temp = strlen(s1->password);
	MQTTSendDataToBuff(&temp, 2, MQTT_NUM);
	//密码
	MQTTSendDataToBuff((void*)s1->password, temp, MQTT_CHAR);

	/* 编码剩余长度 */
	codeRemainLengthAndSendMessageType();

	/* 计算需要发送的数据的长度并将数据发送到服务器 */
	mqtt.MQTTSendDataToServer(mqtt.sendBuff, mqtt.sendBuffPointNow - mqtt.sendBuff);

	/* 等待服务器响应 */
	temp = 0;
	while (mqtt.resultCode == MQTT_RESULT_CODE_INIT)
	{
		mqtt.MQTTDelayms(1);
		temp++;
		if (temp > MQTT_MAX_TIMES_OUT)
		{
			return 1;//超时返回
		}
	}

	return 0;
}

/** \brief  MQTT发布消息
 *
 * \param s1 指向发布消息结构体的指针
 * \param s2 指向发布确认结构体的指针(如果QoS = 0，则可以将传入NULL给s2)
 * \return 返回 0 成功
 *				返回 1 连接超时，等待连接的时间已经超过最大超时等待时间(MQTT_MAX_TIMES_OUT)
 *
 */
char MQTTPublish(MQTTPublishStruct_t* s1, MQTTPubACKStruct_t* s2)
{
	unsigned short temp = 0;

	MQTTInit();
	mqtt.returnData = s2;

	//报文的类型
	mqtt.messageType = MQTT_PUBLISH;
	mqtt.messageType |= s1->RETAIN;
	mqtt.messageType |= s1->QoS << 1;
	mqtt.messageType |= s1->DUP << 3;

	/* 可变报头 */
	//主题长度
	temp = strlen(s1->topic);
	MQTTSendDataToBuff(&temp, 2, MQTT_NUM);
	//主题
	MQTTSendDataToBuff((void*)s1->topic, temp, MQTT_CHAR);
	//报文标识符
	if (s1->QoS)
	{
		MQTTSendDataToBuff(&s1->messageID, 2, MQTT_NUM);
	}

	/* 有效载荷 */
	temp = strlen(s1->payload);
	MQTTSendDataToBuff((void*)s1->payload, temp, MQTT_CHAR);

	/* 编码剩余长度 */
	codeRemainLengthAndSendMessageType();

	/* 计算需要发送的数据的长度并将数据发送到服务器 */
	MQTTSendData(mqtt.sendBuff, mqtt.sendBuffPointNow - mqtt.sendBuff);

	//QoS = 0 服务器没有响应动作
	if (!s1->QoS)
		return 0;

	/* 等待服务器响应 */
	temp = 0;
	while (mqtt.resultCode == MQTT_RESULT_CODE_INIT)
	{
		mqtt.MQTTDelayms(1);
		temp++;
		if (temp > MQTT_MAX_TIMES_OUT)
		{
			return 1;//超时返回
		}
	}

	return 0;
}

/** \brief  MQTT订阅
 *
 * \param s1 指向订阅主题结构体的指针
 * \param s2 指向订阅确认结构体的指针
 * \return 返回 0 成功
 *				返回 1 连接超时，等待连接的时间已经超过最大超时等待时间(MQTT_MAX_TIMES_OUT)
 *
 */
char MQTTSubscribe(MQTTSubscribeStruct_t* s1, MQTTSubACKStruct_t* s2)
{
	unsigned short temp = 0;

	MQTTInit();
	mqtt.returnData = s2;

	//报文的类型
	mqtt.messageType = MQTT_SUBSCRIBE;

	/* 可变报头 */
	//报文标识符(客户端标识符)
	MQTTSendDataToBuff(&s1->messageID, 2, MQTT_NUM);

	/* 有效载荷 */
	//有效载荷长度
	temp = strlen(s1->payload);
	MQTTSendDataToBuff(&temp, 2, MQTT_NUM);
	MQTTSendDataToBuff((void*)s1->payload, temp, MQTT_CHAR);

	/* 服务质量 */
	MQTTSendDataToBuff(&s1->QoS, 1, MQTT_NUM);

	/* 编码剩余长度 */
	codeRemainLengthAndSendMessageType();

	/* 计算需要发送的数据的长度并将数据发送到服务器 */
	MQTTSendData(mqtt.sendBuff, mqtt.sendBuffPointNow - mqtt.sendBuff);

	/* 等待服务器响应 */
	temp = 0;
	while (mqtt.resultCode == MQTT_RESULT_CODE_INIT)
	{
		mqtt.MQTTDelayms(1);
		temp++;
		if (temp > MQTT_MAX_TIMES_OUT)
		{
			return 1;//超时返回
		}
	}

	return 0;
}


/** \brief  MQTT取消订阅
 *
 * \param s1 指向订阅主题结构体的指针
 * \param s2 指向订阅确认结构体的指针
 * \return 返回 0 成功
 *				返回 1 连接超时，等待连接的时间已经超过最大超时等待时间(MQTT_MAX_TIMES_OUT)
 *
 */
char MQTTUnsubscribe(MQTTUnsubscribeStruct_t* s1, MQTTSubACKStruct_t* s2)
{
	unsigned short temp = 0;

	MQTTInit();
	mqtt.returnData = s2;

	//报文的类型
	mqtt.messageType = MQTT_UNSUBSCRIBE;

	/* 可变报头 */
	//报文标识符(客户端标识符)
	MQTTSendDataToBuff(&s1->messageID, 2, MQTT_NUM);

	/* 有效载荷 */
	//有效载荷长度
	temp = strlen(s1->payload);
	MQTTSendDataToBuff(&temp, 2, MQTT_NUM);
	MQTTSendDataToBuff((void*)s1->payload, temp, MQTT_CHAR);

	/* 编码剩余长度 */
	codeRemainLengthAndSendMessageType();

	/* 计算需要发送的数据的长度并将数据发送到服务器 */
	mqtt.MQTTSendDataToServer(mqtt.sendBuff, mqtt.sendBuffPointNow - mqtt.sendBuff);

	/* 等待服务器响应 */
	temp = 0;
	while (mqtt.resultCode == MQTT_RESULT_CODE_INIT)
	{
		mqtt.MQTTDelayms(1);
		temp++;
		if (temp > MQTT_MAX_TIMES_OUT)
		{
			return 1;//超时返回
		}
	}

	return 0;
}


/** \brief  MQTT心跳响应(保持连接请求)
 *
 * \param 无
 * \return  1    保活失败/连接超时，等待连接的时间已经超过最大超时等待时间(MQTT_MAX_TIMES_OUT)
 *              0    保活成功
 *
 */
char MQTTPing(void)
{
	unsigned short temp = 0;

	MQTTInit();

	//报文的类型
	mqtt.messageType = MQTT_PINGREQ;

	/* 编码剩余长度 */
	codeRemainLengthAndSendMessageType();

	/* 计算需要发送的数据的长度并将数据发送到服务器 */
	mqtt.MQTTSendDataToServer(mqtt.sendBuff, mqtt.sendBuffPointNow - mqtt.sendBuff);

	/* 等待服务器响应 */
	temp = 0;
	while (mqtt.resultCode == MQTT_RESULT_CODE_INIT)
	{
		mqtt.MQTTDelayms(1);
		temp++;
		if (temp > MQTT_MAX_TIMES_OUT)
		{
			return 1;//超时返回
		}
	}

	return 0;
}


/** \brief  MQTT断开连接
 *
 * \param 无
 * \return  无
 *
 */
void MQTTDisconnect(void)
{
	MQTTInit();

	//报文的类型
	mqtt.messageType = MQTT_DISCONNECT;

	/* 编码剩余长度 */
	codeRemainLengthAndSendMessageType();

	/* 计算需要发送的数据的长度并将数据发送到服务器 */
	mqtt.MQTTSendDataToServer(mqtt.sendBuff, mqtt.sendBuffPointNow - mqtt.sendBuff);
}


/** \brief  设置MQTT发送数据至服务端回调函数
 *
 * \param MQTTSendDataToServer 回调函数地址
 * \return  无
 *
 */
void SetMQTTSendDataToServer(int (*MQTTSendDataToServer)(const char* data, unsigned int length))
{
	mqtt.MQTTSendDataToServer = MQTTSendDataToServer;
}


/** \brief  设置MQTT毫秒级延时回调函数
 *
 * \param MQTTDelayms 回调函数地址
 * \return  无
 *
 */
void SetMQTTDelayms(void (*MQTTDelayms)(unsigned int ms))
{
	mqtt.MQTTDelayms = MQTTDelayms;
}


/** \brief  设置MQTT接收来自服务端的数据回调函数
 *
 * \param MQTTReceiveDadaFromServer 回调函数地址
 * \return  无
 *
 */
void SetMQTTReceiveDadaFromServer(void (*MQTTReceiveDadaFromServer)(unsigned char* data, unsigned int length))
{
	mqtt.MQTTReceiveDadaFromServer = MQTTReceiveDadaFromServer;
}