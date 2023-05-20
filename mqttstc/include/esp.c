#include "esp.h"

/*----------uart4 部分----------*/
#define BAUD 115200UL             //串口波特率
#define S4_S0 0x04              //P_SW2.2
#define S4RI  0x01              //S4CON.0
#define S4TI  0x02              //S4CON.1
#define S4RB8 0x04              //S4CON.2
#define S4TB8 0x08              //S4CON.3

bit uart4_busy;
unsigned char esp_race_buf[100] = {0};
unsigned int esp_race_count = 0;
char MQTTConnectData[] = {0x10 ,0xBC ,0x01 ,0x00 ,0x04 ,0x4D ,0x51 ,0x54 ,0x54 ,0x04 ,0xC2 ,0x01 ,0x2C ,0x00 ,0x54 ,0x61 ,0x36 ,0x32 ,0x78 ,0x70 ,0x64 ,0x69 ,0x50 ,0x6D ,0x57 ,0x64 ,0x2E ,0x35 ,0x31 ,0x53 ,0x65 ,0x63 ,0x6F ,0x6E ,0x64 ,0x54 ,0x65 ,0x73 ,0x74 ,0x7C ,0x73 ,0x65 ,0x63 ,0x75 ,0x72 ,0x65 ,0x6D ,0x6F ,0x64 ,0x65 ,0x3D ,0x32 ,0x2C ,0x73 ,0x69 ,0x67 ,0x6E ,0x6D ,0x65 ,0x74 ,0x68 ,0x6F ,0x64 ,0x3D ,0x68 ,0x6D ,0x61 ,0x63 ,0x73 ,0x68 ,0x61 ,0x32 ,0x35 ,0x36 ,0x2C ,0x74 ,0x69 ,0x6D ,0x65 ,0x73 ,0x74 ,0x61 ,0x6D ,0x70 ,0x3D ,0x31 ,0x36 ,0x37 ,0x34 ,0x36 ,0x30 ,0x38 ,0x34 ,0x30 ,0x35 ,0x30 ,0x32 ,0x37 ,0x7C ,0x00 ,0x18 ,0x35 ,0x31 ,0x53 ,0x65 ,0x63 ,0x6F ,0x6E ,0x64 ,0x54 ,0x65 ,0x73 ,0x74 ,0x26 ,0x61 ,0x36 ,0x32 ,0x78 ,0x70 ,0x64 ,0x69 ,0x50 ,0x6D ,0x57 ,0x64 ,0x00 ,0x40 ,0x61 ,0x37 ,0x64 ,0x32 ,0x35 ,0x61 ,0x30 ,0x61 ,0x31 ,0x63 ,0x36 ,0x66 ,0x30 ,0x65 ,0x33 ,0x39 ,0x61 ,0x39 ,0x35 ,0x32 ,0x66 ,0x35 ,0x36 ,0x64 ,0x32 ,0x35 ,0x34 ,0x38 ,0x61 ,0x63 ,0x36 ,0x31 ,0x39 ,0x31 ,0x66 ,0x62 ,0x39 ,0x62 ,0x38 ,0x30 ,0x36 ,0x34 ,0x35 ,0x30 ,0x39 ,0x37 ,0x35 ,0x64 ,0x37 ,0x63 ,0x31 ,0x35 ,0x31 ,0x37 ,0x33 ,0x38 ,0x36 ,0x63 ,0x35 ,0x64 ,0x36 ,0x64 ,0x39 ,0x36};
char MQTTPublishData[] = {0x30 ,0x2E ,0x00 ,0x25 ,0x2F ,0x61 ,0x36 ,0x32 ,0x78 ,0x70 ,0x64 ,0x69 ,0x50 ,0x6D ,0x57 ,0x64 ,0x2F ,0x35 ,0x31 ,0x53 ,0x65 ,0x63 ,0x6F ,0x6E ,0x64 ,0x54 ,0x65 ,0x73 ,0x74 ,0x2F ,0x75 ,0x73 ,0x65 ,0x72 ,0x2F ,0x75 ,0x70 ,0x64 ,0x61 ,0x74 ,0x65 ,0x23 ,0x23 ,0x23 ,0x23 ,0x23 ,0x23 ,0x23};
char MQTTPingData[] = {0xC0 ,0x00};
char MQTTDisconnectData[] = {0xE0 ,0x00};

void Uart4Init(void)
{
    P_SW2 |= S4_S0;             //S4_S0=1 (P5.2/RxD4_2, P5.3/TxD4_2)
    S4CON = 0x50;               //8位可变波特率

    T4L = (65536 - (MAIN_Fosc/4/BAUD));   //设置波特率重装值
    T4H = (65536 - (MAIN_Fosc/4/BAUD))>>8;
    T4T3M |= 0x20;              //定时器4为1T模式
    T4T3M |= 0x80;              //定时器4开始计时

    IE2 = 0x10;                 //使能串口4中断
    EA = 1;
}



void Uart4() interrupt 18
{
	if (S4CON & S4RI)
	{
		S4CON &= ~S4RI;         //清除S4RI位
			
		esp_race_buf[esp_race_count] = S4BUF;	
		esp_race_count++;
		if(esp_race_count>=100)
		{
			esp_race_count = 0;
		}
	}
  if (S4CON & S4TI)
  {
		S4CON &= ~S4TI;         //清除S4TI位
		uart4_busy = 0;               //清忙标志
  }
}


void Uart4SendData(unsigned char dat)
{
    
	S4CON &= ~S4TB8;        //设置校验位为0
	uart4_busy = 1;
	S4BUF = dat;
	while (uart4_busy);
}

/*----------------------------
发送字符串
----------------------------*/
void Uart4SendString(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        Uart4SendData(*s++);         //发送当前字符
    }
}

/*----------esp 部分----------*/

/* 初始化 esp8266 */
void espInit(char *SSID, char *passwd, char *SIP, char *SPORT)
{
    Uart4Init();
	uart1SendString("u4 init end\r\n");

/*----------*/
	Uart4SendString("AT+CWJAP=\"");
    Uart4SendString(SSID);
    Uart4SendString("\",\"");
    Uart4SendString(passwd);
    Uart4SendString("\"\r\n");

	uart1SendString("AT+CWJAP=\"");
    uart1SendString(SSID);
    uart1SendString("\",\"");
    uart1SendString(passwd);
    uart1SendString("\"\r\n");
	delay_10s();
	uart1SendString("race:\r\n");//串口1发送信息
	esp_race_buf[esp_race_count] = '\0';
	uart1SendString(esp_race_buf);
	delay_ms(1000);//延时1秒

	WDT_CONTR = 0x17;
	
/*----------*/
	esp_race_count = 0;//串口4接收计数清零
	Uart4SendString("AT+CWMODE=1\r\n");
	uart1SendString("AT+CWMODE=1\r\n");
	delay_10s();
	uart1SendString("race:\r\n");//串口1发送信息
	esp_race_buf[esp_race_count] = '\0';
	uart1SendString(esp_race_buf);//将串口4收到的数据通过串口1发送给电脑
	delay_ms(2000);

	WDT_CONTR = 0x17;
	
/*----------*/
	esp_race_count = 0;//串口4接收计数清零
	Uart4SendString("AT+CIPMUX=0\r\n");//串口4发送AT命令给WIFI模块
	uart1SendString("AT+CIPMUX=0\r\n");
	delay_10s();//
	uart1SendString("race:\r\n");//串口1发送信息
	esp_race_buf[esp_race_count] = '\0';
	uart1SendString(esp_race_buf);//将串口4收到的数据通过串口1发送给电脑
	delay_ms(2000);

	WDT_CONTR = 0x17;
	
/*----------*/
	esp_race_count = 0;//串口4接收计数清零

	Uart4SendString("AT+CIPSTART=\"TCP\",\"");
    Uart4SendString(SIP);
    Uart4SendString("\",");
    Uart4SendString(SPORT);
    Uart4SendString("\r\n");

	uart1SendString("send:\r\nAT+CIPSTART=\"TCP\",\"");
    uart1SendString(SIP);
    uart1SendString("\",");
    uart1SendString(SPORT);
    uart1SendString("\r\n");
	delay_10s();//
	uart1SendString("race:\r\n");//串口1发送信息
	esp_race_buf[esp_race_count] = '\0';
	uart1SendString(esp_race_buf);//将串口4收到的数据通过串口1发送给电脑

	WDT_CONTR = 0x17;

	delay_ms(2000);//

    uart1SendString("esp init end \r\n");
}

// void espSend(unsigned int length, char *wifi_send_buff)
// {
//     char templength = length;

//     char buf[16];
//     int i = 0;

//     //uart1SendString("\r\nuse espSend\r\nlength is %d \r\n",length);

//     esp_race_count = 0;//串口4接收计数清零

// 	Uart4SendString("send:\r\nAT+CIPSEND=");

//     while (length > 0) {
//         buf[i++] = length % 10 + '0';  // 将每一位数字转换为字符
//         length /= 10;
//     }

//     while (i-- > 0) {
//         Uart4SendData(buf[i]);  // 逆序输出每一位数字字符
//         uart1SendChar(buf[i]);
// 		uart1SendString("\r\n");
//     }

//     Uart4SendString("\r\n");

// 	while(1){
// 		if(esp_race_count - 2 >= 0 && esp_race_buf[esp_race_count - 2] == '>')
// 			break;
// 	}

//     uart1SendString("\r\nrace:\r\n");//
//     esp_race_buf[esp_race_count] = '\0';
// 	uart1SendString(esp_race_buf);

//     esp_race_count = 0;//串口4接收计数清零
//     i = 0;
//     uart1SendString("\r\nwifi_send_buff\r\n");
//     while(i < templength)
//     {
//         Uart4SendData(wifi_send_buff[i]);
//         //uart1SendString("%c",wifi_send_buff[i]);
//         i++;
//     }

// 	Uart4SendString(wifi_send_buff);//串口4发送需要发送的数据   数据在wifi_send_buf 数组中   数组的定义在最上面
// 	delay_ms(1000);//

//     Uart4SendString("+++");
// 	delay_ms(2000);
// }

unsigned char MQTTConnect()
{

	unsigned int num = 0;
	esp_race_count = 0;
	delay_ms(100);

	Uart4SendString("AT+CIPSEND=191\r\n");

	while(1){
		if(esp_race_count - 2 >= 0 && esp_race_buf[esp_race_count - 2] == '>')
			break;
	}
	delay_ms(1000);
	esp_race_count = 0;

	Uart4SendData(0x10);

	for(num = 1;num < 191;num++)
	{
		Uart4SendData(MQTTConnectData[num]);
	}
	delay_ms(1000);
	WDT_CONTR = 0X37; //喂狗操作 
	
	uart1SendChar('0'+esp_race_count);
	Uart1SendData(esp_race_buf,esp_race_count);
  //Uart4SendString("+++");
	
	delay_ms(200);
	WDT_CONTR = 0X37; //喂狗操作 
	delay_ms(200);
	WDT_CONTR = 0X37; //喂狗操作 
	delay_ms(200);
	WDT_CONTR = 0X37; //喂狗操作 
	delay_ms(200);
	WDT_CONTR = 0X37; //喂狗操作 
	delay_ms(200);
	WDT_CONTR = 0X37; //喂狗操作 
	return 1;
}

unsigned char MQTTPublish(char *mqttdata)
{
	unsigned int num = 0;
	esp_race_count = 0;

	MQTTPublishData[41] = mqttdata[0];
	MQTTPublishData[42] = mqttdata[1];
	MQTTPublishData[43] = mqttdata[2];
	MQTTPublishData[44] = mqttdata[3];
	MQTTPublishData[45] = mqttdata[4];
	MQTTPublishData[46] = mqttdata[5];
	MQTTPublishData[47] = mqttdata[6];

	Uart4SendString("AT+CIPSEND=48\r\n");
	
	while(1){
		if((esp_race_count - 2 >= 0 && esp_race_buf[esp_race_count - 2] == '>')||(esp_race_count - 1 >= 0 && esp_race_buf[esp_race_count - 1] == '>')||(esp_race_count >= 0 && esp_race_buf[esp_race_count] == '>'))
			break;
	}
	delay_ms(100);
	WDT_CONTR = 0X37; //喂狗操作 

	for(num = 0;num < 48;num++)
	{
		Uart4SendData(MQTTPublishData[num]);
	}
	delay_ms(100);
	WDT_CONTR = 0X37; //喂狗操作 

  Uart4SendString("+++");
	WDT_CONTR = 0X37; //喂狗操作 
	delay_ms(200);
	return 1;
}
unsigned char MQTTPing()
{
	unsigned int num = 0;
	esp_race_count = 0;
	Uart4SendString("AT+CIPSEND=2\r\n");
	while(1){
		if(esp_race_count - 2 >= 0 && esp_race_buf[esp_race_count - 2] == '>')
			break;
	}
	delay_ms(1000);

	for(num = 0;num < 2;num++)
	{
		Uart4SendData(MQTTPingData[num]);
	}
	delay_ms(1000);

    Uart4SendString("+++");
	delay_ms(2000);
	return 1;
}
unsigned char MQTTDisconnect()
{
	unsigned int num = 0;
	esp_race_count = 0;
	Uart4SendString("AT+CIPSEND=2\r\n");
	while(1){
		if(esp_race_count - 2 >= 0 && esp_race_buf[esp_race_count - 2] == '>')
			break;
	}
	delay_ms(1000);

	for(num = 0;num < 2;num++)
	{
		Uart4SendData(MQTTDisconnectData[num]);
	}
	delay_ms(1000);

    Uart4SendString("+++");
	delay_ms(2000);
	return 1;
}