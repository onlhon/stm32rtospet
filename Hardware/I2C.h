#ifndef _I2C2_
#define _I2C2_
void I2C2_Init(void);
void I2C2_Start(void);
void I2C2_Stop(void);
void I2C2_SendByte(uint8_t byte);
uint8_t I2C2_WaitAck(void);
void I2C2_SendAck(uint8_t ack);
void I2C2_SendData(uint8_t address,uint8_t* data,uint8_t length);
void I2C2_ReadData(uint8_t address,uint8_t* buffer,uint8_t length);
uint8_t I2C2_ReadByte(void);


#endif
