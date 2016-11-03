/*
 * HF_debug_command.c
 *
 *  Created on: 22 oct. 2016
 *      Author: HD-L
 */

#include "stm32f0xx.h"
#include "HF_unit_tests.h"
#include "HF_shell.h"
#include "HF_leds.h"

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

void HFdebugCommand(char * command) {
	int index = 0;
	HF_CMD cmd;

	//                    printf("\n\rDebug Command message: %s\n\r\n\r", debug_command_buffer);

	HFParseArg(command, &cmd);

	printf("\r\nDebug Command:  ");
	for (index = 0; index < cmd.argc; index++) {
		printf("%s ", cmd.argv[index]);
	}
	printf("\r\n");
	HAL_Delay(2);

	if (strcmp(cmd.argv[0], "command") == 0) {
		printf("Hello %d\r\n", 1);
//    	HAL_UART_Transmit(&huart3, "Hello\r\n", 7, 100);
	} else if (strcmp(cmd.argv[0], "rf") == 0) {
		if (strcmp(cmd.argv[1], "say") == 0) {
			HAL_GPIO_TogglePin(GPIOC, TEST_OUT_PIN_Pin);
			NRF24L01_Transmit(cmd.argv[2]); //WiFi send "System Start!" to host
			HAL_GPIO_TogglePin(GPIOC, TEST_OUT_PIN_Pin);
			printf("RF Send: %s\r\n", cmd.argv[2]);
		}

//    	HAL_UART_Transmit(&huart3, "Hello\r\n", 7, 100);
	}
}

void HFShellCommand(char * command, SHELL_COMMAND_CONSOLE_TYPE buffer_type) {
    int index = 0;
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
    if (strcmp(cmd->argv[0], "help") == 0) {
    	hf_print_back("This is a help message. Commands are:\r\n", buffer_type);
    	hf_print_back("  test: Test the board devices. \r\n", buffer_type);
    	hf_print_back("  enc_flags: Encrypt all flags and print their encrypted value. \r\n", buffer_type);
    	hf_print_back("  version: Print version. \r\n", buffer_type);
    	hf_print_back("\r\n", buffer_type);
    	hf_print_back("test sub-command:\r\n", buffer_type);
    	hf_print_back("  all: Test all devices.\r\n", buffer_type);
    	hf_print_back("  usb: Test USB. This test is interactive. \r\n", buffer_type);
    	hf_print_back("  rs232: Test RS232. This test is interactive. \r\n", buffer_type);
    	hf_print_back("  hacker: Test Hacker connector (near the supercapacitor). This test is interactive. \r\n", buffer_type);
    	hf_print_back("  wifi: Test wifi (You need a server running).\r\n", buffer_type);
    	hf_print_back("  eeprom: Test the eeprom. A string will be written and read.\r\n", buffer_type);
    	hf_print_back("  eeprom read: Test a read on eeprom. Use this to test a read after a board shutdown.\r\n", buffer_type);
    	hf_print_back("  ram: Test the ram. A string will be written and read.\r\n", buffer_type);
    	hf_print_back("  led: Test the LEDs. Several pattern will be played.\r\n", buffer_type);
    	hf_print_back("  led infinity: Run the infinity pattern.\r\n", buffer_type);
    	hf_print_back("  led dinfinity: Run the double infinity pattern.\r\n", buffer_type);
    	hf_print_back("  led dloop: Run the double loop pattern.\r\n", buffer_type);
    	hf_print_back("  led flash: Run the flash pattern.\r\n", buffer_type);
    	hf_print_back("  encryption: Test the encryption routine.\r\n", buffer_type);
    	hf_print_back("  decryption: Test the decryption routine.\r\n", buffer_type);
    	hf_print_back("  encrypt <word>: Encrypt a word and print the result.\r\n", buffer_type);
    	hf_print_back("\r\n", buffer_type);
    	hf_print_back("Press the button 2 to change led patterns\r\n", buffer_type);
    } else if (strcmp(cmd->argv[0], "test") == 0) {
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
				test_ir_transmitter();
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
    		} else if (strcmp(cmd->argv[1], "encryption") == 0) {
    			test_encrypt_cbc();
    		} else if (strcmp(cmd->argv[1], "decryption") == 0) {
    			test_decrypt_cbc();
    		}else if (strcmp(cmd->argv[1], "encrypt") == 0) {
    			if (cmd->argc > 2) {
    				test_encrypt_message(cmd->argv[2]);
    			}
    		}else if (strcmp(cmd->argv[1], "ir") == 0) {
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
