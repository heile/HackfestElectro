/*
 * HF_debug_command.c
 *
 *  Created on: 22 oct. 2016
 *      Author: HD-L
 */

#include "HF_debug_command.h"
#include "stm32f0xx.h"


/***************Buffer USB ********************/
uint8_t HF_usb_vcp_RxHead = 0;
uint8_t HF_usb_vcp_RxTail = 0;
uint8_t HF_usb_vcp_RxBuffer[HF_USB_VCP_RX_BUFFER_SIZE];

uint8_t HF_usb_vcp_RxCount = 0;

/****************Buffer Rs232******************/
uint8_t HF_rs232_RxHead = 0;
uint8_t HF_rs232_RxTail = 0;
uint8_t HF_rs232_RxBuffer[HF_RS232_RX_BUFFER_SIZE];

uint8_t HF_rs232_RxCount = 0;

/***************Buffer Hacker uart port********/
uint8_t HF_hacker_uart_port_RxHead = 0;
uint8_t HF_hacker_uart_port_RxTail = 0;
uint8_t HF_hacker_uart_port_RxBuffer[HF_HACKER_UART_PORT_RX_BUFFER_SIZE];

uint8_t HF_hacker_uart_port_RxCount = 0;

/****************End buffer declaration*******/

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
			HAL_GPIO_WritePin(GPIOC, TEST_OUT_PIN_Pin,RESET);
			NRF24L01_Transmit(HF_argv[2]); //WiFi send "System Start!" to host
			HAL_GPIO_WritePin(GPIOC, TEST_OUT_PIN_Pin,SET);
			printf("RF Send: %s\r\n", HF_argv[2]);
		}

//    	HAL_UART_Transmit(&huart3, "Hello\r\n", 7, 100);
	}
}


void HF_ShellCommandSetBuffer(char c, SHELL_COMMAND_CONSOLE_TYPE buffer_type) {
	static uint8_t index_usb_buffer = 0;
	static uint8_t index_rs232_buffer = 0;
	static uint8_t index_hacker_usart_port_buffer = 0;
	switch (buffer_type) {
	case SHELL_CONSOLE_TYPE_USB_VCP:
		if (c == 0x7F) {
			if (index_usb_buffer > 0)
				index_usb_buffer -= 1;
			HF_shell_command_buffer[SHELL_CONSOLE_TYPE_USB_VCP][index_usb_buffer] =0x7F;
			CDC_Transmit_FS(&HF_shell_command_buffer[SHELL_CONSOLE_TYPE_USB_VCP][index_usb_buffer],1);
			HF_shell_command_buffer[SHELL_CONSOLE_TYPE_USB_VCP][index_usb_buffer] =0x00;
		} else if (c >= ' ' && c <= '~') {
			HF_shell_command_buffer[SHELL_CONSOLE_TYPE_USB_VCP][index_usb_buffer++] =c;
			HF_shell_command_buffer[SHELL_CONSOLE_TYPE_USB_VCP][index_usb_buffer] =0x00;
			CDC_Transmit_FS(&c, 1);
		} else {
			//printf(" %#X ", c);
		}
		if (c == '\r') {
			HFdebugCommand(HF_shell_command_buffer[SHELL_CONSOLE_TYPE_USB_VCP]);
			index_usb_buffer = 0;
		}
		break;
	case SHELL_CONSOLE_TYPE_RS232:
		if (c == 0x7F) {
			if (index_rs232_buffer > 0)
				index_rs232_buffer -= 1;
			HF_shell_command_buffer[SHELL_CONSOLE_TYPE_RS232][index_rs232_buffer] =0x7F;
			HAL_UART_Transmit(&huart3,&HF_shell_command_buffer[SHELL_CONSOLE_TYPE_RS232][index_rs232_buffer],1, 2);
			HF_shell_command_buffer[SHELL_CONSOLE_TYPE_RS232][index_rs232_buffer] =0x00;
		} else if (c >= ' ' && c <= '~') {
			HF_shell_command_buffer[SHELL_CONSOLE_TYPE_RS232][index_rs232_buffer++] =c;
			HF_shell_command_buffer[SHELL_CONSOLE_TYPE_RS232][index_rs232_buffer] =0x00;
			HAL_UART_Transmit(&huart3, &c, 1, 2);
		} else {
			//printf(" %#X ", c);
		}
		if (c == '\r') {
			HFdebugCommand(HF_shell_command_buffer[SHELL_CONSOLE_TYPE_RS232]);
			index_rs232_buffer = 0;
		}
		break;
	case SHELL_CONSOLE_TYPE_HACKER_UART_PORT:
		if (c == 0x7F) {
			if (index_hacker_usart_port_buffer > 0)
				index_hacker_usart_port_buffer -= 1;
			HF_shell_command_buffer[SHELL_CONSOLE_TYPE_HACKER_UART_PORT][index_hacker_usart_port_buffer] =0x7F;
			CDC_Transmit_FS(&HF_shell_command_buffer[SHELL_CONSOLE_TYPE_HACKER_UART_PORT][index_hacker_usart_port_buffer],1);
			HAL_UART_Transmit(&huart1,&HF_shell_command_buffer[SHELL_CONSOLE_TYPE_HACKER_UART_PORT][index_hacker_usart_port_buffer],1, 2);
			HF_shell_command_buffer[SHELL_CONSOLE_TYPE_HACKER_UART_PORT][index_hacker_usart_port_buffer] =0x00;
		} else if (c >= ' ' && c <= '~') {
			HF_shell_command_buffer[SHELL_CONSOLE_TYPE_HACKER_UART_PORT][index_hacker_usart_port_buffer++] =c;
			HF_shell_command_buffer[SHELL_CONSOLE_TYPE_HACKER_UART_PORT][index_hacker_usart_port_buffer] =0x00;
			HAL_UART_Transmit(&huart1, &c, 1, 2);
		} else {
			//printf(" %#X ", c);
		}
		if (c == '\r') {
			HFdebugCommand(HF_shell_command_buffer[SHELL_CONSOLE_TYPE_HACKER_UART_PORT]);
			index_hacker_usart_port_buffer = 0;
		}
		break;
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


HF_FUNCTION_RETURN_STATE HF_shell_command_Read(uint8_t * fifo,
		SHELL_COMMAND_CONSOLE_TYPE buffer_type) {
	switch (buffer_type) {
	case SHELL_CONSOLE_TYPE_USB_VCP:
		if (HF_usb_vcp_RxCount == 0) {
			return HF_FUNCTION_RETURN_BUFFER_EMPTY;
		}
		fifo[0] = HF_usb_vcp_RxBuffer[HF_usb_vcp_RxTail++];
		if (sizeof(HF_usb_vcp_RxBuffer) <= HF_usb_vcp_RxTail) {
			HF_usb_vcp_RxTail = 0;
		}
		HF_usb_vcp_RxCount--;
		return HF_FUNCTION_RETURN_OK;
		break;
	case SHELL_CONSOLE_TYPE_RS232:
		if (HF_rs232_RxCount == 0) {
			return HF_FUNCTION_RETURN_BUFFER_EMPTY;
		}
		fifo[0] = HF_rs232_RxBuffer[HF_rs232_RxTail++];
		if (sizeof(HF_rs232_RxBuffer) <= HF_rs232_RxTail) {
			HF_rs232_RxTail = 0;
		}
		HF_rs232_RxCount--;
		return HF_FUNCTION_RETURN_OK;
		break;
	case SHELL_CONSOLE_TYPE_HACKER_UART_PORT:
		if (HF_hacker_uart_port_RxCount == 0) {
			return HF_FUNCTION_RETURN_BUFFER_EMPTY;
		}
		fifo[0] = HF_hacker_uart_port_RxBuffer[HF_hacker_uart_port_RxTail++];
		if (sizeof(HF_hacker_uart_port_RxBuffer)
				<= HF_hacker_uart_port_RxTail) {
			HF_hacker_uart_port_RxTail = 0;
		}
		HF_hacker_uart_port_RxCount--;
		return HF_FUNCTION_RETURN_OK;
		break;
	}
}

HF_FUNCTION_RETURN_STATE HF_shell_command_set_rx_buffer(uint8_t* Buf,
		uint32_t Len, SHELL_COMMAND_CONSOLE_TYPE buffer_type) {
	uint32_t size = 0;
	switch (buffer_type) {
	case SHELL_CONSOLE_TYPE_USB_VCP:
		while (size < Len) {
			HF_usb_vcp_RxBuffer[HF_usb_vcp_RxHead++] = Buf[size];
			if (sizeof(HF_usb_vcp_RxBuffer) <= HF_usb_vcp_RxHead) {
				HF_usb_vcp_RxHead = 0;
			}
			HF_usb_vcp_RxCount++;
			size++;
			if (size >= sizeof(HF_usb_vcp_RxBuffer)) {
				break;
			}
		}
		break;
	case SHELL_CONSOLE_TYPE_RS232:
		while (size < Len) {
			HF_rs232_RxBuffer[HF_rs232_RxHead++] = Buf[size];
			if (sizeof(HF_rs232_RxBuffer) <= HF_rs232_RxHead) {
				HF_rs232_RxHead = 0;
			}
			HF_rs232_RxCount++;
			size++;
			if (size >= sizeof(HF_rs232_RxBuffer)) {
				break;
			}
		}
		break;
	case SHELL_CONSOLE_TYPE_HACKER_UART_PORT:
		while (size < Len) {
			HF_hacker_uart_port_RxBuffer[HF_hacker_uart_port_RxHead++] =
					Buf[size];
			if (sizeof(HF_hacker_uart_port_RxBuffer)
					<= HF_hacker_uart_port_RxHead) {
				HF_hacker_uart_port_RxHead = 0;
			}
			HF_hacker_uart_port_RxCount++;
			size++;
			if (size >= sizeof(HF_hacker_uart_port_RxBuffer)) {
				break;
			}
		}
		break;
	}

}
