# MQTT客户端的C语言实现

## MQTT协议介绍

消息队列遥测传输（英语：Message Queuing Telemetry Transport，MQTT）是ISO 标准（[ISO/IEC PRF 20922](https://www.iso.org/standard/69466.html)）下基于发布 (Publish)/订阅 (Subscribe)范式的消息协议，它工作在 TCP/IP协议族上，是为硬件性能低下的远程设备以及网络状况糟糕的情况下而设计的发布/订阅型消息协议。

### 客户端(Client)

使用 MQTT 的程序或设备。客户端总是通过网络连接到服务端，为此，客户端需要实现以下功能

* 链接(Connect)<br>与服务器端建立TCP/IP链路，通知服务器设备上线

* 订阅(Subscribe)<br>订阅包含主题消息ID(TopicID)以及相应的主题名称(SUBSCRIBE_TOPIC)

* 发布(Publish)<br>客户端将数据(data)打包为某一格式的payload，并发布给服务器相应的Topic

* 保活(Ping)

* 取消订阅(Unsubscribe)

* 断开链接(Disconnect)

<!-- ### 客户端的底层

* 数据处理<br>将需要发送的信息及相应的标识符按照MQTT协议处理为相应的数据流(`char MQTTSendBuff[MQTT_BUFF_SIZE]`) -->

