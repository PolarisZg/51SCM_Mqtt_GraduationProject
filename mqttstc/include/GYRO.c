#include "GYRO.h"
#include "Uart1.h"





static void delay_us(unsigned int ms)
{
	while(ms--);
}

//**************************************
//I2C起始信号
//**************************************
void I2C_Start()
{
    Gyro_SDA = 1;                    //拉高数据线
    Gyro_SCK = 1;                    //拉高时钟线
    delay_us(1);                 //延时
    Gyro_SDA = 0;                    //产生下降沿
    delay_us(1);                 //延时
    Gyro_SCK = 0;                    //拉低时钟线
}
//**************************************
//I2C停止信号
//**************************************
void I2C_Stop()
{
    Gyro_SCK = 0;                    //拉高时钟线
    Gyro_SDA = 0;                    //拉低数据线
    delay_us(1);                 //延时
    Gyro_SCK = 1;                    //拉高时钟线
    delay_us(1);                 //延时
    Gyro_SDA = 1;                    //产生上升沿
    delay_us(1);                 //延时
}
//**************************************
//I2C发送应答信号
//入口参数:ack (0:ACK 1:NAK)
//**************************************
void I2C_SendACK(unsigned char ack)
{
    Gyro_SDA = ack;                  //写应答信号
    Gyro_SCK = 1;                    //拉高时钟线
    delay_us(1);                 //延时
    Gyro_SCK = 0;                    //拉低时钟线
    delay_us(1);                 //延时
}
//**************************************
//I2C接收应答信号
//**************************************
unsigned char I2C_RecvACK()
{
	unsigned char res = 0;
	
    Gyro_SCK = 1;                    //拉高时钟线
    delay_us(1);                 //延时
    res = Gyro_SDA;                   //读应答信号
    Gyro_SCK = 0;                    //拉低时钟线
    delay_us(1);                 //延时
	
    return res;
}
//**************************************
//向I2C总线发送一个字节数据
//**************************************
void I2C_SendByte(unsigned char dat)
{
  unsigned char i;
	unsigned char res = 0;
	
	res = dat;
	for (i=0; i<8; i++)         //8位计数器
	{
		if(res & 0x80)
		{
			Gyro_SDA = 1;               //送数据口
		}else
		{
			Gyro_SDA = 0;               //送数据口
		}
		
		Gyro_SCK = 1;                //拉高时钟线
		delay_us(1);             //延时
		Gyro_SCK = 0;                //拉低时钟线
		delay_us(1);             //延时
		res <<= 1;              //移出数据的最高位
	}
	I2C_RecvACK();
}
//**************************************
//从I2C总线接收一个字节数据
//**************************************
unsigned char I2C_RecvByte()
{
    unsigned char i;
    unsigned char dat = 0;
	
    Gyro_SDA = 1;                    //使能内部上拉,准备读取数据,
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;
        Gyro_SCK = 1;                //拉高时钟线
        delay_us(1);             //延时
        dat |= Gyro_SDA;             //读数据               
        Gyro_SCK = 0;                //拉低时钟线
        delay_us(1);             //延时
    }
		
    return dat;
}









//**************************************
//向I2C设备写入一个字节数据
//**************************************
void Single_WriteI2C(unsigned char REG_Address,unsigned char REG_data)
{
    I2C_Start();                  //起始信号
    I2C_SendByte(SlaveAddress);   //发送设备地址+写信号
    I2C_SendByte(REG_Address);    //内部寄存器地址，
    I2C_SendByte(REG_data);       //内部寄存器数据，
    I2C_Stop();                   //发送停止信号
}
//**************************************
//从I2C设备读取一个字节数据
//**************************************
unsigned char Single_ReadI2C(unsigned char REG_Address)
{
	unsigned char REG_data;
	
	I2C_Start();                   //起始信号
	I2C_SendByte(SlaveAddress);    //发送设备地址+写信号
	I2C_SendByte(REG_Address);     //发送存储单元地址，从0开始	
	I2C_Start();                   //起始信号
	I2C_SendByte(SlaveAddress+1);  //发送设备地址+读信号
	REG_data=I2C_RecvByte();       //读出寄存器数据
	I2C_SendACK(1);                //接收应答信号
	I2C_Stop();                    //停止信号
	return REG_data;
}







//**************************************
//初始化MPU6050
//**************************************
void InitMPU6050(void)
{
	Single_WriteI2C(PWR_MGMT_1, 0x00);	//解除休眠状态
	Single_WriteI2C(SMPLRT_DIV, 0x07);
	Single_WriteI2C(CONFIG, 0x06);
	Single_WriteI2C(GYRO_CONFIG, 0x18);
	Single_WriteI2C(ACCEL_CONFIG, 0x01);
}
//**************************************
//合成数据
//**************************************
int GetData(unsigned char REG_Address)
{
	unsigned char H,L;
	H=Single_ReadI2C(REG_Address);
	L=Single_ReadI2C(REG_Address+1);
	return (H<<8)+L;   //合成数据
}
