/*
 * HF_debug_command.c
 *
 *  Created on: 22 oct. 2016
 *      Author: HD-L
 */

#include "HF_debug_command.h"
#include "stm32f0xx.h"

uint8_t HF_debug_command_RxHead=0;
uint8_t HF_debug_command_RxTail=0;
uint8_t HF_debug_command_RxBuffer[HF_DEBUG_COMMAND_RX_BUFFER_SIZE];

uint8_t HF_debug_command_RxCount=0;

void HFsetArg(char * message);

extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart1;


void HFdebugCommand(char * command) {
	int index = 0;

	//                    printf("\n\rDebug Command message: %s\n\r\n\r", debug_command_buffer);

	HFsetArg(command);

	printf("\r\nDebug Command:  ");
	for (index = 0; index < HF_argc; index++) {
		printf("%s ", HF_argv[index]);
	}
	printf("\r\n");
	HAL_Delay(2);

	if (strcmp(HF_argv[0], "command") == 0) {
		printf("Hello %d\r\n", 1);
//    	HAL_UART_Transmit(&huart3, "Hello\r\n", 7, 100);
	} else if (strcmp(HF_argv[0], "rf") == 0) {
		if (strcmp(HF_argv[1], "say") == 0) {
			HAL_GPIO_TogglePin(GPIOC, TEST_OUT_PIN_Pin);
			NRF24L01_Transmit(HF_argv[2]); //WiFi send "System Start!" to host
			HAL_GPIO_TogglePin(GPIOC, TEST_OUT_PIN_Pin);
			printf("RF Send: %s\r\n", HF_argv[2]);
		}

//    	HAL_UART_Transmit(&huart3, "Hello\r\n", 7, 100);
	}
}



void HF_debugCommandSetBuffer(char c) {
	static uint8_t i = 0;
	if (c == 0x7F) {
		if (i > 0)
			i -= 1;
		HF_debug_command_buffer[i] = 0x7F;
		CDC_Transmit_FS(&HF_debug_command_buffer[i], 1);
		HAL_UART_Transmit(&huart1, &HF_debug_command_buffer[i], 1, 2);
		HF_debug_command_buffer[i] = 0x00;
	} else if (c >= ' ' && c <= '~') {
		HF_debug_command_buffer[i++] = c;
		HF_debug_command_buffer[i] = 0x00;
		CDC_Transmit_FS(&c, 1);
		HAL_UART_Transmit(&huart1, &c, 1, 2);
	} else {
		//printf(" %#X ", c);
	}
	if (c == '\r') {
		HFdebugCommand(HF_debug_command_buffer);
		i = 0;
	}
}

void HFsetArg(char * message) {
	int i = 0;
	HF_argc = 0;
	memset(HF_argv[HF_argc], 0x00, 15);
	while ((*message != '\n') && (*message != '\r') && (*message != '\0')) {
		if (*message != ' ') {
			HF_argv[HF_argc][i++] = *message;
			message++;
		} else if ((*(message + 1) > ' ') && (*(message + 1) <= '~')) {
			HF_argc++;
			memset(HF_argv[HF_argc], 0x00, 15);
			i = 0;
			message++;
		} else {
			message++;
		}
		if ((HF_argc > 4) || i > 15) {
			return;
		}
	}
	HF_argc++;

}

HF_FUNCTION_RETURN_STATE HF_debug_command_Read(uint8_t * fifo) {

	if (HF_debug_command_RxCount == 0) {
		return HF_FUNCTION_RETURN_BUFFER_EMPTY;
	}

	fifo[0] = HF_debug_command_RxBuffer[HF_debug_command_RxTail++];
	if (sizeof(HF_debug_command_RxBuffer) <= HF_debug_command_RxTail) {
		HF_debug_command_RxTail = 0;
	}
	HF_debug_command_RxCount--;

	return HF_FUNCTION_RETURN_OK;
}

HF_FUNCTION_RETURN_STATE HF_debug_command_set_rx_buffer(uint8_t* Buf,
		uint32_t Len) {
	uint32_t size = 0;
	while (size < Len) {
		HF_debug_command_RxBuffer[HF_debug_command_RxHead++] = Buf[size];
		if (sizeof(HF_debug_command_RxBuffer) <= HF_debug_command_RxHead) {
			HF_debug_command_RxHead = 0;
		}
		HF_debug_command_RxCount++;
		size++;
		if (size >= sizeof(HF_debug_command_RxBuffer)) {
			break;
		}
	}
}
