#include "mqtt.h"
#include <windows.h>
#include <stdio.h>
#pragma comment (lib, "ws2_32.lib")

/* ������������� */
// ��Ԫ��
#define Product_Key "a62xpdiPmWd"
#define Device_Name "51SecondTest"
#define Device_Secret "b3fe731d823457e0a0bc4447b51d1ab0"
// ���ܷ�ʽ
#define Secret_Mode "hmacsha256"
// ����
#define PASSWORD "a7d25a0a1c6f0e39a952f56d2548ac6191fb9b806450975d7c1517386c5d6d96"

// ��������ַ
#define SERVER_ADDR Product_Key ".iot-as-mqtt.ap-northeast-1.aliyuncs.com"
// �������˿�
#define SERVER_PORT 1883

// �ͻ��˱�ʶ��
#define CLIENT_ID Product_Key "." Device_Name "|securemode=2,signmethod=" Secret_Mode ",timestamp=1674608405027|"
// �û���
#define USER_NAME Device_Name "&" Product_Key
// ��������
#define SUBSCRIBE_TOPIC	"/" Product_Key "/" Device_Name "/user/get"
// ������ϢID
#define TOPIC_MESSAGE_ID (0xAACCU)
// ������Ϣ����
#define PUBLISH_TOPIC	"/" Product_Key "/" Device_Name "/user/update"
// ������Ϣ����Ч�غ�
#define PUBLISH_PAYLOAD	"{\"method\":\"thing.event.property.post\",\"id\":\"000000000\",\"params\":{\"LightSwitch\":1},\"\version\":\"1.0.0\"}"

/* �ͻ��˾�� */
SOCKET sock;

/* �������ݻ����� */
char payloadBuff[MQTT_BUFF_SIZE] = { 0 };
unsigned payloadSize = 0;

/* �������Է���˵����ݻ����� */
char receiveBuff[MQTT_BUFF_SIZE] = { 0 };


/** \brief ��������-----�ӿ�
 *
 * \param   data ָ����Ҫ���͵����ݵ�ָ��
 * \param   length ���ݵĳ��� ��λ�ֽ�
 * \return  ����1���ͳɹ�������0����ʧ��
 *
 */
int MQTTSendData(const char* data, unsigned int length)
{
	/* �������ݵ������ */
	if (send(sock, data, length, 0) > 0)
	{
		return 1;
	}
	return 0;
}


int Connect(void)
{
	/* MQTT���ӱ��� */
	MQTTConnectStruct_t s1 = { 0 };
	MQTTConnACKStruct_t s2 = { 0 };
	s1.connectFlag = 0xC2;//���ӱ�־
	s1.keepAliveTime = MQTT_KEEP_ALIVE;//����ʱ��
	s1.clientID = CLIENT_ID;//����ͻ��˱�ʶ��
	s1.userName = USER_NAME;//�û�����ʶ��
	s1.password = PASSWORD;//����
	return MQTTConnect(&s1, &s2);//���Ӱ�����

	//unsigned int receiveLength = recv(sock, receiveBuff, 200, 0);

	///* ���CONNECACK���� */
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
	s1.QoS = 0;//QoS = 0 ��û�б��ı�ʶ�� �� ����˲��ᷢ��PUBLISHACK
	//һ����˵QoS = 0�Ѿ������ˣ��ټ��ϰ������� post_reply �������֪��������Ϣ�Ƿ�ɹ�
	s1.RETAIN = 0;
	//����
	s1.topic = PUBLISH_TOPIC;//��дTOPIC
	//��Ч�غ�
	s1.payload = payloadBuff;//��д��Ч�غ�
	s1.payloadSize = payloadSize;
	return MQTTPublish(&s1, &s2);

	//unsigned int receiveLength = recv(sock, receiveBuff, 200, 0);
	///* ���PUBLISHACK���� */
	////��Ϊ��������post_reply�������Ի�ش�������Ϣ���͵�״̬
	//printf("PUBLISHACK\n");
	//for (size_t i = 0; i < receiveLength; i++)
	//{
	//	printf("%c", receiveBuff[i]);
	//}
	//printf("\n");
}


int main()
{
	/* ��ʼ��DLL */
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		goto end;

	/* ����TCP�׽��� */
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
		goto end;

	/* ����������������ȡIP��ַ */
	HOSTENT* host = gethostbyname(SERVER_ADDR);
	if (!host)
		goto end;
	printf("ip:%s\n", inet_ntoa(*(IN_ADDR*)*host->h_addr_list));//���������Ӧ��ip��ַ

	SOCKADDR_IN servAddr = { 0 };
	servAddr.sin_family = AF_INET;//IPV4
	servAddr.sin_addr = *(IN_ADDR*)*host->h_addr_list;//IP��ַ
	servAddr.sin_port = htons(SERVER_PORT);//�˿�
	if (connect(sock, (SOCKADDR*)&servAddr, sizeof(servAddr)))//���Ӱ�����
	{
		printf("error:%d\n", WSAGetLastError());
		goto end;
	}

	/* ����MQTT��������������˽ӿ� */
	SetMQTTSendDataToServer(MQTTSendData);

	/* ����CONNECT */
	printf("Connect = %d\n", Connect());
	Sleep(100);

	/* ������ϢPUBLISH */
	{
		char tempPayload[] = { "202302091918" };
		strDataPublish(tempPayload);
	}
	printf("Publish = %d\n", Publish());
	Sleep(100);




end:
	/* ��ֹ DLL ��ʹ�� */
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