/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_hal.h"
#include "usb_device.h"

/* USER CODE BEGIN Includes */
#include "nRF24L01.h"

#include "string.h"

#include "usbd_cdc_if.h"

#include "HF_timer_service.h"

#include "HF_printf.h"

#include "HF_I2C_Software.h"

#include "HF_debug_command.h"

#include "HF_IR_Receiver.h"
#include "HF_IR_Transmitter.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim16;
TIM_HandleTypeDef htim17;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC_Init(void);
static void MX_TIM17_Init(void);
static void MX_TIM16_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM7_Init(void);
static void MX_TIM6_Init(void);
static void MX_NVIC_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                
                                

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

char dataOut[32];
char rx_fifo_hacker_uart_port;
char rx_fifo_rs232;
uint32_t len;
uint8_t counter_ir;

///* My address */
//uint8_t MyAddress[] = { 'c', 'l', 'i', 'e', '1' };
//
///* Receiver address */
//uint8_t TxAddress[] = { 's', 'e', 'r', 'v', '1' };

/* My address */
uint8_t MyAddress[] = { 's', 'e', 'r', 'v', '1' };

/* Receiver address */
uint8_t TxAddress[] = { 'c', 'l', 'i', 'e', '1' };

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

void run_one_led() {

}

void run_seven_led() {

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart == &huart3) {
//		HAL_UART_Transmit(&huart3, dataOut, 1, 10);	//Replay Message received
		HF_shell_command_set_rx_buffer(&rx_fifo_rs232, 1,SHELL_CONSOLE_TYPE_RS232);
		HAL_UART_Receive_IT(&huart3, &rx_fifo_rs232, 1);
	} else if (huart == &huart1) {
		HF_shell_command_set_rx_buffer(&rx_fifo_hacker_uart_port, 1,SHELL_CONSOLE_TYPE_HACKER_UART_PORT);
		HAL_UART_Receive_IT(&huart1, &rx_fifo_hacker_uart_port, 1);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim == &htim17) {
//		static char sens = 0;
//		static uint32_t value = 0;
//		if (sens == 0) {
//			if (value < 1000) {
//				value++;
//			} else {
//				sens = 1;
//			}
//		} else {
//			if (value > 0) {
//				value--;
//			} else {
//				sens = 0;
//			}
//		}
//		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, value);
	}else if (htim == &htim7){
//		counter_ir++;
		hf_ir_reciever.timer_tick++;
		HfIrTransmitterSendCode();


	}else if (htim == &htim6){
		if (hf_ir_transmitter.state!=HF_IR_TRANSMITTER_STATE_READY){
			if (hf_ir_transmitter.bit_state==HF_IR_TRANSMITTER_BIT_STATE_PULSE){
				HAL_GPIO_TogglePin(GPIOB, IR_OUT_Pin);
				HAL_GPIO_WritePin(Test_GPIO_Port, Test_Pin,SET);
			}
			else{
				HAL_GPIO_WritePin(GPIOB, IR_OUT_Pin,RESET);
				HAL_GPIO_WritePin(Test_GPIO_Port, Test_Pin,RESET);
			}
		}
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	volatile static uint8_t counter = 0;
	volatile static uint8_t timer_value[80];
	if (GPIO_Pin == GPIO_PIN_0) {
		printf("Counter: %d\r\n",counter);
		counter=0;

		HAL_UART_Transmit(&huart3, "Botton Fn1\r\n", 12, 100);
	} else if (GPIO_Pin == GPIO_PIN_1) {
//		uint8_t i=0;
//		uint32_t value=0;
//		printf("Time Value:\r\n");
//		for(i=0;i<counter;i++){
//			if (i==1){
//				if (timer_value[i-1]-177<6 && timer_value[i-1]-87<6)		//NEC Code
//				printf("NEC Code: ");
//			}else if ((i%2)==1){
//				static int8_t position=31;
//				if (position>=0){
//					if ((timer_value[i]-timer_value[i-1])>6){
//						value |= (1<<position);
//						printf("1 ");
//					}else{
//						printf("0 ");
//					}
//				position--;
//				}
//			}
////			if (i==1){
////
////			}else if (i==17){
////				printf("    ");
////			}else if (i==33){
////				printf("\r\n");
////			}else if (i==49){
////				printf("    ");
////			}
//		}


//		HAL_UART_Transmit(&huart3, "Botton Fn2\r\n", 12, 100);
	} else if (GPIO_Pin == GPIO_PIN_2) {
//		if (counter==0){
//			counter_ir=0;
//			HAL_GPIO_TogglePin(GPIOB, IR_OUT_Pin);
//		}else if ((counter % 2) == 0) {
//			if(counter<80)
//				timer_value[counter-1] = counter_ir;
//			counter_ir=0;
//			HAL_GPIO_TogglePin(GPIOB, IR_OUT_Pin);
//		}else {
//			if(counter<80)
//				timer_value[counter-1] = counter_ir;
//			HAL_GPIO_TogglePin(GPIOB, IR_OUT_Pin);
//			counter_ir=0;
//		}
////		HAL_UART_Transmit(&huart3, "IR\r\n", 4, 100);
//		counter++;
		if (HAL_GPIO_ReadPin(IR_REC_GPIO_Port,IR_REC_Pin)==RESET){
			HfIrReceiverEdgeInterruption(HF_INTERRUPTION_EDGE_FALLING);
		}else{
			HfIrReceiverEdgeInterruption(HF_INTERRUPTION_EDGE_RSING);
		}

	}
}

//#ifdef __GNUC__
//  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
//     set to 'Yes') calls __io_putchar() */
//  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
//#else
//  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//#endif /* __GNUC__ */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART3_UART_Init();
  MX_USART1_UART_Init();
  MX_ADC_Init();
  MX_TIM17_Init();
  MX_TIM16_Init();
  MX_USB_DEVICE_Init();
  MX_SPI1_Init();
  MX_TIM7_Init();
  MX_TIM6_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();

  /* USER CODE BEGIN 2 */

	HAL_TIM_Base_Start(&htim2);

	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

	HAL_TIM_Base_Start(&htim3);

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);

	HAL_TIM_Base_Start_IT(&htim17);

	HAL_TIM_Base_Start_IT(&htim16);

	HAL_TIM_Base_Start_IT(&htim7);
	HAL_TIM_Base_Start_IT(&htim6);

	strcpy(dataOut, "System Start!\n\r");	//Message to print and send

	NRF24L01_Init(&hspi2, 1, 32);// Definition des pins sont dans le fichier STM32CubeMX, fichier Nucleo64_Test1.ioc
	NRF24L01_SetRF(NRF24L01_DataRate_2M, NRF24L01_OutputPower_M18dBm);
	NRF24L01_SetMyAddress(MyAddress);
	NRF24L01_SetTxAddress(TxAddress);

	HAL_GPIO_WritePin(GPIOC, TEST_OUT_PIN_Pin,RESET);
	NRF24L01_Transmit(dataOut);				//WiFi send "System Start!" to host
	HAL_GPIO_WritePin(GPIOC, TEST_OUT_PIN_Pin,SET);

	CDC_Transmit_FS(dataOut, strlen(dataOut));//USB(Virtual Com Port) send "System Start!" to PC

	/////////////////UART 3 /////////////////////////////
	HAL_UART_Transmit(&huart3, dataOut, strlen(dataOut), 100);//Uart send message "System Start!"

	HAL_UART_Receive_IT(&huart3, &rx_fifo_rs232, 1);

	//////////////////UART 1 //////////////////////////
	HAL_UART_Transmit(&huart1, dataOut, strlen(dataOut), 100);//Uart send message "System Start!"

	HAL_UART_Receive_IT(&huart1, &rx_fifo_hacker_uart_port, 1);

	/***************Test EEPROM ****************************/
	i2cWrite(0xA0, 0x00, 0x55);
	HAL_Delay(50);
	i2cMemWriteSequence(0xA0, 0x01, dataOut, 15);
	HAL_Delay(50);
	i2cRead(0xA0, 0x00, 20, dataOut);

	/***************Test RAM***********************************/
//	dataOut[0] = 0x05;	//Read status register
//	dataOut[1] = 0x00;
//	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_RESET);
//	HAL_SPI_TransmitReceive(&hspi1, dataOut, dataOut, 2, 100);
//	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_SET);
//	HAL_Delay(2);
//	dataOut[0] = 0x01;	//Write status register
//	dataOut[1] = 0x41;	//Set Sequential mode
//	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_RESET);
//	HAL_SPI_TransmitReceive(&hspi1, dataOut, dataOut, 2, 100);
//	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_SET);
//	HAL_Delay(2);
//	dataOut[0] = 0x02;	//Write Instruction
//	dataOut[1] = 0x00;
//	dataOut[2] = 0x00;
//	dataOut[3] = 0x02;
//	dataOut[4] = 0x03;
//	dataOut[5] = 0x04;
//	dataOut[6] = 0x05;
//	dataOut[7] = 0x06;
//	dataOut[8] = 0x07;
//	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_RESET);
//	HAL_SPI_TransmitReceive(&hspi1, dataOut, dataOut, 10, 100);
//	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_SET);
//	HAL_Delay(50);
//	dataOut[0] = 0x03;	//Read Instruction
//	dataOut[1] = 0x00;
//	dataOut[2] = 0x00;
//	dataOut[3] = 0x00;
//	dataOut[4] = 0x00;
//	dataOut[5] = 0x00;
//	dataOut[6] = 0x00;
//	dataOut[7] = 0x00;
//	dataOut[8] = 0x00;
//	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_RESET);
//	HAL_SPI_TransmitReceive(&hspi1, dataOut, dataOut, 15, 100);
//	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_SET);


	/***************************test IR Transmitter******************/
	if (hf_ir_transmitter.state == HF_IR_TRANSMITTER_STATE_READY){
		hf_ir_transmitter.nec_code.address=0x00;
		hf_ir_transmitter.nec_code.address_inverse=0xFF;
		hf_ir_transmitter.nec_code.command=0x2A;
		hf_ir_transmitter.nec_code.command_inverse=0xD5;
		hf_ir_transmitter.type_code = HF_IR_TRANSMITTER_CODE_TYPE_NEC;
		hf_ir_transmitter.state = HF_IR_TRANSMITTER_STATE_SET_HEADER;
	}


	systemTimerServiceSetTimer(HF_test_timer, HF_TIMER_MILLISECOND_AUTO_RESET,
			50);

	printf("Ready\r\n");


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
		if (systemTimerServiceCheckEnd(HF_test_timer) == 0) {
//			static char sens = 0;
//			if (sens == 0) {
//				if (htim17.Init.Period < 50000) {
//					htim17.Init.Period += 1000;
//				} else {
//					sens = 1;
//				}
//			} else {
//				if (htim17.Init.Period > 5000) {
//					htim17.Init.Period -= 1000;
//				} else {
//					sens = 0;
//				}
//			}
//			if (HAL_TIM_Base_Init(&htim17) != HAL_OK) {
//				Error_Handler();
//			}

			run_led_infinity();
//			HAL_GPIO_TogglePin(GPIOB, IR_OUT_Pin);
//			printf("Hello \n\r");
		}

//		if (HFusbRxCount > 0) {
//			uint8_t fifo;
//			HF_USB_Read(&fifo);
//			HF_debugCommandSetBuffer(fifo);
//			HAL_Delay(2);
//			//CDC_Transmit_FS(dataOut, 1);
////			printf("%c",fifo);
//		}
		if (HF_usb_vcp_RxCount > 0) {
			uint8_t fifo;
			HF_shell_command_Read(&fifo,SHELL_CONSOLE_TYPE_USB_VCP);
			HF_ShellCommandSetBuffer(fifo,SHELL_CONSOLE_TYPE_USB_VCP);
		}else if (HF_rs232_RxCount > 0) {
			uint8_t fifo;
			HF_shell_command_Read(&fifo,SHELL_CONSOLE_TYPE_RS232);
			HF_ShellCommandSetBuffer(fifo,SHELL_CONSOLE_TYPE_RS232);
		}else if (HF_hacker_uart_port_RxCount > 0) {
			uint8_t fifo;
			HF_shell_command_Read(&fifo,SHELL_CONSOLE_TYPE_HACKER_UART_PORT);
			HF_ShellCommandSetBuffer(fifo,SHELL_CONSOLE_TYPE_HACKER_UART_PORT);
		}

		if (HF_rf_buffer.rx_state.message_to_process == 1) {
			HF_rf_buffer.rx_state.message_to_process = 0;
			printf("RF Receive: %s\r\n", HF_rf_buffer.rx_buffer.buffer);
//			HAL_GPIO_TogglePin(GPIOC, TEST_OUT_PIN_Pin);
//			NRF24L01_Transmit(HF_rf_buffer.rx_buffer.buffer);		//WiFi send "System Start!" to host
//			HAL_GPIO_TogglePin(GPIOC, TEST_OUT_PIN_Pin);
		}

		if (hf_ir_reciever.state ==  HF_IR_RECEIVER_STATE_DATA_READY){
			hf_ir_reciever.state =  HF_IR_RECEIVER_STATE_DATA_READ;
			printf("\r\n%#X\r\n",hf_ir_reciever.data);
		}

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

	}
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14
                              |RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/** NVIC Configuration
*/
static void MX_NVIC_Init(void)
{
  /* USART3_4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART3_4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART3_4_IRQn);
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* EXTI0_1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
  /* EXTI2_3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);
}

/* ADC init function */
static void MX_ADC_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure for the selected ADC regular channel to be converted. 
    */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }

}

/* SPI2 init function */
static void MX_SPI2_Init(void)
{

  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }

}

/* TIM2 init function */
static void MX_TIM2_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 300;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigOC.Pulse = 500;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigOC.Pulse = 100;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigOC.Pulse = 800;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_TIM_MspPostInit(&htim2);

}

/* TIM3 init function */
static void MX_TIM3_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 300;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigOC.Pulse = 30;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_TIM_MspPostInit(&htim3);

}

/* TIM6 init function */
static void MX_TIM6_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 0;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 632;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

/* TIM7 init function */
static void MX_TIM7_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;

  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 0;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 2700;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

/* TIM16 init function */
static void MX_TIM16_Init(void)
{

  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 0;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 48000;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }

}

/* TIM17 init function */
static void MX_TIM17_Init(void)
{

  htim17.Instance = TIM17;
  htim17.Init.Prescaler = 0;
  htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim17.Init.Period = 48000;
  htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim17.Init.RepetitionCounter = 0;
  if (HAL_TIM_Base_Init(&htim17) != HAL_OK)
  {
    Error_Handler();
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}

/* USART3 init function */
static void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin : PC13_Pin */
  GPIO_InitStruct.Pin = PC13_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PC13_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Button_Fn1_Pin Button_Fn2_Pin */
  GPIO_InitStruct.Pin = Button_Fn1_Pin|Button_Fn2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : IR_REC_Pin */
  GPIO_InitStruct.Pin = IR_REC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(IR_REC_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : RF_IRQ_Pin */
  GPIO_InitStruct.Pin = RF_IRQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(RF_IRQ_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RF_CE_Pin SPI2_CS_RF_Pin */
  GPIO_InitStruct.Pin = RF_CE_Pin|SPI2_CS_RF_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : TEST_OUT_PIN_Pin EEPROM_WP_Pin */
  GPIO_InitStruct.Pin = TEST_OUT_PIN_Pin|EEPROM_WP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : RAM_HOLD_Pin */
  GPIO_InitStruct.Pin = RAM_HOLD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RAM_HOLD_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI1_CS_RAM_Pin */
  GPIO_InitStruct.Pin = SPI1_CS_RAM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SPI1_CS_RAM_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Test_Pin */
  GPIO_InitStruct.Pin = Test_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Test_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : I2C1_SDA_EEPROM_Pin I2C1_SCL_EEPROM_Pin */
  GPIO_InitStruct.Pin = I2C1_SDA_EEPROM_Pin|I2C1_SCL_EEPROM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : IR_OUT_Pin */
  GPIO_InitStruct.Pin = IR_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(IR_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RF_CE_Pin|Test_Pin|IR_OUT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SPI2_CS_RF_Pin|I2C1_SDA_EEPROM_Pin|I2C1_SCL_EEPROM_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, TEST_OUT_PIN_Pin|EEPROM_WP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RAM_HOLD_GPIO_Port, RAM_HOLD_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_SET);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

}

/* USER CODE BEGIN 4 */

///**
//  * @brief  Retargets the C library printf function to the USART.
//  * @param  None
//  * @retval None
//  */
//PUTCHAR_PROTOTYPE
//{
//  /* Place your implementation of fputc here */
//  /* e.g. write a character to the USART */
////  USART_SendData(EVAL_COM1, (uint8_t) ch);
//	HAL_UART_Transmit(&huart3, &ch, 1, 100);
//
//  /* Loop until the end of transmission */
////  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
////  {}
//
//  return ch;
//}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
	/* User can add his own implementation to report the HAL error return state */
	while (1) {
	}
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
