/*
 * HF_debug_command.c
 *
 *  Created on: 22 oct. 2016
 *      Author: HD-L
 */


#include "stm32f0xx.h"
#include "aes.h"
#include "HF_shell.h"
#include <HF_server.h>
#include <HF_flags.h>
#include "HF_leds.h"
#include "HF_IR_Transmitter.h"
#include "HF_IR_Receiver.h"
#include "HF_eeprom_ram.h"

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

const char VERSION_STRING[] = "HF Board v0.01 by Lei and Martin for Hackfest 2016.\r\n";

extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart1;

extern LED_PATTERN_NAME LED_RUNNING_PATTERN;		//
extern SHELL_COMMAND_CONSOLE_TYPE;


//extern uint8_t* FLAG_3;

MEM_ADDR ROM_SELECTED_ADDR = { .fifo=900 }; 	// max = 2048
MEM_ADDR RAM_SELECTED_ADDR = { .fifo=1600 };	// max = 8192


void HFShellCommand(char * command, SHELL_COMMAND_CONSOLE_TYPE buffer_type) {
    HF_CMD cmd;

    HFParseArg(command,&cmd);

    if (strcmp(cmd.argv[0],"") != 0){
    	hf_print_back("\r\n", buffer_type);
		process_shell_command(&cmd,buffer_type);
    }else{
    	hf_print_back("\r\n", buffer_type);
    }

    hf_print_back("hfserver>", buffer_type);
}

void process_shell_command(HF_CMD* cmd, SHELL_COMMAND_CONSOLE_TYPE buffer_type){
	char flag_buffer[FLAG_LEN];

    if (strcmp(cmd->argv[0], "help") == 0) {

    	hf_print_back("Welcome on the reactor server. Standard commands are:\r\n", buffer_type);
    	hf_print_back("  help: Print this help message. \r\n", buffer_type);
    	hf_print_back("  version: Print version. \r\n", buffer_type);
    	hf_print_back("\r\n", buffer_type);
    	hf_print_back("Enable the debug mode for more output.\r\n", buffer_type);
       	hf_print_back("\r\n", buffer_type);

		hf_print_back("Some additional commands...\r\n", buffer_type);
		hf_print_back("  ir wr t <string_no_space>: Write text.\r\n", buffer_type);
		hf_print_back("  ir wr h <0x...>: Write hex.\r\n", buffer_type);
		hf_print_back("  wifi wr t <string_no_space>: Write text.\r\n", buffer_type);
		hf_print_back("  wifi wr h <0x...>: Write hex.\r\n", buffer_type);
		hf_print_back("\r\n", buffer_type);


		hf_print_back("Some additional commands...\r\n", buffer_type);
		hf_print_back("  rom se <1-2byte (0x..)>: Select an address to read/write.\r\n", buffer_type);
		hf_print_back("  rom wr t <string_no_space>: Write text (up to 96 chars).\r\n", buffer_type);
		hf_print_back("  rom rd t <len>: Read text (up to 96 chars).\r\n", buffer_type);
		hf_print_back("  rom rd h <len>: Read hex (up to 96 chars).\r\n", buffer_type);
		hf_print_back("  ram se <1-2byte (0x..)>: Select an address to read/write.\r\n", buffer_type);
		hf_print_back("  ram wr t <string_no_space>: Write text (up to 96 chars).\r\n", buffer_type);
		hf_print_back("  ram rd t <len>: Read text (up to 96 chars).\r\n", buffer_type);
		hf_print_back("  ram rd h <len>: Read hex (up to 96 chars).\r\n", buffer_type);
		hf_print_back("\r\n", buffer_type);

		hf_print_back("Some hidden commands...\r\n", buffer_type);
		hf_print_back("  flag: Print a flag.\r\n", buffer_type);
		hf_print_back("\r\n", buffer_type);

    } else if (strcmp(cmd->argv[0], "version") == 0) {
    	hf_print_back(VERSION_STRING, buffer_type);
    }
    else if (strcmp(cmd->argv[0], "ir") == 0){
    	if (cmd->argc > 2){
			process_ir_wr(cmd->argv[2], buffer_type);
		} else {
			hf_print_back("Wrong arguments\r\n", buffer_type);
		}
	} else if (strcmp(cmd->argv[0], "wifi") == 0){
		if (cmd->argc > 2){
			process_wifi_wr(cmd->argv[2], buffer_type);
		} else {
			hf_print_back("Wrong arguments\r\n", buffer_type);
		}
	} else if (strcmp(cmd->argv[0], "rom") == 0){
    	if (cmd->argc > 1){
			if (strcmp(cmd->argv[1], "se") == 0) {		// rom select
				if (cmd->argc > 2){
					process_rom_select(cmd->argv[2], buffer_type);
				} else if (cmd->argc == 2){
					process_rom_select_print(buffer_type);
				} else {
					hf_print_back("Wrong arguments\r\n", buffer_type);
				}
			} else if (strcmp(cmd->argv[1], "wr") == 0) {		// rom wr
				if (cmd->argc > 3){
					process_rom_wr(cmd->argv[2], cmd->argv[3], buffer_type);
				} else {
					hf_print_back("Wrong arguments\r\n", buffer_type);
				}
			} else if (strcmp(cmd->argv[1], "rd") == 0) {		// rom rd
				if (cmd->argc > 3){
					process_rom_rd(cmd->argv[2], cmd->argv[3], buffer_type);
				} else {
					hf_print_back("Wrong arguments\r\n", buffer_type);
				}
			} else if (strcmp(cmd->argv[1], "le") == 0) {
				uint8_t buffer[2048];
				eeprom_cat24c16_read((uint8_t)strtol(cmd->argv[2], NULL, 0),buffer,(uint16_t)strtol(cmd->argv[3], NULL, 0));
			} else if (strcmp(cmd->argv[1], "ec") == 0) {
				int i=0;
				uint8_t buffer[2048];
				for (i=0;i<30;i++){
					buffer[i]=i;
				}
				eeprom_cat24c16_write((uint8_t)strtol(cmd->argv[2], NULL, 0),buffer,(uint16_t)strtol(cmd->argv[3], NULL, 0));
			}
    	} else {
        	hf_print_back("Wrong arguments\r\n", buffer_type);
    	}
    } else if (strcmp(cmd->argv[0], "ram") == 0){
    	if (cmd->argc > 1){
			if (strcmp(cmd->argv[1], "se") == 0) {		// rom select
				if (cmd->argc > 2){
					process_ram_select(cmd->argv[2], buffer_type);
				} else if (cmd->argc == 2){
					process_ram_select_print(buffer_type);
				} else {
					hf_print_back("Wrong arguments\r\n", buffer_type);
				}
			} else if (strcmp(cmd->argv[1], "wr") == 0) {		// rom wr
				if (cmd->argc > 3){
					process_ram_wr(cmd->argv[2], cmd->argv[3], buffer_type);
				} else {
					hf_print_back("Wrong arguments\r\n", buffer_type);
				}
			} else if (strcmp(cmd->argv[1], "rd") == 0) {		// rom rd
				if (cmd->argc > 3){
					process_ram_rd(cmd->argv[2], cmd->argv[3], buffer_type);
				} else {
					hf_print_back("Wrong arguments\r\n", buffer_type);
				}
			}else if (strcmp(cmd->argv[1], "le") == 0) {
				uint8_t buffer[2048];
				ram_23k640_read((uint8_t)strtol(cmd->argv[2], NULL, 0),buffer,(uint16_t)strtol(cmd->argv[3], NULL, 0));
			} else if (strcmp(cmd->argv[1], "ec") == 0) {
				int i=0;
				uint8_t buffer[2048];
				for (i=0;i<30;i++){
					buffer[i]=i;
				}
				ram_23k640_write((uint8_t)strtol(cmd->argv[2], NULL, 0),buffer,(uint16_t)strtol(cmd->argv[3], NULL, 0));
			}
    	} else {
        	hf_print_back("Wrong arguments\r\n", buffer_type);
    	}
    } else if (strcmp(cmd->argv[0], "bomb") == 0){
		HAL_GPIO_WritePin(GPIOC, R3_Liquid_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, R2_Sirene_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, R1_Light_Pin, GPIO_PIN_RESET);
    }
}

void process_rom_select_print(SHELL_COMMAND_CONSOLE_TYPE buffer_type){
	char buf[32];
	sprintf(buf, "Addr=%#x\r\n", ROM_SELECTED_ADDR.fifo);
	hf_print_back(buf, buffer_type);
}

void process_rom_select(char* addr, SHELL_COMMAND_CONSOLE_TYPE buffer_type){
	uint16_t iAddr;

	iAddr = (uint16_t)strtol(addr, NULL, 0);

	// Validate addresses
	if (iAddr > 0 && iAddr < 2048){
		ROM_SELECTED_ADDR.fifo = iAddr;
		hf_print_back("Address selected.\r\n", buffer_type);

	} else {
		hf_print_back("Invalid address.\r\n", buffer_type);
	}
}

void process_rom_wr(char* type, char* in, SHELL_COMMAND_CONSOLE_TYPE buffer_type){
	// validations
	if (strlen(in) > MEM_ACCESS_MAX_LEN){
		hf_print_back("Message too long\r\n", buffer_type);
		return;
	}
	eeprom_write_str(&ROM_SELECTED_ADDR, in);
	hf_print_back("Message written.\r\n", buffer_type);
}

void process_rom_rd(char* type, char* len, SHELL_COMMAND_CONSOLE_TYPE buffer_type){
	uint8_t iLen;

	iLen = (uint8_t)strtol(len, NULL, 0);

	// validations
	if (iLen > MEM_ACCESS_MAX_LEN){
		hf_print_back("Length too long\r\n", buffer_type);
		return;
	}

	if(strcmp(type,"h") == 0){
		eeprom_print_hex(&ROM_SELECTED_ADDR, iLen, buffer_type);
	} else {
		eeprom_print_str(&ROM_SELECTED_ADDR, iLen, buffer_type);
	}
}


void process_ram_select_print(SHELL_COMMAND_CONSOLE_TYPE buffer_type){
	char buf[32];
	sprintf(buf, "Addr=%#x\r\n", RAM_SELECTED_ADDR.fifo);
	hf_print_back(buf, buffer_type);
}

void process_ram_select(char* addr, SHELL_COMMAND_CONSOLE_TYPE buffer_type){
	uint16_t iAddr;

	iAddr = (uint16_t)strtol(addr, NULL, 0);

	// Validate addresses
	if (iAddr > 0 && iAddr < 8192){
		RAM_SELECTED_ADDR.fifo = iAddr;
		hf_print_back("Address selected.\r\n", buffer_type);
	} else {
		hf_print_back("Invalid address.\r\n", buffer_type);
	}
}

void process_ram_wr(char* type, char* in, SHELL_COMMAND_CONSOLE_TYPE buffer_type){
	// validations
	if (strlen(in) > MEM_ACCESS_MAX_LEN){
		hf_print_back("Message too long\r\n", buffer_type);
		return;
	}
	ram_write_str(&RAM_SELECTED_ADDR, in);
	hf_print_back("Message written.\r\n", buffer_type);
}

void process_ram_rd(char* type, char* len, SHELL_COMMAND_CONSOLE_TYPE buffer_type){
	uint8_t iLen;

	iLen = (uint8_t)strtol(len, NULL, 0);

	// validations
	if (iLen > MEM_ACCESS_MAX_LEN){
		hf_print_back("Length too long\r\n", buffer_type);
		return;
	}

	if(strcmp(type,"h") == 0){
		ram_print_hex(&RAM_SELECTED_ADDR, iLen, buffer_type);
	} else {
		ram_print_str(&RAM_SELECTED_ADDR, iLen, buffer_type);
	}
}

void process_ir_wr(char* in, SHELL_COMMAND_CONSOLE_TYPE buffer_type){
	hf_print_back("Not part of this program.\r\n", buffer_type);
}

void process_wifi_wr(char* in, SHELL_COMMAND_CONSOLE_TYPE buffer_type){
	// validations
	if (strlen(in) > 32){
		hf_print_back("Message too long. Max 32 bytes\r\n", buffer_type);
		return;
	}

	HAL_GPIO_WritePin(GPIOC, TEST_OUT_PIN_Pin, RESET);
	NRF24L01_Transmit(in);
	HAL_GPIO_WritePin(GPIOC, TEST_OUT_PIN_Pin, SET);

	hf_print_back("Message sent.\r\n", buffer_type);
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
			HFShellCommand(HF_shell_command_buffer[SHELL_CONSOLE_TYPE_USB_VCP], SHELL_CONSOLE_TYPE_USB_VCP);
			index_usb_buffer = 0;
			memset(HF_shell_command_buffer[SHELL_CONSOLE_TYPE_USB_VCP], 0x00, 50);
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
			HFShellCommand(HF_shell_command_buffer[SHELL_CONSOLE_TYPE_RS232], SHELL_CONSOLE_TYPE_RS232);
			index_rs232_buffer = 0;
			memset(HF_shell_command_buffer[SHELL_CONSOLE_TYPE_RS232], 0x00, 50);
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
			HFShellCommand(HF_shell_command_buffer[SHELL_CONSOLE_TYPE_HACKER_UART_PORT], SHELL_CONSOLE_TYPE_HACKER_UART_PORT);
			index_hacker_usart_port_buffer = 0;
			memset(HF_shell_command_buffer[SHELL_CONSOLE_TYPE_HACKER_UART_PORT], 0x00, 50);

		}
		break;
	}
}

void HFParseArg(char* message, HF_CMD* cmd) {
	int i = 0;
	cmd->argc = 0;
	memset(cmd->argv[cmd->argc], 0x00, 15);
	while ((*message != '\n') && (*message != '\r') && (*message != '\0')) {
		if (*message != ' ') {
			cmd->argv[cmd->argc][i++] = *message;
			message++;
		} else if ((*(message + 1) > ' ') && (*(message + 1) <= '~')) {
			cmd->argc++;
			memset(cmd->argv[cmd->argc], 0x00, 15);
			i = 0;
			message++;
		} else {
			message++;
		}
		if ((cmd->argc > 4) || i > 15) {
			return;
		}
	}
	cmd->argc++;


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

void test_ir_transmitter(void){
	/***************************test IR Transmitter******************/
	if (hf_ir_transmitter.state == HF_IR_TRANSMITTER_STATE_READY){
		HfIrTransmitterSetData(0x00,0x2A,HF_IR_TRANSMITTER_CODE_TYPE_NEC);
		hf_ir_transmitter.state = HF_IR_TRANSMITTER_STATE_SNED;
	}
}

void test_ir_receiver(void){
	if (hf_ir_reciever.state ==  HF_IR_RECEIVER_STATE_DATA_READY){
		hf_ir_reciever.state =  HF_IR_RECEIVER_STATE_DATA_READ;
		printf("\r\n%#X\r\n",hf_ir_reciever.data);
		printf("Address %#X, Command %#X\r\n",hf_ir_reciever.address,hf_ir_reciever.command);
	}
}

