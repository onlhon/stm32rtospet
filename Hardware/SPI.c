#include "stm32f10x.h"                  // Device header
//软件模拟SPI，使用GPIOA5、GPIOA6和GPIOA7作为SCK、MISO和MOSI线
void SPI_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,GPIO_InitStructure);
    
    GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
}
//SPI发送一个字节
void SPI_SendByte(uint8_t byte)
{
    for(int i=0;i<8;i++)
    {
        GPIO_WriteBit(GPIOA,GPIO_Pin_7,(byte&0x80)>>7); //设置MOSI线
        byte<<=1;
        GPIO_SetBits(GPIOA,GPIO_Pin_5); //拉高SCK线
        GPIO_ResetBits(GPIOA,GPIO_Pin_5); //拉低SCK线
    }
}
//SPI接收一个字节
uint8_t SPI_ReceiveByte(void)
{
    uint8_t byte=0;
    for(int i=0;i<8;i++)
    {
        GPIO_SetBits(GPIOA,GPIO_Pin_5); //拉高SCK线
        byte<<=1;
        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6))
            byte|=0x01; //读取一个比特
        GPIO_ResetBits(GPIOA,GPIO_Pin_5); //拉低SCK线
    }
    return byte;
}
//SPI发送数据
void SPI_SendData(uint8_t* data,uint8_t length)
{
    for(int i=0;i<length;i++)
    {
        SPI_SendByte(data[i]);
    }
}
//SPI接收数据
void SPI_ReceiveData(uint8_t* buffer,uint8_t length)
{
    for(int i=0;i<length;i++)
    {
        buffer[i]=SPI_ReceiveByte();
    }
}
//SPI发送和接收数据
void SPI_TransferData(uint8_t* txData,uint8_t* rxData,uint8_t length)
{
    for(int i=0;i<length;i++)
    {
        SPI_SendByte(txData[i]);
        rxData[i]=SPI_ReceiveByte();
    }
}
