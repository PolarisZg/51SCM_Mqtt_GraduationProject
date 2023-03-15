# 51SCM_Mqtt_GraduationProject
 51单片机采集环境数据并使用MQTT协议上传至阿里云

## 配置

|硬件|硬件型号|
|---|---|
|单片机|IAP15w4k58s4|
|OLED显示屏|128x64 IIC|
|WiFi|ESP8266|

## 头文件列表

[include](/include)

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

## 华北电力大学毕业设计(论 文) 开题报告

|题目|基于单片机的远程数据采集系统|
|---|---|
|学生姓名|祁百川|
|学号|120191080308|
|所在院系|控制与计算机工程学院
|专业班级|计算1901
|指导教师|闫江毓|

2023年3月8日

### 一 、选题背景和意义

&ensp;&ensp;&ensp;&ensp;环境数据采集在更好地了解环境状况、监测环境变化、支持研究、促进环境治理、服务公众等方面具有重要的意义。然而，当前环境数据采集存在以下痛点：数据来源不够全面，数据质量难以保证，数据管理和共享困难，数据处理和分析难度大等等，这些问题导致了环境数据采集的效率和质量不尽如人意，限制了其在环境保护和研究方面的应用和发展。<br>

&ensp;&ensp;&ensp;&ensp;为解决这些痛点，需要采取一系列的措施。例如降低成本，加大监测系统的建设力度，提高数据质量保障；选择统一精简的数据传输协议，方便各平台间的数据流转或迁移；开发简洁可视化的数据展示界面，降低数据分析难度。通过这些措施的实施，可以更好地发挥环境数据采集的作用，为环境保护和研究提供更好的支持和保障。<br>

&ensp;&ensp;&ensp;&ensp;嵌入式系统是以具体应用为导向的，以计算机技术为核心的根据具体应用对软件和硬件系统定做的微型计算机系统[[1]](#section1)。英特尔8051系列单片机具有高集成度、低功耗、可编程等特点，生产成本较低，价格相对于其他微控制器较为便宜，因此广泛应用于嵌入式系统，在低成本系统中大量使用。开发以单片机为控制核心的数据采集系统，不仅能降低成本，也便于设计开发。<br>

&ensp;&ensp;&ensp;&ensp;随着大数据、物联网等新兴技术的普及，无线通信技术发展越来越块，Wi-Fi覆盖面积越来越大，监测系统也正趋向于对数据的无线传输[[2]](#section2)。环境数据采集的特点是采集点较多，范围较远；若使用传统有线方式传输数据，会大大增加成本。利用ZigBee无线传感器网络进行环境数据采集与传输，优点是功耗低、应用广泛，但其构建的网络是封闭式网络，无法与Internet直接集成，所以传输距离受到限制[[3]](#section3)。利用无线模块nRF905以无线载波的形式将数据传输至上位机，相较于ZigBee虽然在传输距离上得到优化，但系统开放性大，易受干扰[[4]](#section4)。利用GPRS无线传输方案，虽然传输距离较远，但目前该网络没有固定IP[[5]](#section5)，持续租用公网使得成本上涨。基于以上所述，本设计中利用ESP8266 Wi-Fi通信模块，连接区域内Wi-Fi，与云端服务器实现数据远程通信，降低了成本，解决了发送端IP地址不固定的问题。<br>

&ensp;&ensp;&ensp;&ensp;MQTT是基于TCP/IP协议构建的异步通信消息协议，是一种轻量级的发布、订阅信息传输协议。可以在不可靠的网络环境中进行扩展，适用于设备硬件存储空间或网络带宽有限的场景。使用MQTT协议，消息发送者与接收者不受时间和空间的限制。物联网平台支持设备使用MQTT协议接入[[6]](#section6)。由于MQTT协议精简、易于实现，因此在节约带宽和控制功耗方面表现出很好的性能[[7]](#section7)[[8]](#section8)[[9]](#section9)。<br>

### 二、国内外研究现状

&ensp;&ensp;&ensp;&ensp;环境信息是影响工业生产与个人生活的关键因素，对环境信息的收集可帮助了解环境的变化，及时采取措施处理异常。在实践中，环境信息收集一般面向特定领域，如农业信息，工业信息等，根据实际需要有所不同，其关键技术主要包括信息感知技术，信息传输技术，信息处理技术等[[10]](#section10)。<br>

&ensp;&ensp;&ensp;&ensp;信息感知是通过感测技术获得信息，例如农业物联网中的感知环节就是部署大量传感器，将各类物理、化学信号转化为电信号，然后通过计算机进行处理[[11]](#section11)。目前，温度传感器使用较多的是数字温度传感器DS18B20，张仲明[[12]](#section12)等将其与单片机结合，构建出基于DS18B20的温度测量系统；朱顺兰[[13]](#section13)等设计的光照强度传感器，以光敏电阻为元件，基于光敏二极管的性能设计电路，提高了光敏传感器的精度。<br>

&ensp;&ensp;&ensp;&ensp;由于环境复杂多变，信息传输的稳定性至关重要。Wi-Fi也被称为WLAN，Wi-Fi通信具有带宽较宽、传输快、兼容强、抗干扰能力强等优势，在Wi-Fi覆盖区可实现远距离传输。作为一种无线网络技术，Wi-Fi允许计算机和其他设备在没有电线和电缆的情况下相互链接到局域网和英特网[[14]](#section14)。随着Wi-Fi技术和Wi-Fi网络的大面积覆盖，带来了建立物联网系统运输层的新方法，Wang[[15]](#section15)等将传感器网络应用与深海环境监测平台中，使得Wi-Fi无线技术在SOPC的高速可编程上有了新的突破；梁志勋[[16]](#section16)等基于MQTT协议，采用嵌入式集成模块ESP 8266作为主控，结合环境参数感知器等设备，使用Wi-Fi通信实现了对农业大棚内环境的监测。<br>

&ensp;&ensp;&ensp;&ensp;服务器是相应客户端请求的应用程序或计算机，可为客户端提供计算服务。传统上，开发人员必须构建自己的服务器，大量的底层开发工作使得开发过程成本高、时间长、工作量大。近年来，各种云平台的出现使得物联网系统的数据管理变得非常容易，可以根据云平台提供的服务设计应用程序，形成物联网的应用层。郭美荣[[17]](#section17)等对云平台在农业上的应用进行了初步探究，重点分析了云平台的理论和技术支撑，探讨了云平台的搭建方法和主要框架；姚丹[[18]](#section18)等在IBM Bluemix IoT平台服务的基础上，研究MQTT协议的结构和消息格式，实现了设备和移动端的数据传输。目前，市场主流云平台有AWS（亚马逊）、Azure（微软）、阿里云、腾讯云和OneNet（中国移动），平台功能丰富但成本和便捷程度差别较大。<br>

### 三、设计（论文）的主要研究内容及预期目标

&ensp;&ensp;&ensp;&ensp;本设计旨在选择合适的传单其，用于监测收集环境信息，通过无线网络实现数据实时上传，并利用云平台开发简洁明了的用户界面实现数据实时查看。为降低开发成本和难度，本设计采用IAP15W4K32S4作为微控制器，ESP8266模块作为收发器，配置阿里云平台搭建数据传输网络，设计Web界面以实现数据实时查看，以实现低成本的远程环境数据采集方案。本设计的主要内容如下：<br>

1. 设计基于单片机的远程数据采集系统的总体方案。该系统分为三层设计：由单片机为控制核心的信息感知层，由阿里云平台担任的数据流转层以及以web服务器为核心的展示层，其结构如图1所示：<br>
 
图 1 系统结构

2. 选取适当的传感器，以IAP15W4K32S4为核心设计硬件电路，通过外部GPIO接口与传感器相互连接，将传感器采集到的数据存于单片机内存中，并在OLED显示屏上展示。<br>

3. 设计适当的硬件电路，连接单片机与ESP8266模块；分析数据格式与传输协议，以MQTT协议规范将数据打包并传送至ESP8266相应GPIO端口，实现信息感知层与数据流转层（阿里云IoT平台）之间的信息传输。<br>

4. 在Keil[[19]](#section19)中配置IAP15W4K32S4的开发环境于硬件库，简化单片机C语言程序开发[[20]](#section20)，调用相应的库函数实现对单片机各端口的控制以及与其他硬件模块之间的信息交互。完成程序设计并烧录至单片机。<br>

5. 利用阿里云IoT平台提供的数据服务，注册设备，配置服务端订阅，利用AMQP服务，实现数据流转层与展示层之间的数据传输。在Web服务器上设计实现一个简洁的用户界面，实现数据的可视化显示，帮助用户远程监控环境数据。<br>

### 四、工作进度安排

1. 任务书及开题报告，2023年3月8日-2023年3月15日<br>

2. 调试单片机各串行通信端口，确保单片机与外部模块正确传输信息。2023年3月16日-2023年3月25日<br>

3. 开发基于C语言标准库的MQTT协议栈，需要包含的功能有：链接Connect，发布Publish，保活Ping以及断开链接Disconnect，该协议栈的输入为数据字符串，输出为符合MQTT协议的字符数据流（char*）及该字符数据流的长度。2023年3月26日-2023年4月10日<br>

4. 调试单片机数据上传准确度。2023年4月11日-2023年4月20日<br>

5. 开发Web端展示界面。2023年4月21日-2023年5月10日<br>

6. 撰写毕业设计论文。2023年5月11日-2023年6月1日<br>

### 五、参考文献

[1]	<a name="section1"></a>何宾. STC单片机原理及应用[M]. 第2版. 清华大学出版社, 2019.
[2]	<a name="section2"></a>KUO Y W,LI C L,JHANG J H,et al.Design of a wireless sensor network-based Io T platform for wide area and heterogeneous applications[J].IEEE sensors journal,2018,18(12):5187-5197.
[3]	<a name="section3"></a>艾红,邱靖鹏.基于Zig Bee无线传输的供暖温度监测系统研究[J].现代电子技术,2018,41(23):113-117.
[4]	<a name="section4"></a>YANG Wentao,QIAO Shushan,SONG Qiangguo,et al.The design and implementation of wireless temperature and humidity control system based on n RF905[C]//2015 IEEE10th Conference on Industrial Electronics and Applications.Auckland:IEEE,2015:753-756.
[5]	<a name="section5"></a>肖振锋,袁荣湘,邓翔天,等.基于MSP430F169的远程智能故障监测器[J].电力自动化设备,2013,33(1):160-164.
[6]	<a name="section6"></a>Aliyun.com. MQTT协议规范[EB/OL]. 2023-01-11. https://help.aliyun.com/document_detail/30540.html.
[7]	<a name="section7"></a>盖荣丽,钱玉磊,李鸿彬,贾军营.基于MQTT的企业消息推送系统[J].计算机系统应用,2015,24(11):69-75.
[8]	<a name="section8"></a>郑峰. 基于MQTT的云推送平台的研究及应用[D].杭州电子科技大学,2016.
[9]	<a name="section9"></a>Gomes Y F, Santos D F S, Almeida H O, et al.Integrating MQTT and ISO/IEEE 11073 for health information sharing in the Internet of Things[C]//IEEE International Conference on Consumer Electronics.IEEE, 2015:200-201.
[10] <a name="section10"></a>Díaz S E, Pérez J C, Mateos A C, et al. A novel methodology for the monitoring of the agricultural production process based on wireless sensor networks[J]. Computers and electronics in agriculture, 2011, 76(2): 252-265.
[11] <a name="section11"></a>孙晓梅. 智慧农业传感器的应用现状及展望[J]. 农业网络信息, 2015 (2): 39-41.
[12] <a name="section12"></a>张仲明,郭东伟,吕巍,张立明.基于DS18B20温度传感器的温度测量系统设计[J].实验技术与管理,2018,35(05):76-79+88.
[13] <a name="section13"></a>朱顺兰, 王雪萍. 光照强度传感器及其变送电路设计与实现[J]. 中国电子商务, 2009 (12): 93-93.
[14] <a name="section14"></a>Piromalis D D, Arvanitis K G, Sigrimis N. DASH7 mode 2: A promising perspective for wireless agriculture[J]. IFAC Proceedings Volumes, 2013, 46(18): 127-132.
[15] <a name="section15"></a>Wang Y, Qi C, Pan H. Design of remote monitoring system for aquaculture cages based on 3G networks and ARM-Android embedded system[J]. Procedia Engineering, 2012, 29: 79-83.
[16] <a name="section16"></a>梁志勋,施运应,覃有燎,王之兴.基于MQTT协议的智慧农业大棚测控系统研究[J].北方园艺,2020,(23):161-171.
[17] <a name="section17"></a>郭美荣, 李瑾, 秦向阳. 农业信息服务云平台架构初探[J]. 农业网络信息, 2012 (2): 13-16.
[18] <a name="section18"></a>姚丹,谢雪松,杨建军,Han Fei,Wang Shuai.基于MQTT协议的物联网通信系统的研究与实现[J].信息通信,2016(03):33-35.
[19] <a name="section19"></a>王海燕,杨艳华.Proteus和Keil软件在单片机实验教学中的应用[J].实验室研究与探索,2012,31(05):88-91.
[20] <a name="section20"></a>(美)StephenPrata著; 张海龙,袁国忠译.C Primer Plus(第六版)中文版[M] 北京：人民邮电出版社.2020[12]  谢希仁.计算机网络[M].8版.北京:电子工业出版社,2021.

六、指导教师意见








指导教师签名：

                                                      年    月    日  
