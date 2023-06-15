# 51SCM_Mqtt_GraduationProject
 51单片机采集环境数据并使用MQTT协议上传至阿里云，阿里云使用AMQP消息队列下发至业务服务器，在前端页面展示数据

## 配置

|硬件|硬件型号|
|---|---|
|单片机|IAP15w4k58s4|
|OLED显示屏|128x64 IIC|
|WiFi|ESP8266|

## 头文件列表

[include](/include)

## 文件目录结构

```
.
|-- KeyAndOled		按键及OLED的测试
|   |-- KOLED.uvgui.zghbq
|   |-- KOLED.uvopt
|   |-- KOLED.uvproj
|   |-- Listings
|   |-- Objects
|   |-- STARTUP.A51
|   |-- include
|   `-- main.c
|-- MQTTClient		C语言实现的MQTT客户端
|   |-- ClassDiagram.cd
|   |-- MQTTClient.sln
|   |-- MQTTClient.vcxproj
|   |-- MQTTClient.vcxproj.filters
|   |-- MQTTClient.vcxproj.user
|   |-- README.md
|   |-- esp.h
|   |-- main.c
|   |-- mqtt.c
|   |-- mqtt.h
|   `-- x64
|-- README.md
|-- Uart1test		对UART1串口通信的测试
|   |-- Listings
|   |-- Objects
|   |-- README.md
|   |-- STARTUP.A51
|   |-- esptest.uvgui.zghbq
|   |-- esptest.uvopt
|   |-- esptest.uvproj
|   |-- include
|   `-- main.c
|-- amqp-java		来自阿里云iot平台的AMQP客户端的Java版本的demo
|   |-- lib
|   |-- pom.xml
|   |-- src
|   `-- target
|-- amqpSpring		Java Spring业务服务器
|   |-- HELP.md
|   |-- amqpSpring.iml
|   |-- mvnw
|   |-- mvnw.cmd
|   |-- pom.xml
|   |-- src
|   |   |-- main
|   |   |   |-- java
|   |   |   |   `-- com
|   |   |   |       `-- example
|   |   |   |           `-- amqpspring
|   |   |   |               |-- AmqpClient.java
|   |   |   |               |-- AmqpFullSQL.java
|   |   |   |               |-- AmqpMessage.java
|   |   |   |               |-- AmqpMessageDTO.java
|   |   |   |               |-- AmqpSpringApplication.java
|   |   |   |               |-- MessageOfTopicController.java
|   |   |   |               |-- PageAmqpMessageRepository.java
|   |   |   |               |-- SaveAmqpMessageRepository.java
|   |   |   |               |-- TopicAmqpMessageRepository.java
|   |   |   |               `-- TopicsController.java
|   |   |   `-- resources
|   |   |       |-- application.properties
|   |   |       |-- static
|   |   |       |   |-- anewdemo.html
|   |   |       |   |-- css
|   |   |       |   |   `-- aDemo.css
|   |   |       |   |-- index.html
|   |   |       |   |-- js
|   |   |       |   |   `-- topics.js
|   |   |       |   `-- topics.html
|   |   |       `-- templates
|   |   `-- test
|   |       `-- java
|   |           `-- com
|   |               `-- example
|   |                   `-- amqpspring
|   |                       `-- AmqpSpringApplicationTests.java
|   `-- target
|-- doc			使用pandoc生成论文的过程文档
|-- esp			ESP8266测试模块
|   |-- Listings
|   |-- Objects
|   |-- STARTUP.A51
|   |-- esp.uvgui.zghbq
|   |-- esp.uvopt
|   |-- esp.uvproj
|   |-- include
|   `-- main.c
|-- esptest		同样也是ESP8266测试模块
|   |-- Listings
|   |-- Objects
|   |-- STARTUP.A51
|   |-- esptest.uvgui.zghbq
|   |-- esptest.uvopt
|   |-- esptest.uvproj
|   |-- include
|   `-- main.c
|-- include		头文件
|-- mqttstc		最终烧录至单片机的程序
|   |-- Listings
|   |-- Objects
|   |-- STARTUP.A51
|   |-- include
|   |-- main.c
|   |-- mqttstc.uvgui.zghbq
|   |-- mqttstc.uvopt
|   `-- mqttstc.uvproj
`-- uml			过程中使用的UML图，使用plantuml绘制

```

## 使用方法

* 注册阿里云iot平台账户，按照其说明文档添加产品及设备，在设备界面获取MQTT连接参数

![新建的阿里云iot产品界面](img/iotproduct.jpg)

![新建的阿里云iot设备界面](img\iotdevice.jpg)

* 使用visual studio打开 `MQTTClient` 文件夹，使用获得的MQTT连接参数修改`main.c` 的阿里云相关配置,仅需要修改 `三元组` 、 `加密方式`  、 `密码`

   * 按照需要，修改服务器地址
  
   * 之后的对于MQTTClient中代码的修改可能会造成无法预料的bug，请自行查询搜索引擎解答，或发送邮件至`zghbqbc@gmail.com`。MQTTClient文件中包含了众多对C语言指针的使用，请至少对C语言的数组、函数指针等知识有一定的了解

* 修改 `main.c` 文件中 `MQTTSendData` 函数中 `printf` 内容，让其输出十六进制数字和空格与逗号，修改后的代码如下：

```
int MQTTSendData(const char* data, unsigned int length)
{
	///* 发送数据到服务端 */
	send(sock, data, length, 0);
	//{
	//	return 1;
	//}
	//return 0;

	unsigned char temp = 0;
	printf("\n\nlength is %d\n\n", length);
	while (length--)
	{
		temp = *data++;
		printf("0x%02X ,", temp);
	}

	return 1;
}
```

* 运行 `main.c` 文件，结果如下图所示，这里需要得到的是阿里云iot的IP地址、MQTT的CONNECT报文及PUBLISH报文的十六进制内容

   * 其实不需要 `MQTTClient` 项目，仅需要使用 `MQTT.fx` 工具和网络抓包工具也能得到同样的数据
   
   * 这一步运行完毕后可在阿里云iot平台日志中看到有设备上下线记录，证明上述步骤圆满完成，若无上下线记录，请排查是否正确的修改了阿里云连接参数
   
   * 运行时可能会被杀毒软件报错，请关闭杀毒软件，或将相应的文件转移至信任区

![MQTTClient运行后的结果](img\MQTTClient.jpg)

* 使用 `Keil` 打开 `mqttstc` 项目，修改 `main.c` 文件中 `初始化esp8266` 的相关参数，第一个为WiFi的名称，第二个为WiFi密码，第三个为上方获取的阿里云iot的IP地址，端口号无需修改

* 修改 `esp.c` 中数组 `MQTTConnectData[]`，`MQTTPublishData` 的内容

* 编译生成可执行文件，烧录至单片机，启动单片机，可在阿里云iot平台的日志中查询到相关设备上线记录，证明设备上线成功

   * 若未上线，请按照如下步骤检查：

      1. 打开宏晶公司烧录软件的串口通信界面，打开接收数据，重启单片机，若能正常接收数据则证明UART串口无故障，按照串口的输出日志可做初步检查
      
	  2. 打开路由器后台界面，检查是否有ESP设备上线
	  
	  3. 使用模拟TCP通信软件（如Serial Port Utility）检查网络通信是否有故障，这一步需要去修改单片机程序中的目的IP地址
	  
	  4. 发邮件给我

* 使用虚拟机或服务器或在物理机上创建一个MySQL数据库，以供之后的步骤使用
   
   * 这一步需要做到可以通过navicat等数据库软件连接至MySQL数据库

* 使用IDEA打开 `amqpSpring` 项目，修改配置文件，打开项目时注意保持网络畅通，以便项目自动下载安装相应的库
    
	* 修改 `application.properties` 的内容，以便Java项目连接数据库
	
	* 根据注释，修改 `AmqpClient` 类中相关的参数，此处的修改可以参考[阿里云物联网平台的帮助文档](https://help.aliyun.com/document_detail/143601.htm)，这个类实际就是将帮助文档中的示例的Java demo修改而来
	
* 启动 `AmqpSpringApplication` ，正常情况下可在控制台查看到相应的日志输出

![日志输出](img\amqpspringlog.jpg)

* 点击进入 `anewdemo.html` 页面，启动该页面，可访问浏览器前端



## UART1端口输出测试

### 使用

1. 使用 keil51 打开 *.uvproj 文件

2. 导入 include 文件夹 及相应的 .c 文件 <br> ***仅导入 include 文件夹而不导入 .c 文件是无法编译的***

3. 若遇到 `main.c(24): error C267: 'Uart1SendData': requires ANSI-style prototype` 之类的定义错误。检查相应的 .h 文件中是否声明了相应的函数

4. 烧录

## OLED屏和按键K1 ~ K6

### 使用

1. 使用 keil51 打开 *.uvproj 文件

2. 导入 include 文件夹 及相应的 .c 文件 <br> ***仅导入 include 文件夹而不导入 .c 文件是无法编译的***

3. 烧录

4. 若OLED屏不显示，检查OLED.h文件(或 main.c 文件)中是否对OLED屏使用到的两个时钟GPIO端口进行初始化

```
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
	
    //推挽输出
    GPIO_InitStructure.Mode = GPIO_OUT_PP;		//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_InitStructure.Pin  = GPIO_Pin_5;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
    GPIO_Inilize(GPIO_P4, &GPIO_InitStructure);	//初始化
	
    //开漏输出
    GPIO_InitStructure.Mode = GPIO_OUT_OD;		//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	
    GPIO_InitStructure.Pin  = GPIO_Pin_6;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
    GPIO_Inilize(GPIO_P4, &GPIO_InitStructure);	//初始化
```

## 利用esp8266连接

## tips

1. 需要使用uart1数据串口与电脑通信，需要使用uart4数据串口与esp8266通信

2. 因此，需要初始化两个数据串口，注意初始化两个串口时需要两个计时器，而官方给的include文件中对uart1端口初始化与uart4端口初始化的函数中，使用了同一个计数器<br>官方include文件如下<br>

```
void Uart1_Init(void)
{
	ACC = P_SW1;
	ACC &= ~(S1_S0 | S1_S1); //S1_S0=0 S1_S1=0(P3.0/RxD, P3.1/TxD)
	P_SW1 = ACC;
	SCON = (SCON & 0x3f) | 0x40; //8位可变波特率

	T2L = (65536 - (MAIN_Fosc / 4 / BAUD)); //设置波特率重装值
	T2H = (65536 - (MAIN_Fosc / 4 / BAUD)) >> 8;

	AUXR = 0x14;  //T2为1T模式, 并启动定时器2
	AUXR |= 0x01; //选择定时器2为串口1的波特率发生器
	PS = 1;		  //高优先级中断

	ES = 1;	 //使能串口1中断
			 //	TI = 1;         //S4TI位
	REN = 1; //允许接收
	EA = 1;	 //允许全局中断
}
```

```
void Uart4_Init(void)
{
	P_SW2 |= S4_S0;             //S4_S0=1 (P5.2/RxD4_2, P5.3/TxD4_2)

	S4CON = 0x10;               //8位可变波特率

	T2L = (65536 - (MAIN_Fosc/4/BAUD));   //设置波特率重装值
	T2H = (65536 - (MAIN_Fosc/4/BAUD))>>8;

	AUXR = 0x14;                //T2为1T模式, 并启动定时器2
	PS = 0;	//高优先级中断
	
	IE2 = 0x10;                 //使能串口4中断
}
```

可以看到，两个串口都使用了T2计时器，因此若使用官方include文件进行初始化，会导致其中一个串口不可用<br>这样，我们可以自己写一个串口初始化函数，调用其他的计时器对串口初始化<br>修改后的uart4串口初始化函数如下

```
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
```

3. 函数命名啊，简直太混乱了，驼峰命名和下划线混着来，uart4和uart1俩传输数据都混在一起，留到以后再改吧

4. 变量的声明与变量的定义不同，变量声明可以再 .h 文件中，变量的定义可以在 .c 文件中，同名变量仅能定义一次

5. 需要长时间的delay_ms，esp8266的返回信息很耗时间。当然，长时间的delay容易影响采集数据的精准度，因此有时需要根据返回值去判断是否进行为非delay<br>~~不过delay_ms它无脑啊~~<br>一个长达10s的延迟，可以让连接的wifi更加稳定

```
void delay_10s()
{
	delay_ms(1000);	//延时
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
}

/* 初始化esp8266 */
	Uart4Init();
	printf("u4 init end\r\n");
	Uart4SendString("AT+CWJAP=\"WF406\",\"406406ncepu\"\r\n");
	printf("AT+CWJAP=\"WF406\",\"406406ncepu\"\r\n");
	delay_10s();
	printf("race:\r\n");//串口1发送信息
	Uart1SendData(uart4_race_buf,uart4_race_count);
	delay_ms(1000);//延时1秒
```

发送数据时，不好控制延时，所以就根据返回值去判断是否允许发送数据

```
uart4_race_count = 0;//串口4接收计数清零
Uart4SendString("send:\r\nAT+CIPSEND=7\r\n");//
while(1){
	if(uart4_race_count - 2 >= 0 && uart4_race_buf[uart4_race_count - 2] == '>')
		break;
}

printf("\r\n------race:\r\n");//
Uart1SendData(uart4_race_buf,uart4_race_count);//将串口4收到的数据通过串口1发送给电脑

uart4_race_count = 0;//串口4接收计数清零
Uart4SendString(wifi_send_buf);//串口4发送需要发送的数据   数据在wifi_send_buf 数组中   数组的定义在最上面
delay_ms(1000);//
```

6. `AT+CIPSEND`指令需要使用`+++`指令中止，在`+++`之后或之前需要至少20ms的延时(所以咱就用1000ms就好了)

```
Uart4SendString("+++");
delay_ms(2000);
```

7. `AT+CIPSEND`需要指定所传输数据的长度，这个长度值要整合在字符串中，估计是一个一位至三位的数字，之后再写这个部分吧

8. 还是要吐槽一下那个混乱的函数命名

## vmware虚拟机配置服务器

**首先非常感谢chat GPT 做出的巨大贡献**<br>

**让我们为人工智能而欢呼**<br>

### 安装系统，硬件配置

* 安装系统非常简单，使用vmware推荐的配置即可，这里安装的是centos7系统

* 重要的是网卡，如果物理机能联网，则使用桥接模式。
	* 如果不能联网，那么就去联网再说

* 进入系统，找到设置，把网卡打开，然后点设置，设置为`Connect automatically`

* 在虚拟机中查看ip地址 

```
ip a
```

会显示如下信息

```
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host 
       valid_lft forever preferred_lft forever
2: ens33: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast state UP group default qlen 1000
    link/ether 00:0c:29:7a:77:f0 brd ff:ff:ff:ff:ff:ff
    inet 192.168.1.101/24 brd 192.168.1.255 scope global noprefixroute dynamic ens33
       valid_lft 6582sec preferred_lft 6582sec
    inet6 fe80::6d92:3890:c003:5dd2/64 scope link noprefixroute 
       valid_lft forever preferred_lft forever
3: virbr0: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue state DOWN group default qlen 1000
    link/ether 52:54:00:b8:3f:32 brd ff:ff:ff:ff:ff:ff
    inet 192.168.122.1/24 brd 192.168.122.255 scope global virbr0
       valid_lft forever preferred_lft forever
4: virbr0-nic: <BROADCAST,MULTICAST> mtu 1500 qdisc pfifo_fast master virbr0 state DOWN group default qlen 1000
    link/ether 52:54:00:b8:3f:32 brd ff:ff:ff:ff:ff:ff

```
重要的是第二块网卡`ens33`，此处显示该网卡的ip地址为`192.168.1.101`和我本机的ip地址`192.168.1.103`很相似(嘛，毕竟是桥接模式)

### ssh

* 查看是否安装了ssh
	* ssh的连接是mysql连接的第一步
	* 查看ssh的安装

	```
	rpm -qa|grep -E "openssh"
	```

	ssh服务完全安装的话，应该返回如下信息。一般Linux系统都会内置ssh。

	```
	openssh-clients-7.4p1-22.el7_9.x86_64
	openssh-7.4p1-22.el7_9.x86_64
	openssh-server-7.4p1-22.el7_9.x86_64
	```

* 启动ssh服务

	```
	sudo systemctl start sshd
	```

	* 如果想要开机自启动，可使用以下命令

	```
	sudo systemctl enable sshd
	```

* 检查ssh服务是否正常启动

	```
	sudo systemctl status sshd
	```

	* 正常启动的话，会返回如下信息

	```
	● sshd.service - OpenSSH server daemon
   		Loaded: loaded (/usr/lib/systemd/system/sshd.service; enabled; vendor preset: enabled)
   		Active: active (running) since Wed 2023-04-05 19:14:12 CST; 2h 36min ago
     		Docs: man:sshd(8)
           		man:sshd_config(5)
 	Main PID: 35206 (sshd)
    	Tasks: 1
   		CGroup: /system.slice/sshd.service
           		└─35206 /usr/sbin/sshd -D

	Apr 05 19:14:12 localhost.localdomain systemd[1]: Stopped OpenSSH server daemon.
	Apr 05 19:14:12 localhost.localdomain systemd[1]: Starting OpenSSH server daemon...
	Apr 05 19:14:12 localhost.localdomain sshd[35206]: Server listening on 0.0.0.0 port 22.
	Apr 05 19:14:12 localhost.localdomain sshd[35206]: Server listening on :: port 22.
	Apr 05 19:14:12 localhost.localdomain systemd[1]: Started OpenSSH server daemon.
	```

	* 在这段返回信息中，可以看到ssh服务使用了`22`端口，并且允许所有ip地址访问: `0.0.0.0`

* 在物理机上打开ssh客户端，新建连接。地址为`ip a`命令返回的地址，端口为`22`，用户名和密码为登录centos系统所使用的用户名及密码(不建议使用root)

* ssh连接成功就完成了一半

### MySQL

* 安装
	* 需要注意的是 CentOS 7 版本中 MySQL数据库已从默认的程序列表中移除，因此要访问相关网站下载rpm安装包进行安装

	1. **逐句输入以下命令**，由于网址原因，不保证第一条命令会被正确的执行，因此不保证第二条命令被正确执行，可通过访问MySQL官网 [https://dev.mysql.com/downloads/repo/yum/](https://dev.mysql.com/downloads/repo/yum/)自己修改相应的地址及包名
	
	```
	wget http://repo.mysql.com/mysql-community-release-el7-5.noarch.rpm
	rpm -ivh mysql-community-release-el7-5.noarch.rpm
	yum update
	yum install mysql-server
	```

	2. **权限设置：**

	```
	chown -R mysql:mysql /var/lib/mysql/
	```

	3. **初始化 MySQL：**

	```
	mysqld --initialize
	```

	4. **启动 MySQL：**

	```
	systemctl start mysqld
	```

	* 查看 MySQL 运行状态：

	```
	systemctl status mysqld
	```

	* 验证 MySQL 安装

	```
	mysqladmin --version
	```

	linux上该命令将输出以下结果，该结果基于系统信息：

	```
	mysqladmin  Ver 8.42 Distrib 5.6.51, for Linux on x86_64
	```

	5. **接下来，需要运行 MySQL 安全脚本，以确保 MySQL 安全：**

	```
	sudo mysql_secure_installation
	```

	该脚本将提示您设置 MySQL 根密码、删除匿名用户、禁止远程 root 登录等。按照提示完成操作。

	6. **重启MySQL服务以使更改生效：**

	```
	sudo systemctl restart mysqld
	```

* 使用物理机客户端连接虚拟机中的mysql数据库

	1. **查看 MySQL 服务器正在监听的端口：**

	```
	sudo netstat -tlnp | grep mysqld
	```

	* 这将输出正在运行的 MySQL 服务器进程的信息，包括 PID（进程ID），端口号以及绑定的 IP 地址。

	```
	tcp        0      0 0.0.0.0:3306            0.0.0.0:*               LISTEN      69966/mysqld  
	```

	2. **在防火墙上允许连接到MySQL服务器的端口**。在CentOS 7上，默认情况下防火墙使用的是firewalld，您可以使用以下命令添加MySQL服务的firewalld规则：

	```
	sudo firewall-cmd --add-service=mysql --permanent
	sudo firewall-cmd --reload
	```

	这将打开3306端口，允许从远程访问MySQL服务器。请注意，远程访问MySQL服务器是不安全的，因此应该尽可能限制MySQL服务器只对受信任的主机开放。

	3. **编辑配置文件`/etc/my.cnf`**，在[mysqld]下添加以下行，以允许来自远程的连接

	```
	[mysqld]
	bind-address = 0.0.0.0
	skip-name-resolve
	```

	4. 保存并退出配置文件，然后重启 MySQL 服务：

	```
	sudo systemctl restart mysqld
	```

	5. **创建MySQL用户，通过这个用户进行远程访问**

		51. 使用 root 用户或具有 root 权限的用户登录 MySQL：

		```
		mysql -u root -p
		```

		52. 在 MySQL shell 中，创建一个新用户并设置密码：

		```
		CREATE USER 'poi'@'%' IDENTIFIED BY 'poi';
		```

		其中，前一个 `poi` 是新用户的用户名，后一个 `poi` 是新用户的密码。`%` 为希望该用户能够从远程连接到 MySQL.

		53. 授予新用户访问数据库的权限：

		```
		GRANT ALL PRIVILEGES ON *.* TO 'poi'@'%';
		```
		
		`*.*` 为希望该用户能够访问任何数据库.

		54. 刷新权限：

		```
		FLUSH PRIVILEGES;
		```

		现在，应该可以使用新用户访问 MySQL 服务器了

### 遇到的问题

1. 哎呀为什么我用nat模式没法连接虚拟机<br>

	> 使用桥接模式设置虚拟机网卡

2. 连接虚拟机中MySQL数据库时提示`IP address '192.168.1.103' could not be resolved: Name or service not known`

	> 出现这个提示应该是使用root用户访问MySQL，在MySQL中单独创建一个用户进行远程访问即可

3. 啊啊啊我还有很多问题怎么办？

	> 去查看软件的日志，可以解决很多问题，MySQL日志所在地址为`/var/log/mysqld.log`

4. 虚拟机时间对不上的解决方案

	* 要让 CentOS 的系统时间与标准时间同步，您可以使用 NTP 服务。NTP（Network Time Protocol）是一种用于同步计算机系统时间的协议，它通过互联网上的时间服务器获取标准时间，并将其同步到本地系统上。

	以下是在 CentOS 7 上安装和配置 NTP 服务的步骤：

	1. 安装 NTP 服务：

	```
	sudo yum install ntp
	```

	2. 启动 NTP 服务并设置为开机自启动：

	```
	sudo systemctl start ntpd
	sudo systemctl enable ntpd
	```

	3. 检查 NTP 服务是否正在运行：

	```
	sudo systemctl status ntpd
	```

	（可选）如果您希望将系统时区更改为您所在的时区，可以使用 timedatectl 命令来更改时区设置。例如，如果您希望将时区设置为北京时间：

	```
	sudo timedatectl set-timezone Asia/Shanghai
	```

	在此示例中，我们将时区设置为 Asia/Shanghai。

	现在，您的 CentOS 系统应该会从 NTP 服务器同步标准时间。如果您想手动强制同步时间，可以使用 ntpdate 命令。例如，要强制同步时间到 pool.ntp.org 时间服务器：

	```
	sudo ntpdate pool.ntp.org
	```

	请注意，如果您的虚拟机和宿主机之间的时间差距太大，可能会导致网络连接问题。因此，建议您使用 NTP 服务来确保系统时间的准确性。

