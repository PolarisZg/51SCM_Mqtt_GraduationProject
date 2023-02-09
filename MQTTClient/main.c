#include "mqtt.h"
#include <windows.h>
#include <stdio.h>
#pragma comment (lib, "ws2_32.lib")

/* 阿里云相关配置 */
// 三元组
#define Product_Key "a62xpdiPmWd"
#define Device_Name "51SecondTest"
#define Device_Secret "b3fe731d823457e0a0bc4447b51d1ab0"
// 加密方式
#define Secret_Mode "hmacsha256"
// 密码
#define PASSWORD "a7d25a0a1c6f0e39a952f56d2548ac6191fb9b806450975d7c1517386c5d6d96"

// 服务器地址
#define SERVER_ADDR Product_Key ".iot-as-mqtt.ap-northeast-1.aliyuncs.com"
// 服务器端口
#define SERVER_PORT 1883

// 客户端标识符
#define CLIENT_ID Product_Key "." Device_Name "|securemode=2,signmethod=" Secret_Mode ",timestamp=1674608405027|"
// 用户名
#define USER_NAME Device_Name "&" Product_Key
// 订阅主题
#define SUBSCRIBE_TOPIC	"/" Product_Key "/" Device_Name "/user/get"
// 主题消息ID
#define TOPIC_MESSAGE_ID (0xAACCU)
// 发布消息主题
#define PUBLISH_TOPIC	"/" Product_Key "/" Device_Name "/user/update"
// 发布消息的有效载荷
#define PUBLISH_PAYLOAD	"{\"method\":\"thing.event.property.post\",\"id\":\"000000000\",\"params\":{\"LightSwitch\":1},\"\version\":\"1.0.0\"}"

/* 客户端句柄 */
SOCKET sock;

/* 发送数据缓冲区 */
char payloadBuff[MQTT_BUFF_SIZE] = { 0 };
unsigned payloadSize = 0;

/* 接收来自服务端的数据缓冲区 */
char receiveBuff[MQTT_BUFF_SIZE] = { 0 };


/** \brief 发送数据-----接口
 *
 * \param   data 指向需要发送的数据的指针
 * \param   length 数据的长度 单位字节
 * \return  返回1则发送成功，返回0则发送失败
 *
 */
int MQTTSendData(const char* data, unsigned int length)
{
	/* 发送数据到服务端 */
	if (send(sock, data, length, 0) > 0)
	{
		return 1;
	}
	return 0;
}


int Connect(void)
{
	/* MQTT连接报文 */
	MQTTConnectStruct_t s1 = { 0 };
	MQTTConnACKStruct_t s2 = { 0 };
	s1.connectFlag = 0xC2;//连接标志
	s1.keepAliveTime = MQTT_KEEP_ALIVE;//保活时间
	s1.clientID = CLIENT_ID;//填入客户端标识符
	s1.userName = USER_NAME;//用户名标识符
	s1.password = PASSWORD;//密码
	return MQTTConnect(&s1, &s2);//连接阿里云

	//unsigned int receiveLength = recv(sock, receiveBuff, 200, 0);

	///* 输出CONNECACK报文 */
	//printf("CONNECACK\n");
	//for (size_t i = 0; i < receiveLength; i++)
	//{
	//	printf("%02X ", receiveBuff[i]);
	//}
	//printf("\n");
}

int Publish(void)
{
	MQTTPublishStruct_t s1 = { 0 };
	MQTTPubACKStruct_t s2 = { 0 };

	s1.DUP = 0;
	s1.QoS = 0;//QoS = 0 故没有报文标识符 与 服务端不会发送PUBLISHACK
	//一般来说QoS = 0已经够用了，再加上阿里云有 post_reply 主题就能知道发布消息是否成功
	s1.RETAIN = 0;
	//主题
	s1.topic = PUBLISH_TOPIC;//填写TOPIC
	//有效载荷
	s1.payload = payloadBuff;//填写有效载荷
	s1.payloadSize = payloadSize;
	return MQTTPublish(&s1, &s2);

	//unsigned int receiveLength = recv(sock, receiveBuff, 200, 0);
	///* 输出PUBLISHACK报文 */
	////因为阿里云有post_reply主题所以会回传本条消息发送的状态
	//printf("PUBLISHACK\n");
	//for (size_t i = 0; i < receiveLength; i++)
	//{
	//	printf("%c", receiveBuff[i]);
	//}
	//printf("\n");
}


int main()
{
	/* 初始化DLL */
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		goto end;

	/* 创建TCP套接字 */
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
		goto end;

	/* 解析阿里云域名获取IP地址 */
	HOSTENT* host = gethostbyname(SERVER_ADDR);
	if (!host)
		goto end;
	printf("ip:%s\n", inet_ntoa(*(IN_ADDR*)*host->h_addr_list));//输出域名对应的ip地址

	SOCKADDR_IN servAddr = { 0 };
	servAddr.sin_family = AF_INET;//IPV4
	servAddr.sin_addr = *(IN_ADDR*)*host->h_addr_list;//IP地址
	servAddr.sin_port = htons(SERVER_PORT);//端口
	if (connect(sock, (SOCKADDR*)&servAddr, sizeof(servAddr)))//连接阿里云
	{
		printf("error:%d\n", WSAGetLastError());
		goto end;
	}

	/* 设置MQTT发送数据至服务端接口 */
	SetMQTTSendDataToServer(MQTTSendData);

	/* 连接CONNECT */
	printf("Connect = %d\n", Connect());
	Sleep(100);

	/* 发布消息PUBLISH */
	{
		char tempPayload[] = { "202302091918" };
		strDataPublish(tempPayload);
	}
	printf("Publish = %d\n", Publish());
	Sleep(100);




end:
	/* 终止 DLL 的使用 */
	WSACleanup();


	system("pause");

	return 0;
}

int strDataPublish(char* tempPayload)
{
	int len = strlen(tempPayload);
	char* tPnow = tempPayload;
	char* Pnow = payloadBuff;
	memset(payloadBuff, 0, sizeof(payloadBuff));
	while (len--)
	{
		*Pnow++ = *tPnow++;
	}
	payloadSize = len;
	return 0;
}

int payloadToBuff(MQTTPayload payload)
{
	memset(payloadBuff, 0, sizeof(payloadBuff));
	return 0;
}