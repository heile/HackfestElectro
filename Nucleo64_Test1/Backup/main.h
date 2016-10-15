/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define Button_Fn1_Pin GPIO_PIN_0
#define Button_Fn1_GPIO_Port GPIOC
#define Button_Fn2_Pin GPIO_PIN_1
#define Button_Fn2_GPIO_Port GPIOC
#define IR_REC_Pin GPIO_PIN_2
#define IR_REC_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define PA4_Pin GPIO_PIN_4
#define PA4_GPIO_Port GPIOA
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
#define SPI1_MISO_RAM_Pin GPIO_PIN_4
#define SPI1_MISO_RAM_GPIO_Port GPIOB
#define SPI1_MOSI_RAM_Pin GPIO_PIN_5
#define SPI1_MOSI_RAM_GPIO_Port GPIOB
#define I2C1_SDA_EEPROM_Pin GPIO_PIN_6
#define I2C1_SDA_EEPROM_GPIO_Port GPIOB
#define I2C1_SCL_EEPROM_Pin GPIO_PIN_7
#define I2C1_SCL_EEPROM_GPIO_Port GPIOB
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
