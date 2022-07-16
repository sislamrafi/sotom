#ifndef __I2C_H__
#define __I2C_H__

#include "../../../core/stm32f446re/registers/external/peripherals.h"

void I2C_Config(I2C_t* I2Cx, GPIO_t* SCL_port, uint8_t SCL_pin, GPIO_t* SDA_port, uint8_t SDA_pin);

void I2C_Config_default(I2C_t* I2Cx);

void I2C_Reset(I2C_t* I2Cx);

void I2C_Start(I2C_t* I2Cx);

void I2C_Write(I2C_t* I2Cx, uint8_t data);

void I2C_Address(I2C_t* I2Cx, uint8_t address);

void I2C_Stop(I2C_t* I2Cx);

void I2C_WriteMulti(I2C_t* I2Cx, uint8_t* data, uint8_t size);

void I2C_Read(I2C_t* I2Cx, uint8_t Address, uint8_t* buffer, uint8_t size);

#endif