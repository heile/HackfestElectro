/*
 * HF_debug_command.c
 *
 *  Created on: 22 oct. 2016
 *      Author: HD-L
 */


#include "stm32f0xx.h"
#include "aes.h"
#include "HF_shell.h"
#include <HF_player.h>
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

MEM_ADDR ROM_SELECTED_ADDR = {.page=0xA0, .addr=0};
MEM_ADDR RAM_SELECTED_ADDR = {.page=0, .addr=0};


void HFShellCommand(char * command, SHELL_COMMAND_CONSOLE_TYPE buffer_type) {
    HF_CMD cmd;

    HFParseArg(command,&cmd);

    if (strcmp(cmd.argv[0],"") != 0){
    	hf_print_back("\r\n", buffer_type);
		process_shell_command(&cmd,buffer_type);
    }else{
    	hf_print_back("\r\n", buffer_type);
    }

    hf_print_back("hfboard>", buffer_type);
}

void process_shell_command(HF_CMD* cmd, SHELL_COMMAND_CONSOLE_TYPE buffer_type){
	char flag_buffer[FLAG_LEN];

    if (strcmp(cmd->argv[0], "help") == 0) {
    	hf_print_back("Welcome on the reactor remote. Standard commands are:\r\n", buffer_type);
    	hf_print_back("  help: Print this help message. \r\n", buffer_type);
    	hf_print_back("  version: Print version. \r\n", buffer_type);
    	hf_print_back("\r\n", buffer_type);
    	hf_print_back("Enable the debug mode for more features.\r\n", buffer_type);
       	hf_print_back("\r\n", buffer_type);
    	hf_print_back("rom se <1byte (0x..)> <1byte (0x..)>: Select a page and address to read/write.\r\n", buffer_type);
    	hf_print_back("rom wr t <string_no_space>: Write text (up to 96 chars).\r\n", buffer_type);
    	hf_print_back("rom rd t <len>: Read text (up to 96 chars).\r\n", buffer_type);
    	hf_print_back("rom rd h <len>: Read hex (up to 96 chars).\r\n", buffer_type);
    	hf_print_back("ram se <1byte (0x..)> <1byte (0x..)>: Select a page and address to read/write.\r\n", buffer_type);
    	hf_print_back("ram wr t <string_no_space>: Write text (up to 96 chars).\r\n", buffer_type);
    	hf_print_back("ram rd t <len>: Read text (up to 96 chars).\r\n", buffer_type);
    	hf_print_back("ram rd h <len>: Read hex (up to 96 chars).\r\n", buffer_type);
    	hf_print_back("ir wr t <string_no_space>: Write text (up to 96 chars).\r\n", buffer_type);
    	hf_print_back("wifi wr t <string_no_space>: Write text (up to 96 chars).\r\n", buffer_type);
       	hf_print_back("\r\n", buffer_type);
     	if (buffer_type == SHELL_CONSOLE_TYPE_HACKER_UART_PORT){
    		hf_print_back("Some commands hidden in the Hacker UART port.\r\n", buffer_type);
    		hf_print_back("  flag: Print a flag.\r\n", buffer_type);
    	}
    } else if (strcmp(cmd->argv[0], "version") == 0) {
    	hf_print_back(VERSION_STRING, buffer_type);
    } else if (strcmp(cmd->argv[0], "rom") == 0){
    	if (cmd->argc > 1){
			if (strcmp(cmd->argv[1], "se") == 0) {		// rom select
				if (cmd->argc > 3){
					process_rom_select(cmd->argv[2], cmd->argv[3], buffer_type);
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
				eeprom_cat24c16_read((uint8_t)strtol(cmd->argv[2], NULL, 0),buffer,(uint8_t)strtol(cmd->argv[3], NULL, 0));
			} else if (strcmp(cmd->argv[1], "ec") == 0) {
				int i=0;
				uint8_t buffer[2048];
				for (i=0;i<30;i++){
					buffer[i]=i;
				}
				eeprom_cat24c16_write((uint8_t)strtol(cmd->argv[2], NULL, 0),buffer,(uint8_t)strtol(cmd->argv[3], NULL, 0));
			}
    	} else {
        	hf_print_back("Wrong arguments\r\n", buffer_type);
    	}
    } else if (strcmp(cmd->argv[0], "ram") == 0){
    	if (cmd->argc > 1){
			if (strcmp(cmd->argv[1], "se") == 0) {		// rom select
				if (cmd->argc > 3){
					process_ram_select(cmd->argv[2], cmd->argv[3], buffer_type);
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
			}
    	} else {
        	hf_print_back("Wrong arguments\r\n", buffer_type);
    	}
    } else if (strcmp(cmd->argv[0], "ir") == 0){

    } else if (strcmp(cmd->argv[0], "wifi") == 0){

    }

    // Commands specific to HACKER port
    if (buffer_type == SHELL_CONSOLE_TYPE_HACKER_UART_PORT){
    	if (strcmp(cmd->argv[0], "flag") == 0){
           	memset(flag_buffer, 0x00, FLAG_LEN);
           	get_flag(FLAG3,flag_buffer);
           	hf_print_hacker("Flag: ");
           	hf_print_hacker(flag_buffer);
           	hf_print_hacker("\r\n");
        }
    }


    /*
    else if (strcmp(cmd->argv[0], "test") == 0) {
    	if (cmd->argc > 1){
    		if (strcmp(cmd->argv[1], "all") == 0) {
    			hf_print_back("Testing back in your shell!\r\n", buffer_type);
    			hf_print_usb("Test USB!\r\n");
    			hf_print_rs232("Test RS232!\r\n");
    			hf_print_hacker("Test Hacker!\r\n");
    			test_wifi();
    			test_eeprom_write();
    			test_eeprom_read();
    			test_ram();
				hf_print_back("Running double infinity pattern.\r\n", buffer_type);
				LED_RUNNING_PATTERN = DOUBLE_INFINITY;
    		} else if (strcmp(cmd->argv[1], "usb") == 0) {
    			hf_print_usb("Test USB!\r\n");
    		} else if (strcmp(cmd->argv[1], "rs232") == 0) {
    			hf_print_rs232("Test RS232!\r\n");
    		} else if (strcmp(cmd->argv[1], "hacker") == 0) {
    			hf_print_hacker("Test Hacker!\r\n");
    		} else if (strcmp(cmd->argv[1], "wifi") == 0) {
    			test_wifi();
    		} else if (strcmp(cmd->argv[1], "eeprom") == 0) {
    			if (cmd->argc > 2 && strcmp(cmd->argv[2], "read") == 0) {
    				test_eeprom_read();
    			} else {
        			test_eeprom_write();
        			test_eeprom_read();
    			}
    		} else if (strcmp(cmd->argv[1], "ram") == 0) {
    			test_ram();
    		} else if (strcmp(cmd->argv[1], "led") == 0) {
    			if (cmd->argc > 2 && strcmp(cmd->argv[2], "infinity") == 0) {
    				hf_print_back("Running infinity pattern.\r\n", buffer_type);
    				LED_RUNNING_PATTERN = INFINITY;
    			} else if (cmd->argc > 2 && strcmp(cmd->argv[2], "dinfinity") == 0) {
    				hf_print_back("Running double infinity pattern.\r\n", buffer_type);
    				LED_RUNNING_PATTERN = DOUBLE_INFINITY;
    			} else if (cmd->argc > 2 && strcmp(cmd->argv[2], "dloop") == 0) {
    				hf_print_back("Running double loop pattern.\r\n", buffer_type);
    				LED_RUNNING_PATTERN = DOUBLE_LOOP;
    			} else {
    				hf_print_back("Running infinity pattern.\r\n", buffer_type);
    				LED_RUNNING_PATTERN = INFINITY;
    			}
    		} else if (strcmp(cmd->argv[1], "ir") == 0) {
    			if (cmd->argc > 2 && strcmp(cmd->argv[2], "tx") == 0) {
    				test_ir_transmitter();
    			}else{
    				test_ir_transmitter();
    			}
    		}


    	}
    } else if (strcmp(cmd->argv[0], "enc_flags") == 0) {
    	generate_flags();
    }else if (strcmp(cmd->argv[0], "version") == 0) {
    	hf_print_back(VERSION_STRING, buffer_type);
    }
    */
}

void process_rom_select_print(SHELL_COMMAND_CONSOLE_TYPE buffer_type){
	char buf[32];
	sprintf(buf, "Page=%#x, Addr=%#x\r\n", ROM_SELECTED_ADDR.page, ROM_SELECTED_ADDR.addr);
	hf_print_back(buf, buffer_type);
}

void process_rom_select(char* page, char* addr, SHELL_COMMAND_CONSOLE_TYPE buffer_type){
	uint16_t iPage;
	uint16_t iAddr;

	// Validate addresses
	if (strlen(page) > 3 && strlen(addr) > 2){
		hf_print_back("Invalid address. Page <= 127; Addr <= 15\r\n", buffer_type);
	}
	// TODO: Put more validations based on the component.

	iPage = (uint16_t)strtol(page, NULL, 0);
	iAddr = (uint16_t)strtol(addr, NULL, 0);
	//sscanf(page, "%x", &iPage);
	//sscanf(addr, "%x", &iAddr);

	ROM_SELECTED_ADDR.page = iPage;
	ROM_SELECTED_ADDR.addr = iAddr;

	hf_print_back("Address selected.\r\n", buffer_type);
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
	sprintf(buf, "Page=%#x, Addr=%#x\r\n", RAM_SELECTED_ADDR.page, RAM_SELECTED_ADDR.addr);
	hf_print_back(buf, buffer_type);
}

void process_ram_select(char* page, char* addr, SHELL_COMMAND_CONSOLE_TYPE buffer_type){
	uint16_t iPage;
	uint16_t iAddr;

	// Validate addresses
	if (strlen(page) > 4 && strlen(addr) > 4){
		hf_print_back("Invalid address.\r\n", buffer_type);
	}
	// TODO: Put more validations based on the component.

	iPage = (uint16_t)strtol(page, NULL, 0);
	iAddr = (uint16_t)strtol(addr, NULL, 0);
	//sscanf(page, "%x", &iPage);
	//sscanf(addr, "%x", &iAddr);

	RAM_SELECTED_ADDR.page = iPage;
	RAM_SELECTED_ADDR.addr = iAddr;

	hf_print_back("Address selected.\r\n", buffer_type);
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

