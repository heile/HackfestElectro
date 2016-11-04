/*
 * HF_debug_command.h
 *
 *  Created on: 22 oct. 2016
 *      Author: HD-L
 */

#ifndef HF_SHELL_H_
#define HF_SHELL_H_

#include "stdio.h"
#include "string.h"
#include "HF_functions.h"

typedef struct HF_CMD {
	char argv[4][15];
	uint8_t argc;
} HF_CMD;


char HF_shell_command_buffer[3][60];

#define HF_USB_VCP_RX_BUFFER_SIZE 32
extern uint8_t HF_usb_vcp_RxCount;

#define HF_RS232_RX_BUFFER_SIZE 32
extern uint8_t HF_rs232_RxCount;

#define HF_HACKER_UART_PORT_RX_BUFFER_SIZE 32
extern uint8_t HF_hacker_uart_port_RxCount;


typedef enum SHELL_COMMAND_CONSOLE_TYPE {
	SHELL_CONSOLE_TYPE_USB_VCP=0,
	SHELL_CONSOLE_TYPE_RS232,
	SHELL_CONSOLE_TYPE_HACKER_UART_PORT,
	SHELL_CONSOLE_TYPE_ALL,
}SHELL_COMMAND_CONSOLE_TYPE;


void HFShellCommand(char * command, SHELL_COMMAND_CONSOLE_TYPE buffer_type);
void process_shell_command(HF_CMD* cmd, SHELL_COMMAND_CONSOLE_TYPE buffer_type);

void HFParseArg(char* message, HF_CMD* cmd);

void HF_debugCommandSetBuffer(char c);

HF_FUNCTION_RETURN_STATE HF_shell_command_Read(uint8_t * fifo, SHELL_COMMAND_CONSOLE_TYPE buffer_type);

HF_FUNCTION_RETURN_STATE HF_shell_command_set_rx_buffer(uint8_t* Buf, uint32_t Len, SHELL_COMMAND_CONSOLE_TYPE buffer_type);

#endif /* HF_SHELL_H_ */
