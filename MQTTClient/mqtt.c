#include "mqtt.h"

MQTT_t mqtt = { 0 };

static char MQTTSendBuff[MQTT_BUFF_SIZE] = { 0 };

/** \brief ��������-----�ӿ�  
 *
 * \param   data ָ����Ҫ���͵����ݵ�ָ��
 * \param   length ���ݵĳ��� ��λ�ֽ�
 * \return  ����1���ͳɹ�������0����ʧ��
 *
 */
static int Demo_MQTTSendDataToServer(const char* data, unsigned int length)
{
	/*
		����ʹ����C���Կ⺯��ʵ�ָýӿ�
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
/** \brief ��ʱ���뺯��-----�ӿ�  
 *
 * \param   ms ������
 * \return  ��
 *
 */
static void Demo_MQTTDelayms(unsigned int ms)
{
	/*
	����ʹ����C���Կ⺯��ʵ�ָýӿ�
	���Ҫ�Լ�ʵ���������Ĵ���ɾ����
	����
	#include <time.h>
	ȥ��
	*/
	clock_t start = clock();
	while (clock() - start < ms);
}

/** \brief �ӷ��������յ�����-----�ӿ�  
 *          (δдQoS = 1/2����Ӧ������յ����Է����������ݲ��������ݣ������Ҫ����龰��Ҫ�޸Ĵ���)
 *
 * \param   data ָ����յ������ݵ�ָ��
 * \param   length ���ݵĳ��� ��λ�ֽ�
 * \return  ��
 *
 */
void Demo_MQTTReceiveDadaFromServer(unsigned char* data, unsigned int length)
{
	switch (*data)
	{
		/* ȷ���������� */
	case MQTT_CONNACK:
		//����Ұָ��
		if (mqtt.returnData)
		{
			//��ȡ��ǰ�Ự��־
			((MQTTConnACKStruct_t*)mqtt.returnData)->sp = *(data + 2);
			//��ȡ���ӷ�����
			((MQTTConnACKStruct_t*)mqtt.returnData)->code = *(data + 3);
		}
		mqtt.resultCode = ~MQTT_RESULT_CODE_INIT;
		break;

		/* ȷ�Ϸ�����Ϣ����(δдQoS = 1/2����Ӧ����) */
	case MQTT_PUBACK:
	case MQTT_PUBREC:
	case MQTT_PUBREL:
	case MQTT_PUBCOMP:
		mqtt.resultCode = ~MQTT_RESULT_CODE_INIT;
		break;

		/* ����ȷ�� */
	case MQTT_SUBACK:
		//����Ұָ��
		if (mqtt.returnData)
		{
			((MQTTSubACKStruct_t*)mqtt.returnData)->messageID = *(data + 2) * 256 + *(data + 3);
			((MQTTSubACKStruct_t*)mqtt.returnData)->code = *(data + 4);
		}
		mqtt.resultCode = ~MQTT_RESULT_CODE_INIT;
		break;

		/* ȡ������ȷ�� */
	case MQTT_UNSUBACK:
		//����Ұָ��
		if (mqtt.returnData)
		{
			((MQTTUnsubACKStruct_t*)mqtt.returnData)->messageID = *(data + 2) * 256 + *(data + 3);
		}
		mqtt.resultCode = ~MQTT_RESULT_CODE_INIT;
		break;

		/* ������Ӧ */
	case MQTT_PINGRESP:
		mqtt.resultCode = ~MQTT_RESULT_CODE_INIT;
		break;

		/*
			�����������˷�����Ϣ
			����������Լ���ҵ�����
		*/
	default:
		break;
	}

	//��ջ�����
	memset((void*)data, 0, length);
}

/** \brief  ������д��MQTT�ķ��ͻ�����
 * \param   data ָ��Ҫд�������Դ������ǿ��ת��Ϊ void* ָ�롣
 * \param   n Ҫ��д����ֽ�����
 * \param   dataType  ˵������Դ data ���������ͣ�������ַ�����CHAR(1)����������NUM(0)��
 * \return   �ޡ�
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

/** \brief  ����ʣ�೤�Ȳ����ͻ�����д�뱨�ĵ�����
 *
 * \param   ��
 * \return  ��
 *
 */
static void codeRemainLengthAndSendMessageType(void)
{
	char temp[4] = { 0 };
	unsigned char length = 0;
	char encodedByte = 0;
	unsigned int X = 0;

	//����ʣ�೤��
	X = mqtt.sendBuffPointNow - mqtt.sendBuff - 1;
	//����
	do
	{
		encodedByte = X % 128;
		X = X / 128;
		if (X > 0)
		{
			encodedByte = encodedByte | 128;
		}
		//�õ��ѱ����ʣ�೤��
		temp[length] = encodedByte;
		//��¼�����ʣ�೤��ռ�õ��ֽ���
		length++;
	} while (X > 0);

	//��ת
	while (length--)
	{
		*mqtt.sendBuff = temp[length];
		mqtt.sendBuff--;
	}

	//д�뱨�ĵ�����
	*mqtt.sendBuff = mqtt.messageType;
}

/** \brief  ����ʣ�೤��
 *
 * \param   data ָ���ѱ����ʣ�೤��������׸�Ԫ�ص�ָ��
 * \return  ��
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

	//�õ��ѽ����ʣ�೤��
	mqtt.remainLength = value;
}

/** \brief  MQTT��ʼ��
 *
 * \param   ��
 * \return  ��
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

/** \brief  MQTT���ӱ���
 *
 * \param s1 ָ�����ӱ��Ľṹ���ָ��
 * \param s2 ָ��ȷ����������ṹ���ָ��
 * \return ���� 0 ���ӳɹ�
 *				���� 1 ���ӳ�ʱ���ȴ����ӵ�ʱ���Ѿ��������ʱ�ȴ�ʱ��(MQTT_MAX_TIMES_OUT)
 *
 */
char MQTTConnect(MQTTConnectStruct_t* s1, MQTTConnACKStruct_t* s2)
{
	unsigned short temp = 0;

	MQTTInit();
	mqtt.returnData = s2;

	//���ĵ�����
	mqtt.messageType = MQTT_CONNECT;

	/* �ɱ䱨ͷ */
	//Э����
	temp = 0x0004;
	MQTTSendDataToBuff(&temp, 2, MQTT_NUM);
	MQTTSendDataToBuff("MQTT", 4, MQTT_CHAR);
	//Э�鼶��
	MQTTSendDataToBuff(&temp, 1, MQTT_NUM);
	//���ӱ�־
	MQTTSendDataToBuff(&s1->connectFlag, 1, MQTT_NUM);
	//��������ʱ��
	MQTTSendDataToBuff(&s1->keepAliveTime, 2, MQTT_NUM);

	/* ��Ч�غ� */
	//�ͻ��˱�ʶ����
	temp = strlen(s1->clientID);
	MQTTSendDataToBuff(&temp, 2, MQTT_NUM);
	//�ͻ��˱�ʶ
	MQTTSendDataToBuff((void*)s1->clientID, temp, MQTT_CHAR);
	//�û�������
	temp = strlen(s1->userName);
	MQTTSendDataToBuff(&temp, 2, MQTT_NUM);
	//�û���
	MQTTSendDataToBuff((void*)s1->userName, temp, MQTT_CHAR);
	//���볤��
	temp = strlen(s1->password);
	MQTTSendDataToBuff(&temp, 2, MQTT_NUM);
	//����
	MQTTSendDataToBuff((void*)s1->password, temp, MQTT_CHAR);

	/* ����ʣ�೤�� */
	codeRemainLengthAndSendMessageType();

	/* ������Ҫ���͵����ݵĳ��Ȳ������ݷ��͵������� */
	mqtt.MQTTSendDataToServer(mqtt.sendBuff, mqtt.sendBuffPointNow - mqtt.sendBuff);

	/* �ȴ���������Ӧ */
	temp = 0;
	while (mqtt.resultCode == MQTT_RESULT_CODE_INIT)
	{
		mqtt.MQTTDelayms(1);
		temp++;
		if (temp > MQTT_MAX_TIMES_OUT)
		{
			return 1;//��ʱ����
		}
	}

	return 0;
}

/** \brief  MQTT������Ϣ
 *
 * \param s1 ָ�򷢲���Ϣ�ṹ���ָ��
 * \param s2 ָ�򷢲�ȷ�Ͻṹ���ָ��(���QoS = 0������Խ�����NULL��s2)
 * \return ���� 0 �ɹ�
 *				���� 1 ���ӳ�ʱ���ȴ����ӵ�ʱ���Ѿ��������ʱ�ȴ�ʱ��(MQTT_MAX_TIMES_OUT)
 *
 */
char MQTTPublish(MQTTPublishStruct_t* s1, MQTTPubACKStruct_t* s2)
{
	unsigned short temp = 0;

	MQTTInit();
	mqtt.returnData = s2;

	//���ĵ�����
	mqtt.messageType = MQTT_PUBLISH;
	mqtt.messageType |= s1->RETAIN;
	mqtt.messageType |= s1->QoS << 1;
	mqtt.messageType |= s1->DUP << 3;

	/* �ɱ䱨ͷ */
	//���ⳤ��
	temp = strlen(s1->topic);
	MQTTSendDataToBuff(&temp, 2, MQTT_NUM);
	//����
	MQTTSendDataToBuff((void*)s1->topic, temp, MQTT_CHAR);
	//���ı�ʶ��
	if (s1->QoS)
	{
		MQTTSendDataToBuff(&s1->messageID, 2, MQTT_NUM);
	}

	/* ��Ч�غ� */
	temp = strlen(s1->payload);
	MQTTSendDataToBuff((void*)s1->payload, temp, MQTT_CHAR);

	/* ����ʣ�೤�� */
	codeRemainLengthAndSendMessageType();

	/* ������Ҫ���͵����ݵĳ��Ȳ������ݷ��͵������� */
	MQTTSendData(mqtt.sendBuff, mqtt.sendBuffPointNow - mqtt.sendBuff);

	//QoS = 0 ������û����Ӧ����
	if (!s1->QoS)
		return 0;

	/* �ȴ���������Ӧ */
	temp = 0;
	while (mqtt.resultCode == MQTT_RESULT_CODE_INIT)
	{
		mqtt.MQTTDelayms(1);
		temp++;
		if (temp > MQTT_MAX_TIMES_OUT)
		{
			return 1;//��ʱ����
		}
	}

	return 0;
}

/** \brief  MQTT����
 *
 * \param s1 ָ��������ṹ���ָ��
 * \param s2 ָ����ȷ�Ͻṹ���ָ��
 * \return ���� 0 �ɹ�
 *				���� 1 ���ӳ�ʱ���ȴ����ӵ�ʱ���Ѿ��������ʱ�ȴ�ʱ��(MQTT_MAX_TIMES_OUT)
 *
 */
char MQTTSubscribe(MQTTSubscribeStruct_t* s1, MQTTSubACKStruct_t* s2)
{
	unsigned short temp = 0;

	MQTTInit();
	mqtt.returnData = s2;

	//���ĵ�����
	mqtt.messageType = MQTT_SUBSCRIBE;

	/* �ɱ䱨ͷ */
	//���ı�ʶ��(�ͻ��˱�ʶ��)
	MQTTSendDataToBuff(&s1->messageID, 2, MQTT_NUM);

	/* ��Ч�غ� */
	//��Ч�غɳ���
	temp = strlen(s1->payload);
	MQTTSendDataToBuff(&temp, 2, MQTT_NUM);
	MQTTSendDataToBuff((void*)s1->payload, temp, MQTT_CHAR);

	/* �������� */
	MQTTSendDataToBuff(&s1->QoS, 1, MQTT_NUM);

	/* ����ʣ�೤�� */
	codeRemainLengthAndSendMessageType();

	/* ������Ҫ���͵����ݵĳ��Ȳ������ݷ��͵������� */
	MQTTSendData(mqtt.sendBuff, mqtt.sendBuffPointNow - mqtt.sendBuff);

	/* �ȴ���������Ӧ */
	temp = 0;
	while (mqtt.resultCode == MQTT_RESULT_CODE_INIT)
	{
		mqtt.MQTTDelayms(1);
		temp++;
		if (temp > MQTT_MAX_TIMES_OUT)
		{
			return 1;//��ʱ����
		}
	}

	return 0;
}


/** \brief  MQTTȡ������
 *
 * \param s1 ָ��������ṹ���ָ��
 * \param s2 ָ����ȷ�Ͻṹ���ָ��
 * \return ���� 0 �ɹ�
 *				���� 1 ���ӳ�ʱ���ȴ����ӵ�ʱ���Ѿ��������ʱ�ȴ�ʱ��(MQTT_MAX_TIMES_OUT)
 *
 */
char MQTTUnsubscribe(MQTTUnsubscribeStruct_t* s1, MQTTSubACKStruct_t* s2)
{
	unsigned short temp = 0;

	MQTTInit();
	mqtt.returnData = s2;

	//���ĵ�����
	mqtt.messageType = MQTT_UNSUBSCRIBE;

	/* �ɱ䱨ͷ */
	//���ı�ʶ��(�ͻ��˱�ʶ��)
	MQTTSendDataToBuff(&s1->messageID, 2, MQTT_NUM);

	/* ��Ч�غ� */
	//��Ч�غɳ���
	temp = strlen(s1->payload);
	MQTTSendDataToBuff(&temp, 2, MQTT_NUM);
	MQTTSendDataToBuff((void*)s1->payload, temp, MQTT_CHAR);

	/* ����ʣ�೤�� */
	codeRemainLengthAndSendMessageType();

	/* ������Ҫ���͵����ݵĳ��Ȳ������ݷ��͵������� */
	mqtt.MQTTSendDataToServer(mqtt.sendBuff, mqtt.sendBuffPointNow - mqtt.sendBuff);

	/* �ȴ���������Ӧ */
	temp = 0;
	while (mqtt.resultCode == MQTT_RESULT_CODE_INIT)
	{
		mqtt.MQTTDelayms(1);
		temp++;
		if (temp > MQTT_MAX_TIMES_OUT)
		{
			return 1;//��ʱ����
		}
	}

	return 0;
}


/** \brief  MQTT������Ӧ(������������)
 *
 * \param ��
 * \return  1    ����ʧ��/���ӳ�ʱ���ȴ����ӵ�ʱ���Ѿ��������ʱ�ȴ�ʱ��(MQTT_MAX_TIMES_OUT)
 *              0    ����ɹ�
 *
 */
char MQTTPing(void)
{
	unsigned short temp = 0;

	MQTTInit();

	//���ĵ�����
	mqtt.messageType = MQTT_PINGREQ;

	/* ����ʣ�೤�� */
	codeRemainLengthAndSendMessageType();

	/* ������Ҫ���͵����ݵĳ��Ȳ������ݷ��͵������� */
	mqtt.MQTTSendDataToServer(mqtt.sendBuff, mqtt.sendBuffPointNow - mqtt.sendBuff);

	/* �ȴ���������Ӧ */
	temp = 0;
	while (mqtt.resultCode == MQTT_RESULT_CODE_INIT)
	{
		mqtt.MQTTDelayms(1);
		temp++;
		if (temp > MQTT_MAX_TIMES_OUT)
		{
			return 1;//��ʱ����
		}
	}

	return 0;
}


/** \brief  MQTT�Ͽ�����
 *
 * \param ��
 * \return  ��
 *
 */
void MQTTDisconnect(void)
{
	MQTTInit();

	//���ĵ�����
	mqtt.messageType = MQTT_DISCONNECT;

	/* ����ʣ�೤�� */
	codeRemainLengthAndSendMessageType();

	/* ������Ҫ���͵����ݵĳ��Ȳ������ݷ��͵������� */
	mqtt.MQTTSendDataToServer(mqtt.sendBuff, mqtt.sendBuffPointNow - mqtt.sendBuff);
}


/** \brief  ����MQTT��������������˻ص�����
 *
 * \param MQTTSendDataToServer �ص�������ַ
 * \return  ��
 *
 */
void SetMQTTSendDataToServer(int (*MQTTSendDataToServer)(const char* data, unsigned int length))
{
	mqtt.MQTTSendDataToServer = MQTTSendDataToServer;
}


/** \brief  ����MQTT���뼶��ʱ�ص�����
 *
 * \param MQTTDelayms �ص�������ַ
 * \return  ��
 *
 */
void SetMQTTDelayms(void (*MQTTDelayms)(unsigned int ms))
{
	mqtt.MQTTDelayms = MQTTDelayms;
}


/** \brief  ����MQTT�������Է���˵����ݻص�����
 *
 * \param MQTTReceiveDadaFromServer �ص�������ַ
 * \return  ��
 *
 */
void SetMQTTReceiveDadaFromServer(void (*MQTTReceiveDadaFromServer)(unsigned char* data, unsigned int length))
{
	mqtt.MQTTReceiveDadaFromServer = MQTTReceiveDadaFromServer;
}