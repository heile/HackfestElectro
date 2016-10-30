/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2015/09/hal-library-25-nrf24l01-for-stm32fxxx/
 * @version v1.0
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   Library template
 *
\verbatim
   ----------------------------------------------------------------------
    Copyright (C) Tilen Majerle, 2015

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
\endverbatim
 */
#ifndef NRF24L01_H
#define NRF24L01_H 100

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup STM32Fxxx_HAL_Libraries
 * @{
 */

/**
 * @defgroup NRF24L01P
 * @brief    nRF24L01+ library for STM32xxx devices - http://stm32f4-discovery.com/2015/09/hal-library-25-nrf24l01-for-stm32fxxx/
 * @{
 *
 * This library allows you to work with nRF24L01+ modules.
 *
 * You can send and receive data from nRF24L01+ modules.
 *
 * \par Default pinout
 *
\verbatim
NRF24L01+	STM32Fxxx	DESCRIPTION

GND			GND			Ground
VCC			3.3V		3.3V
CE			PD8			RF activated pin
CSN			PD7			Chip select pin for SPI
SCK			PC10		SCK pin for SPI
MOSI		PC12		MOSI pin for SPI
MISO		PC11		MISO pin for SPI
IRQ			Not used	Interrupt pin. Goes low when active. Pin functionality is active, but not used in library
\endverbatim
 *
 * IRQ pin is not used in this library, but its functionality is enabled by this software.
 *
 * You can still set any pin on Fxxx to be an external interrupt and handle interrupts from nRF24L01+ module.
 *
 * The easiest way to that is to use @ref EXTI library and attach interrupt functionality to this pin
 *
 * \par Custom pinout
 *
 * Add lines below in your defines.h file if you want to change default pinout:
 *
\code
//Change SPI used. Refer to TM SPI library to check which pins are for SPI


//Change CSN pin. This is for SPI communication


//Change CE pin. This pin is used to enable/disable transmitter/receiver functionality

\endcode
 *
 * \par Changelog
 *
\verbatim
 Version 1.0
  - First release
\endverbatim
 *
 * \par Dependencies
 *
\verbatim
 - STM32Fxxx HAL
 - defines.h
 - TM SPI
 - TM GPIO
\endverbatim
 */
#include "stm32f0xx_hal.h"

#include "string.h"
/**
 * @defgroup NRF24L01P_Macros
 * @brief    Library defines
 * @{
 */

/* Default SPI used */


/* SPI chip enable pin */
//#define NRF24L01_CSN_PORT			SPI2_CS_RF_GPIO_Port
//#define NRF24L01_CSN_PIN			SPI2_CS_RF_Pin
//
///* Chip enable for transmitting */
//#define NRF24L01_CE_PORT			RF_CE_GPIO_Port
//#define NRF24L01_CE_PIN				RF_CE_Pin

/* Pins configuration */
//#define NRF24L01_CE_LOW				HAL_GPIO_WritePin(NRF24L01_CE_PORT, NRF24L01_CE_PIN, GPIO_PIN_RESET)
//#define NRF24L01_CE_HIGH			HAL_GPIO_WritePin(NRF24L01_CE_PORT, NRF24L01_CE_PIN, GPIO_PIN_SET)
//#define NRF24L01_CSN_LOW			HAL_GPIO_WritePin(NRF24L01_CSN_PORT, NRF24L01_CSN_PIN, GPIO_PIN_RESET)
//#define NRF24L01_CSN_HIGH			HAL_GPIO_WritePin(NRF24L01_CSN_PORT, NRF24L01_CSN_PIN, GPIO_PIN_SET)
#define NRF24L01_CE_LOW				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET)
#define NRF24L01_CE_HIGH			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET)
#define NRF24L01_CSN_LOW			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)
#define NRF24L01_CSN_HIGH			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)

/* Interrupt masks */
#define NRF24L01_IRQ_DATA_READY     0x40 /*!< Data ready for receive */
#define NRF24L01_IRQ_TRAN_OK        0x20 /*!< Transmission went OK */
#define NRF24L01_IRQ_MAX_RT         0x10 /*!< Max retransmissions reached, last transmission failed */

/**
 * @}
 */

/**
 * @defgroup NRF24L01P_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Interrupt structure
 */
typedef union _NRF24L01_IRQ_t {
	struct {
		uint8_t reserved0:4;
		uint8_t MaxRT:1;     /*!< Set to 1 if MAX retransmissions flag is set */
		uint8_t DataSent:1;  /*!< Set to 1 if last transmission is OK */
		uint8_t DataReady:1; /*!< Set to 1 if data are ready to be read */
		uint8_t reserved1:1;
	} F;
	uint8_t Status;          /*!< NRF status register value */
} NRF24L01_IRQ_t;


/**
 *
 */
typedef struct _RF_buffer {
	union _rx_buffer {
		uint8_t buffer[32];
		struct{
			uint8_t header:8;
			uint8_t address:8;
		};
	} rx_buffer;
	union _rx_state {
		uint8_t state;
		struct {
			uint8_t message_to_process :1;
			uint8_t ready_to_receive :1;
		};
	} rx_state;

	union _tx_buffer {
		uint8_t buffer[32];
	} tx_buffer;
	union _tx_state {
		uint8_t state;
		struct {
			uint8_t message_to_process :1;
			uint8_t ready_to_send :1;
		};
	} tx_state;
} RF_buffer;


/**
 * @brief  Transmission status enumeration
 */
typedef enum _NRF24L01_Transmit_Status_t {
	NRF24L01_Transmit_Status_Lost = 0x00,   /*!< Message is lost, reached maximum number of retransmissions */
	NRF24L01_Transmit_Status_Ok = 0x01,     /*!< Message sent successfully */
	NRF24L01_Transmit_Status_Sending = 0xFF /*!< Message is still sending */
} NRF24L01_Transmit_Status_t;

/**
 * @brief  Data rate enumeration
 */
typedef enum _NRF24L01_DataRate_t {
	NRF24L01_DataRate_2M = 0x00, /*!< Data rate set to 2Mbps */
	NRF24L01_DataRate_1M,        /*!< Data rate set to 1Mbps */
	NRF24L01_DataRate_250k       /*!< Data rate set to 250kbps */
} NRF24L01_DataRate_t;

/**
 * @brief  Output power enumeration
 */
typedef enum _NRF24L01_OutputPower_t {
	NRF24L01_OutputPower_M18dBm = 0x00,/*!< Output power set to -18dBm */
	NRF24L01_OutputPower_M12dBm,       /*!< Output power set to -12dBm */
	NRF24L01_OutputPower_M6dBm,        /*!< Output power set to -6dBm */
	NRF24L01_OutputPower_0dBm          /*!< Output power set to 0dBm */
} NRF24L01_OutputPower_t;

/**
 * @}
 */


RF_buffer HF_rf_buffer;


void SPI_Send(SPI_HandleTypeDef* SPIx, uint8_t data);

void SPI_SendMulti(SPI_HandleTypeDef* SPIx, uint8_t* dataOut, uint8_t* dataIn, uint32_t count);

void SPI_WriteMulti(SPI_HandleTypeDef* SPIx, uint8_t* dataOut, uint32_t count);

void SPI_ReadMulti(SPI_HandleTypeDef* SPIx, uint8_t *dataIn, uint8_t dummy, uint32_t count);

/**
 * @defgroup NRF24L01P_Functions
 * @brief    Library Functions
 *
 * Here are listed very basic functions to work with NRF modules
 *
 * @{
 */

/**
 * @brief  Initializes NRF24L01+ module
 * @param  channel: channel you will use for communication, from 0 to 125 eg. working frequency from 2.4 to 2.525 GHz
 * @param  payload_size: maximum data to be sent in one packet from one NRF to another.
 * @note   Maximal payload size is 32bytes
 * @retval 1
 */
uint8_t NRF24L01_Init(SPI_HandleTypeDef *hspi, uint8_t channel, uint8_t payload_size);

/**
 * @brief  Sets own address. This is used for settings own id when communication with other modules
 * @note   "Own" address of one device must be the same as "TX" address of other device (and vice versa),
 *         if you want to get successful communication
 * @param  *adr: Pointer to 5-bytes length array with address
 * @retval None
 */
void NRF24L01_SetMyAddress(uint8_t* adr);

/**
 * @brief  Sets address you will communicate with
 * @note   "Own" address of one device must be the same as "TX" address of other device (and vice versa),
 *         if you want to get successful communication
 * @param  *adr: Pointer to 5-bytes length array with address
 * @retval None
 */
void NRF24L01_SetTxAddress(uint8_t* adr);

/**
 * @brief  Gets number of retransmissions needed in last transmission
 * @param  None
 * @retval Number of retransmissions, between 0 and 15.
 */
uint8_t NRF24L01_GetRetransmissionsCount(void);

/**
 * @brief  Sets NRF24L01+ to TX mode
 * @note   In this mode is NRF able to send data to another NRF module
 * @param  None
 * @retval None
 */
void NRF24L01_PowerUpTx(void);

/**
 * @brief  Sets NRF24L01+ to RX mode
 * @note   In this mode is NRF able to receive data from another NRF module.
 *         This is default mode and should be used all the time, except when sending data
 * @param  None
 * @retval None
 */
void NRF24L01_PowerUpRx(void);

void NRF24L01_Flush_Tx(void);

/**
 * @brief  Sets NRF24L01+ to power down mode
 * @note   In power down mode, you are not able to transmit/receive data.
 *         You can wake up device using @ref NRF24L01_PowerUpTx() or @ref NRF24L01_PowerUpRx() functions
 * @param  None
 * @retval None
 */
void NRF24L01_PowerDown(void);

/**
 * @brief  Gets transmissions status
 * @param  None
 * @retval Transmission status. Return is based on @ref NRF24L01_Transmit_Status_t enumeration
 */
NRF24L01_Transmit_Status_t NRF24L01_GetTransmissionStatus(void);

/**
 * @brief  Transmits data with NRF24L01+ to another NRF module
 * @param  *data: Pointer to 8-bit array with data.
 *         Maximum length of array can be the same as "payload_size" parameter on initialization
 * @retval None
 */
void NRF24L01_Transmit(uint8_t *data);

/**
 * @brief  Checks if data is ready to be read from NRF24L01+
 * @param  None
 * @retval Data ready status:
 *            - 0: No data available for receive in bufferReturns
 *            - > 0: Data is ready to be collected
 */
uint8_t NRF24L01_DataReady(void);

/**
 * @brief  Gets data from NRF24L01+
 * @param  *data: Pointer to 8-bits array where data from NRF will be saved
 * @retval None
 */
void NRF24L01_GetData(uint8_t *data);

/**
 * @brief  Sets working channel
 * @note   Channel value is just an offset in units MHz from 2.4GHz
 *         For example, if you select channel 65, then operation frequency will be set to 2.465GHz.
 * @param  channel: RF channel where device will operate
 * @retval None
 */
void NRF24L01_SetChannel(uint8_t channel);

/**
 * @brief  Sets RF parameters for NRF24L01+
 * @param  DataRate: Data rate selection for NRF module. This parameter can be a value of @ref NRF24L01_DataRate_t enumeration
 * @param  OutPwr: Output power selection for NRF module. This parameter can be a value of @ref NRF24L01_OutputPower_t enumeration
 * @retval None
 */
void NRF24L01_SetRF(NRF24L01_DataRate_t DataRate, NRF24L01_OutputPower_t OutPwr);

/**
 * @brief  Gets NRLF+ status register value
 * @param  None
 * @retval Status register from NRF
 */
//uint8_t NRF24L01_GetStatus(void);
uint8_t NRF24L01_GetStatus(void);

/**
 * @brief  Reads interrupts from NRF
 * @param  *IRQ: Pointer to @ref NRF24L01_IRQ_t where IRQ status will be saved
 * @retval IRQ status
 *            - 0: No interrupts are active
 *            - > 0: At least one interrupt is active
 */
uint8_t NRF24L01_Read_Interrupts(NRF24L01_IRQ_t* IRQ);

/**
 * @brief  Clears interrupt status
 * @param  None
 * @retval None
 */
void NRF24L01_Clear_Interrupts(uint8_t value);

/* Private */
void NRF24L01_WriteRegister(uint8_t reg, uint8_t value);

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif

