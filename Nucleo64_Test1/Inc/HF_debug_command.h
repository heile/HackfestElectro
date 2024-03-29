/*
 * HF_debug_command.h
 *
 *  Created on: 22 oct. 2016
 *      Author: HD-L
 */

#ifndef HF_DEBUG_COMMAND_H_
#define HF_DEBUG_COMMAND_H_

#include <stdio.h>
#include <string.h>
#include "HF_functions.h"


char HF_argv[5][15];
char HF_argc;


char HF_shell_command_buffer[3][50];

#define HF_USB_VCP_RX_BUFFER_SIZE 32
extern uint8_t HF_usb_vcp_RxCount;

#define HF_RS232_RX_BUFFER_SIZE 32
extern uint8_t HF_rs232_RxCount;

#define HF_HACKER_UART_PORT_RX_BUFFER_SIZE 32
extern uint8_t HF_hacker_uart_port_RxCount;


typedef enum {
	SHELL_CONSOLE_TYPE_USB_VCP=0,
	SHELL_CONSOLE_TYPE_RS232,
	SHELL_CONSOLE_TYPE_HACKER_UART_PORT,
	SHELL_CONSOLE_TYPE_ALL,
}SHELL_COMMAND_CONSOLE_TYPE;


void HF_debugCommandSetBuffer(char c);

HF_FUNCTION_RETURN_STATE HF_shell_command_Read(uint8_t * fifo, SHELL_COMMAND_CONSOLE_TYPE buffer_type);

HF_FUNCTION_RETURN_STATE HF_shell_command_set_rx_buffer(uint8_t* Buf, uint32_t Len, SHELL_COMMAND_CONSOLE_TYPE buffer_type);

#endif /* HF_DEBUG_COMMAND_H_ */
