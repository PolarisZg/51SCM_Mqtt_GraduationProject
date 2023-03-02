#include "Nrf24l01.h"



tsNrf_Set gs_Nrf_Set = {
	40,
	32,
	{0x34,0x23,0x23,0x23,0x20},
	{0},
	0,
};




static void Nrf24L01_delayus(unsigned short ms)
{
	unsigned short i = ms;
	
	while(i--)
	{
		_nop_();
	}
}
//SPI写寄存器
//reg:指定寄存器地址
//value:写入的值
unsigned char NRF24L01_Write_Reg(unsigned char reg, unsigned char value)
{
	unsigned char status;
	
	Clr_NRF24L01_CSN;                 //使能SPI传输
	
	status = SPI_ReadWriteByte(reg);//发送寄存器号
	
	SPI_ReadWriteByte(value);      //写入寄存器的值
	
	Set_NRF24L01_CSN;                 //禁止SPI传输
	Nrf24L01_delayus(1);
	
	
	return(status);       			//返回状态值
}

//读取SPI寄存器值
//reg:要读的寄存器
unsigned char NRF24L01_Read_Reg(unsigned char reg)
{
	unsigned char reg_val;
	
	Clr_NRF24L01_CSN;          //使能SPI传输
	
	SPI_ReadWriteByte(reg);   //发送寄存器号
	
	reg_val = SPI_ReadWriteByte(0XFF);//读取寄存器内容
	
	Set_NRF24L01_CSN;          //禁止SPI传输
	Nrf24L01_delayus(1);
	
	return(reg_val);           //返回状态值
}

//在指定位置读出指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值 
unsigned char NRF24L01_Read_Buf(unsigned char reg,unsigned char *pBuf,unsigned char len)
{
	unsigned char status,u8_ctr;
	
	Clr_NRF24L01_CSN;           //使能SPI传输
	
	status = SPI_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值
	
	for(u8_ctr=0; u8_ctr<len; u8_ctr++)
		pBuf[u8_ctr]=SPI_ReadWriteByte(0XFF);//读出数据
	
	Set_NRF24L01_CSN;       //关闭SPI传输
	Nrf24L01_delayus(1);
	
	return status;        //返回读到的状态值
}

//在指定位置写指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值
unsigned char NRF24L01_Write_Buf(unsigned char reg, unsigned char *pBuf, unsigned char len)
{
	unsigned char status,u8_ctr;
	
	Clr_NRF24L01_CSN;          //使能SPI传输
	
	status = SPI_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值
	
	for(u8_ctr=0; u8_ctr<len; u8_ctr++)
		SPI_ReadWriteByte(*pBuf++); //写入数据
	
	Set_NRF24L01_CSN;       //关闭SPI传输
	Nrf24L01_delayus(1);
	
	return status;          //返回读到的状态值
}








//
//void EXTI9_5_IRQHandler(void)
//{
//	EXTI->PR = 1<<5;  //清除中断标志位
//	gs_Nrf_Set.Nrf_Flag |= EXTI_Data_Flag;
//}

//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
unsigned char NRF24L01_TxPacket(unsigned char *txbuf)
{
	unsigned char sta;
	
	Clr_NRF24L01_CE;
	NRF24L01_Write_Buf(NRF24L01_WR_TX_PLOAD, txbuf, gs_Nrf_Set.TRX_PLOAD_WIDTH);//写数据到TX BUF  32个字节
	Set_NRF24L01_CE;//启动发送
	
	while(NRF24L01_IRQ_N != 0);//等待发送完成
	
	sta = NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值
	
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG + STATUS, sta); //清除TX_DS或MAX_RT中断标志
	NRF24L01_Write_Reg(NRF24L01_FLUSH_TX, 0xFF);//清除TX FIFO寄存器
	
	if(sta&MAX_TX)//达到最大重发次数
	{
		return 2;
	}
	
	if(sta&TX_OK)
	{
		return 0;//发送完成
	}
	
	return 1;//其他原因发送失败
}

//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:1，接收完成；其他，错误代码
unsigned char NRF24L01_RxPacket(unsigned char *rxbuf)
{
	unsigned char sta = 0;
	
	sta = NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG + STATUS, sta); //清除TX_DS或MAX_RT中断标志
	
	if((sta&RX_OK) == RX_OK)//接收到数据
	{
		NRF24L01_Read_Buf(NRF24L01_RD_RX_PLOAD, rxbuf, gs_Nrf_Set.TRX_PLOAD_WIDTH);//读取数据
		NRF24L01_Write_Reg(NRF24L01_FLUSH_RX, 0xFF);//清除RX FIFO寄存器
		
		return 1;
	}
	
//	NRF24L01_Write_Reg(NRF24L01_FLUSH_RX, 0xFF);//清除RX FIFO寄存器
	
	return 0;//没收到任何数据
}

//该函数初始化NRF24L01到RX模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了		   
void RX_Mode(void)
{
	Clr_NRF24L01_CE;
	Set_NRF24L01_CSN;
	Nrf24L01_delayus(1);
	
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG + CONFIG_24L01, 0x0f);//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式
	Set_NRF24L01_CE; //CE为高,进入接收模式
	Nrf24L01_delayus(1);//130us
}

//该函数初始化NRF24L01到TX模式
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
//PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了		   
//CE为高大于10us,则启动发送.	 
void TX_Mode(void)
{
	Clr_NRF24L01_CE;
	Set_NRF24L01_CSN;
	Nrf24L01_delayus(1);
	
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG + CONFIG_24L01, 0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	Set_NRF24L01_CE;//CE为高,启动发送
	Nrf24L01_delayus(1);
}

// void NRF24L01_Config(void)
// {
// 	Nrf24L01_delayus(1);
// 	NRF24L01_Write_Buf(NRF24L01_WRITE_REG + TX_ADDR, (unsigned char *)gs_Nrf_Set.TRX_ADDRESS, TRX_ADR_WIDTH);//写TX节点地址 
// 	NRF24L01_Write_Buf(NRF24L01_WRITE_REG + RX_ADDR_P0, (unsigned char *)gs_Nrf_Set.TRX_ADDRESS, TRX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK
	
// 	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+RF_CH, gs_Nrf_Set.TRx_Hz);	     //设置RF通信频率
// 	Nrf24L01_delayus(1);
// }

//
// void Enable_Sleep(void)
// {
// 	Clr_NRF24L01_CE;
// 	Clr_NRF24L01_CSN;
//   NRF24L01_Write_Reg(NRF24L01_WRITE_REG+CONFIG_24L01, 0x00);		//Off Power在写寄存器之前一定要进入待机或掉电模式
// 	Nrf24L01_delayus(1);
// 	Set_NRF24L01_CE;//CE为高
// 	Set_NRF24L01_CSN;//禁止SPI
// }

void NRF24L01_Init(void)
{
	NRF24L01_Write_Buf(NRF24L01_WRITE_REG + TX_ADDR, (unsigned char *)gs_Nrf_Set.TRX_ADDRESS, TRX_ADR_WIDTH);//写TX节点地址
	NRF24L01_Write_Buf(NRF24L01_WRITE_REG + RX_ADDR_P0, (unsigned char *)gs_Nrf_Set.TRX_ADDRESS, TRX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK
	
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+SETUP_RETR,0x0a);//设置自动重发间隔时间:250us + 86us;最大自动重发次数:5次
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+RF_CH, gs_Nrf_Set.TRx_Hz);	     //设置RF通信频率
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+RF_SETUP,0x07);  //设置TX发射参数,0db增益,1Mbps,低噪声增益开启
	NRF24L01_Write_Reg(NRF24L01_WRITE_REG+RX_PW_P0, gs_Nrf_Set.TRX_PLOAD_WIDTH);//选择通道0的有效数据宽度
}

#ifdef NEF_NRF24L01_CHECK
//检测24L01是否存在
//返回值:   0---------成功    1---------失败	
unsigned char NRF24L01_Check(void)
{
	unsigned char buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	unsigned char i;
	
	Clr_NRF24L01_CSN;                 //使能SPI传输
	NRF24L01_Write_Buf(NRF24L01_WRITE_REG + TX_ADDR, buf, 5);//写入5个字节的地址.
	NRF24L01_Read_Buf(TX_ADDR, buf, 5); //读出写入的地址
	Set_NRF24L01_CSN;                 //禁止SPI传输
	
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;
	
	if(i!=5)
		return 1;		//未检测24L01	
	else 
		return 0;		 	//检测到24L01
}
#endif
