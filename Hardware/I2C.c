#include "stm32f10x.h"                  // Device header
//软件模拟I2C，使用GPIOB10和GPIOB11作为SCL和SDA线
void I2C2_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);
	

}

//控制SCL电平
void I2C2_SCL(uint8_t level)
{
	if(level)
	GPIO_SetBits(GPIOB,GPIO_Pin_10);
	else
	GPIO_ResetBits(GPIOB,GPIO_Pin_10);
}	

//控制SDA电平
void I2C2_SDA(uint8_t level)
{
	if(level)
	GPIO_SetBits(GPIOB,GPIO_Pin_11);
	else
	GPIO_ResetBits(GPIOB,GPIO_Pin_11);
}	

//I2C起始信号
void I2C2_Start(void)
{
	I2C2_SDA(1);
	I2C2_SCL(1);
	I2C2_SDA(0);
	I2C2_SCL(0);
}

//I2C停止信号
void I2C2_Stop(void)	
{
	I2C2_SDA(0);
	I2C2_SCL(1);
	I2C2_SDA(1);
}

//I2C发送一个字节
void I2C2_SendByte(uint8_t byte)	
{
	for(int i=0;i<8;i++)
	{
		I2C2_SDA((byte&0x80)>>7);
		byte<<=1;
		I2C2_SCL(1);
		I2C2_SCL(0);
	}
}

//I2C等待ACK信号
uint8_t I2C2_WaitAck(void)
{
	I2C2_SDA(1);
	I2C2_SCL(1);
	uint8_t ack=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
	I2C2_SCL(0);
	return ack;
}

//I2C发送ACK信号
void I2C2_SendAck(uint8_t ack)
{
	I2C2_SDA(ack);
	I2C2_SCL(1);
	I2C2_SCL(0);
}	

//I2C发送数据
void I2C2_SendData(uint8_t address,uint8_t* data,uint8_t length)
{
	I2C2_Start();
	I2C2_SendByte(address<<1); //发送设备地址，写操作
	if(I2C2_WaitAck()) //等待ACK
	{
		I2C2_Stop();
		return; //设备无响应，停止通信
	}
	for(int i=0;i<length;i++)
	{
		I2C2_SendByte(data[i]); //发送数据字节
		if(I2C2_WaitAck()) //等待ACK
		{
			I2C2_Stop();
			return; //设备无响应，停止通信
		}
	}
	I2C2_Stop(); //发送完成，停止通信
}


//I2C读取一个字节
uint8_t I2C2_ReadByte(void)
{
	uint8_t byte=0;
	I2C2_SDA(1); //释放SDA线，准备读取数据
	for(int i=0;i<8;i++)
	{
		I2C2_SCL(1);
		byte<<=1;
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11))
			byte|=0x01; //读取一个比特
		I2C2_SCL(0);
	}
	return byte;
}

//I2C读取数据
void I2C2_ReadData(uint8_t address,uint8_t* buffer,uint8_t length)
{
	I2C2_Start();
	I2C2_SendByte((address<<1)|0x01); //发送设备地址，读操作
	if(I2C2_WaitAck()) //等待ACK
	{
		I2C2_Stop();
		return; //设备无响应，停止通信
	}
	for(int i=0;i<length;i++)
	{
		buffer[i]=I2C2_ReadByte(); //读取数据字节
		if(i<length-1)
			I2C2_SendAck(0); //发送ACK，继续读取
		else
			I2C2_SendAck(1); //发送NACK，结束读取
	}
	I2C2_Stop(); //读取完成，停止通信
}
