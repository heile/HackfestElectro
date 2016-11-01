/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * Copyright (c) 2016 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define PC13_Pin GPIO_PIN_13
#define PC13_GPIO_Port GPIOC
#define Button_Fn1_Pin GPIO_PIN_0
#define Button_Fn1_GPIO_Port GPIOC
#define Button_Fn2_Pin GPIO_PIN_1
#define Button_Fn2_GPIO_Port GPIOC
#define IR_REC_Pin GPIO_PIN_2
#define IR_REC_GPIO_Port GPIOC
#define LED1_Pin GPIO_PIN_0
#define LED1_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_1
#define LED2_GPIO_Port GPIOA
#define LED3_Pin GPIO_PIN_2
#define LED3_GPIO_Port GPIOA
#define LED4_Pin GPIO_PIN_3
#define LED4_GPIO_Port GPIOA
#define PA4_Pin GPIO_PIN_4
#define PA4_GPIO_Port GPIOA
#define SPI1_SCK_RAM_Pin GPIO_PIN_5
#define SPI1_SCK_RAM_GPIO_Port GPIOA
#define USART3_CTS_RS232_Pin GPIO_PIN_6
#define USART3_CTS_RS232_GPIO_Port GPIOA
#define USART3_TX_RS232_Pin GPIO_PIN_4
#define USART3_TX_RS232_GPIO_Port GPIOC
#define USART3_RX_RS232_Pin GPIO_PIN_5
#define USART3_RX_RS232_GPIO_Port GPIOC
#define RF_IRQ_Pin GPIO_PIN_10
#define RF_IRQ_GPIO_Port GPIOB
#define RF_CE_Pin GPIO_PIN_11
#define RF_CE_GPIO_Port GPIOB
#define SPI2_CS_RF_Pin GPIO_PIN_12
#define SPI2_CS_RF_GPIO_Port GPIOB
#define SPI2_SCK_RF_Pin GPIO_PIN_13
#define SPI2_SCK_RF_GPIO_Port GPIOB
#define SPI2_MISO_RF_Pin GPIO_PIN_14
#define SPI2_MISO_RF_GPIO_Port GPIOB
#define SPI2_MOSI_RF_Pin GPIO_PIN_15
#define SPI2_MOSI_RF_GPIO_Port GPIOB
#define LED5_Pin GPIO_PIN_6
#define LED5_GPIO_Port GPIOC
#define LED6_Pin GPIO_PIN_7
#define LED6_GPIO_Port GPIOC
#define LED7_Pin GPIO_PIN_8
#define LED7_GPIO_Port GPIOC
#define TEST_OUT_PIN_Pin GPIO_PIN_9
#define TEST_OUT_PIN_GPIO_Port GPIOC
#define PA9_UART1_TX_Pin GPIO_PIN_9
#define PA9_UART1_TX_GPIO_Port GPIOA
#define PA10_UART1_RX_Pin GPIO_PIN_10
#define PA10_UART1_RX_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define EEPROM_WP_Pin GPIO_PIN_11
#define EEPROM_WP_GPIO_Port GPIOC
#define RAM_HOLD_Pin GPIO_PIN_12
#define RAM_HOLD_GPIO_Port GPIOC
#define SPI1_CS_RAM_Pin GPIO_PIN_2
#define SPI1_CS_RAM_GPIO_Port GPIOD
#define Test_Pin GPIO_PIN_3
#define Test_GPIO_Port GPIOB
#define SPI1_MISO_RAM_Pin GPIO_PIN_4
#define SPI1_MISO_RAM_GPIO_Port GPIOB
#define SPI1_MOSI_RAM_Pin GPIO_PIN_5
#define SPI1_MOSI_RAM_GPIO_Port GPIOB
#define I2C1_SDA_EEPROM_Pin GPIO_PIN_6
#define I2C1_SDA_EEPROM_GPIO_Port GPIOB
#define I2C1_SCL_EEPROM_Pin GPIO_PIN_7
#define I2C1_SCL_EEPROM_GPIO_Port GPIOB
#define IR_OUT_Pin GPIO_PIN_9
#define IR_OUT_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */


/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
