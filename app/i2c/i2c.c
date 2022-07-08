#include "../gpio/gpio.h"
#include "timer.h"

/*
	I2C1->SCL => PB6, PB8
	I2C1->SDA => PB7, PB9
	I2C2->SCL => PB10
	I2C2->SDA => *PB3, PB11, PC12
	I2C3->SCL => PA8
	I2C3->SDA => *PB4, PC9
*/


void I2C_Config(I2C_t* I2Cx, GPIO_t* SCL_port, uint8_t SCL_pin, GPIO_t* SDA_port, uint8_t SDA_pin) {

	// check pins
	uint8_t lockedSCL = isPinLocked(SCL_port, SCL_pin);
	uint8_t lockedSDA = isPinLocked(SCL_port, SCL_pin);
	if (lockedSCL | lockedSDA) {
		// handle what to do if locked
		return;
	}

	// Enable I2C
	if (I2Cx == I2C1) RCC->APB1ENR |= (1 << 21);  // enable I2C1 CLOCK
	else if (I2Cx == I2C2) RCC->APB1ENR |= (1 << 22);  // enable I2C2 CLOCK
	else RCC->APB1ENR |= (1 << 23);  // enable I2C3 CLOCK


	// Enable the GPIO port
	if (SCL_port == GPIOA) RCC->AHB1ENR |= (1 << 0);  // Enable GPIOA CLOCK
	else if (SCL_port == GPIOB) RCC->AHB1ENR |= (1 << 1);  // Enable GPIOB CLOCK
	else RCC->AHB1ENR |= (1 << 2);  // Enable GPIOC CLOCK


	// if using same port for scl and sda then skip otherwise:
	if (SCL_port != SDA_port) {
		if (SDA_port == GPIOA) RCC->AHB1ENR |= (1 << 0);  // Enable GPIOA CLOCK
		else if (SDA_port == GPIOB) RCC->AHB1ENR |= (1 << 1);  // Enable GPIOB CLOCK
		else RCC->AHB1ENR |= (1 << 2);  // Enable GPIOC CLOCK
	}

	// GPIO config
	GPIO_CONFIG config = GPIO_ALTERNET | GPIO_OPEN_DRAIN | GPIO_HIGH_SPEED | GPIO_PULL_UP | GPIO_AF_AF4 | GPIO_LOCK_PIN;
	pinConfig(SCL_port, SCL_pin, config);
	pinConfig(SDA_port, SDA_pin, config);
	I2C_Reset(I2Cx);
}

/*
	I2C1->SCL => PB6, PB8
	I2C1->SDA => PB7, PB9
	I2C2->SCL => PB10
	I2C2->SDA => *PB3, PB11, PC12
	I2C3->SCL => PA8
	I2C3->SDA => *PB4, PC9
*/

void I2C_Config_default(I2C_t* I2Cx) {
	if (I2Cx == I2C1) {
		I2C_Config(I2C1, GPIOB, 6, GPIOB, 7);
	}
	else if (I2Cx == I2C2) {
		I2C_Config(I2C2, GPIOB, 10, GPIOB, 11);
	}
	else {
		I2C_Config(I2C3, GPIOA, 8, GPIOC, 9);
	}
}

void I2C_Reset(I2C_t* I2Cx) {
	I2Cx->CR1 |= (1 << 15);
	I2Cx->CR1 &= ~(1 << 15);
	// Program the peripheral input clock in I2C_CR2 Register in order to generate correct timings
	I2Cx->CR2 |= (45 << 0);  // PCLK1 FREQUENCY in MHz
	// Configure the clock control registers
	I2Cx->CCR = 225 << 0;
	// Configure the rise time register
	I2Cx->TRISE = 46;
	// Program the I2C_CR1 register to enable the peripheral
	I2Cx->CR1 |= (1 << 0);
}

void I2C_Start(I2C_t* I2Cx) {
	I2C1->CR1 |= (1 << 10); // Enable the ACK
	I2C1->CR1 |= (1 << 8);  // Generate START
	while (!(I2Cx->SR1 & (1 << 0)));  // Wait fror SB bit to set
}


void I2C_Write(I2C_t* I2Cx, uint8_t data) {
	while (!(I2Cx->SR1 & (1 << 7)));  // wait for TXE bit to set
	I2Cx->DR = data;
	while (!(I2Cx->SR1 & (1 << 2)));  // wait for BTF bit to set
}

#define I2C_TIMEOUT 10

void I2C_Address(I2C_t* I2Cx, uint8_t address) {
	uint8_t temp = I2Cx->SR1 | I2Cx->SR2;  // read SR1 and SR2 to clear the ADDR bit
	I2Cx->DR = address << 1;
	uint16_t i = 0;
	while (!(I2Cx->SR1 & (1 << 1)) && i < I2C_TIMEOUT) {// wait for ADDR bit to set
		i++;
		Delay(100);
	}

	uint8_t temp2 = I2Cx->SR1 | I2Cx->SR2;  // read SR1 and SR2 to clear the ADDR bit

	if (i == I2C_TIMEOUT) {
		// TODO: handle what to do when timeout occurs
	}
}

void I2C_Stop(I2C_t* I2Cx) {
	I2Cx->CR1 |= (1 << 9);  // Stop I2C
}

void I2C_WriteMulti(I2C_t* I2Cx, uint8_t* data, uint8_t size) {
	while (!(I2Cx->SR1 & (1 << 7)));  // wait for TXE bit to set
	while (size) {
		while (!(I2Cx->SR1 & (1 << 7)));  // wait for TXE bit to set
		I2Cx->DR = (uint32_t)*data++;  // send data
		size--;
	}

	while (!(I2Cx->SR1 & (1 << 2)));  // wait for BTF to set
}


void I2C_Read(I2C_t* I2Cx, uint8_t Address, uint8_t* buffer, uint8_t size) {
	int remaining = size;
	if (size == 1) {
		I2Cx->DR = Address << 1 | 1;  //  send the address
		while (!(I2Cx->SR1 & (1 << 1)));  // wait for ADDR bit to set
		I2Cx->CR1 &= ~(1 << 10);  // clear the ACK bit
		uint8_t temp = I2Cx->SR1 | I2Cx->SR2;  // read SR1 and SR2 to clear the ADDR bit.... EV6 condition
		I2Cx->CR1 |= (1 << 9);  // Stop I2C
		while (!(I2Cx->SR1 & (1 << 6)));  // wait for RxNE to set
		buffer[size - remaining] = I2Cx->DR;  // Read the data from the DATA REGISTER

	}

	I2Cx->DR = Address << 1 | 1;  //  send the address
	while (!(I2Cx->SR1 & (1 << 1)));  // wait for ADDR bit to set
	uint8_t temp = I2Cx->SR1 | I2Cx->SR2;  // read SR1 and SR2 to clear the ADDR bit
	while (remaining > 2) {
		while (!(I2Cx->SR1 & (1 << 6)));  // wait for RxNE to set
		buffer[size - remaining] = I2Cx->DR;  // copy the data into the buffer
		I2Cx->CR1 |= 1 << 10;  // Set the ACK bit to Acknowledge the data received
		remaining--;
	}

	// Read the second last byte
	while (!(I2Cx->SR1 & (1 << 6)));  // wait for RxNE to set
	buffer[size - remaining] = I2Cx->DR;

	I2Cx->CR1 &= ~(1 << 10);  // clear the ACK bit
	I2Cx->CR1 |= (1 << 9);  // Stop I2C
	remaining--;
	// Read the Last BYTE
	while (!(I2Cx->SR1 & (1 << 6)));  // wait for RxNE to set
	buffer[size - remaining] = I2Cx->DR;  // copy the data into the buffer
}