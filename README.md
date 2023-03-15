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