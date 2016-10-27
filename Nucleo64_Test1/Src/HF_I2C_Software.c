/*
 * HF_I2C_Software.c
 *
 *  Created on: 26 oct. 2016
 *      Author: HD-L
 */

#include "HF_I2C_Software.h"

static void I2C_delay(void) {
	volatile int i = 7;
	while (i) {
		i--;
	}
}

static bool I2C_Start(void) {
	SDA_H;
	SCL_H;
	I2C_delay();
	if (!SDA_read) {
		return false;
	}
	SDA_L;
	I2C_delay();
	if (SDA_read) {
		return false;
	}
	SDA_L;
	I2C_delay();
	return true;
}

static void I2C_Stop(void) {
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
}

static void I2C_Ack(void) {
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}

static void I2C_NoAck(void) {
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}

static bool I2C_WaitAck(void) {
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	if (SDA_read) {
		SCL_L;
		return false;
	}
	SCL_L;
	return true;
}

static void I2C_SendByte(uint8_t byte) {
	uint8_t i = 8;
	while (i--) {
		SCL_L;
		I2C_delay();
		if (byte & 0x80) {
			SDA_H;
		} else {
			SDA_L;
		}
		byte <<= 1;
		I2C_delay();
		SCL_H;
		I2C_delay();
	}
	SCL_L;
}

static uint8_t I2C_ReceiveByte(void) {
	uint8_t i = 8;
	uint8_t byte = 0;

	SDA_H;
	while (i--) {
		byte <<= 1;
		SCL_L;
		I2C_delay();
		SCL_H;
		I2C_delay();
		if (SDA_read) {
			byte |= 0x01;
		}
	}
	SCL_L;
	return byte;
}

void i2cInit(I2C_TypeDef * I2C) {
//	I2C1_SDA_EEPROM_GPIO_Port->MODER |=
//	GPIOB->
}

bool i2cWriteBuffer(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data) {
	int i;
	if (!I2C_Start()) {
		return false;
	}
	I2C_SendByte(addr);
	if (!I2C_WaitAck()) {
		I2C_Stop();
		return false;
	}
	I2C_SendByte(reg);
	I2C_WaitAck();
	for (i = 0; i < len; i++) {
		I2C_SendByte(data[i]);
		if (!I2C_WaitAck()) {
			I2C_Stop();
			return false;
		}
	}
	I2C_Stop();
	return true;
}

bool i2cWrite(uint8_t addr, uint8_t reg, uint8_t data) {
	if (!I2C_Start()) {
		return false;
	}
	I2C_SendByte(addr);
	if (!I2C_WaitAck()) {
		I2C_Stop();
		return false;
	}
	I2C_SendByte(reg);
	I2C_WaitAck();
	I2C_SendByte(data);
	I2C_WaitAck();
	I2C_Stop();
	return true;
}

/**
 * hello
 * @param addr
 * @param reg
 * @param data
 * @param size
 * @return
 */
bool i2cMemWriteSequence(uint8_t addr, uint8_t reg, uint8_t *data, uint8_t size) {
	if (size > 16) {
		return false;
	}
	if (!I2C_Start()) {
		return false;
	}
	I2C_SendByte(addr);
	if (!I2C_WaitAck()) {
		I2C_Stop();
		return false;
	}
	I2C_SendByte(reg);
	I2C_WaitAck();
	for (size; size > 0; size--) {
		I2C_SendByte(*data);
		I2C_WaitAck();
		data++;
	}
	I2C_Stop();
	return true;
}

bool i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf) {
	if (!I2C_Start()) {
		return false;
	}
	I2C_SendByte(addr);
	if (!I2C_WaitAck()) {
		I2C_Stop();
		return false;
	}
	I2C_SendByte(reg);
	I2C_WaitAck();
	I2C_Start();
	I2C_SendByte(addr | 0x01);	//I2C_Direction_Receiver
	I2C_WaitAck();
	while (len) {
		*buf = I2C_ReceiveByte();
		if (len == 1) {
			I2C_NoAck();
		} else {
			I2C_Ack();
		}
		buf++;
		len--;
	}
	I2C_Stop();
	return true;
}

uint16_t i2cGetErrorCounter(void) {
	// TODO maybe fix this, but since this is test code, doesn't matter.
	return 0;
}

