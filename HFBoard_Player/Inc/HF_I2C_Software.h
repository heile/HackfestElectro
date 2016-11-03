/*
 * HF_I2C_Software.h
 *
 *  Created on: 26 oct. 2016
 *      Author: HD-L
 */

#ifndef HF_I2C_SOFTWARE_H_
#define HF_I2C_SOFTWARE_H_

#include <stdbool.h>
#include <stdint.h>
#include "stm32f0xx_hal.h"
#include "main.h"




#define SCL_H         GPIOB->BSRR = I2C1_SCL_EEPROM_Pin
#define SCL_L         GPIOB->BRR  = I2C1_SCL_EEPROM_Pin

#define SDA_H         GPIOB->BSRR = I2C1_SDA_EEPROM_Pin
#define SDA_L         GPIOB->BRR  = I2C1_SDA_EEPROM_Pin

#define SDA_Mode_Read I2C1_SDA_EEPROM_GPIO_Port->MODER &=0xFFFFCFFF		//mode input
#define SDA_Mode_Write I2C1_SDA_EEPROM_GPIO_Port->MODER |=0xFFFFDFFF	//mode output

#define SCL_Mode_Read I2C1_SCL_EEPROM_GPIO_Port->MODER &=0xFFFF3FFF		//mode input
#define SCL_Mode_Write I2C1_SCL_EEPROM_GPIO_Port->MODER |=0xFFFF7FFF	//mode output

#define SCL_read      (GPIOB->IDR & I2C1_SCL_EEPROM_Pin)
#define SDA_read      (GPIOB->IDR & I2C1_SDA_EEPROM_Pin)




//void i2cInit(I2CDevice index);
bool i2cWriteBuffer(uint8_t addr_, uint8_t reg_, uint8_t len_, uint8_t *data);
bool i2cWrite(uint8_t addr_, uint8_t reg, uint8_t data);

/**
 *
 * @param addr
 * @param reg
 * @param data
 * @param size <=16
 * @return true -> ok; false -> probleme
 */
bool i2cMemWriteSequence(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t size);
bool i2cRead(uint8_t addr_, uint8_t reg, uint16_t len, uint8_t* buf);
uint16_t i2cGetErrorCounter(void);
void i2cSetOverclock(uint8_t OverClock);


#endif /* HF_I2C_SOFTWARE_H_ */
