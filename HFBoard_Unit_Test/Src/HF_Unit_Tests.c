#include "main.h"
#include "stdio.h"
#include "nRF24L01.h"
#include "HF_printf.h"
#include "HF_I2C_Software.h"
#include "HF_debug_command.h"

/*
 * List of tests handled by this file:
 *
 *  - Buttons (1 and 2)
 *  - UART on RS232 connector
 *  - UART on USB
 *  - EEPROM (write, read and compare)
 *  - RAM (write, read and compare)
 *  - Infra Red (require remote)
 *  - Wifi (send and receive, require server)
 *  -
 */

char VERSION_STRING[] = "HF Board v0.01 by Lei and Martin for Hackfest 2016.\r\n";

char fifo_command;

char rx_fifo_hacker_uart_port;
char rx_fifo_rs232;

uint8_t counter_ir;

// All of these handlers should comes from main.c
extern ADC_HandleTypeDef hadc;
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

// USB + RS232 + Hacker
// WIFI
void handle_incoming_message(){

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

	//wifi
	if (HF_rf_buffer.rx_state.message_to_process == 1) {
		HF_rf_buffer.rx_state.message_to_process = 0;
		hf_print("RF Receive: ");
		hf_print(HF_rf_buffer.rx_buffer.buffer);
		hf_print("\r\n");
//			HAL_GPIO_TogglePin(GPIOC, TEST_OUT_PIN_Pin);
//			NRF24L01_Transmit(HF_rf_buffer.rx_buffer.buffer);		//WiFi send "System Start!" to host
//			HAL_GPIO_TogglePin(GPIOC, TEST_OUT_PIN_Pin);
	}
}

// RS232 Receiver
// Hacker Receiver
void uart_callback(UART_HandleTypeDef *huart) {
	if (huart == &huart3) {
//		HAL_UART_Transmit(&huart3, dataOut, 1, 10);	//Replay Message received
		HF_shell_command_set_rx_buffer(&rx_fifo_rs232, 1,SHELL_CONSOLE_TYPE_RS232);
		HAL_UART_Receive_IT(&huart3, &rx_fifo_rs232, 1);
	} else if (huart == &huart1) {
		HF_shell_command_set_rx_buffer(&rx_fifo_hacker_uart_port, 1,SHELL_CONSOLE_TYPE_HACKER_UART_PORT);
		HAL_UART_Receive_IT(&huart1, &rx_fifo_hacker_uart_port, 1);
	}
}

// Button 1
// Button 2
// IR Receiver
void exti_callback(uint16_t GPIO_Pin){
	volatile static uint8_t counter = 0;
		volatile static uint8_t timer_value[80];
		if (GPIO_Pin == GPIO_PIN_0) {
			printf("Counter: %d\r\n",counter);
			counter=0;
			HAL_UART_Transmit(&huart3, "Botton Fn1\r\n", 12, 100);
		} else if (GPIO_Pin == GPIO_PIN_1) {
			uint8_t i=0;
			uint32_t value=0;
			printf("Time Value:\r\n");
			for(i=0;i<counter;i++){
				if (i==1){
					if (timer_value[i-1]-177<6 && timer_value[i-1]-87<6)		//NEC Code
					printf("NEC Code: ");
				}else if ((i%2)==1){
					static int8_t position=31;
					if (position>=0){
						if ((timer_value[i]-timer_value[i-1])>6){
							value |= (1<<position);
							printf("1 ");
						}else{
							printf("0 ");
						}
					position--;
					}
				}
	//			if (i==1){
	//
	//			}else if (i==17){
	//				printf("    ");
	//			}else if (i==33){
	//				printf("\r\n");
	//			}else if (i==49){
	//				printf("    ");
	//			}
			}
			printf("\r\n%#X\r\n",value);
			HAL_UART_Transmit(&huart3, "Botton Fn2\r\n", 12, 100);
		} else if (GPIO_Pin == GPIO_PIN_2) {
			if (counter==0){
				counter_ir=0;
				HAL_GPIO_TogglePin(GPIOB, IR_OUT_Pin);
			}else if ((counter % 2) == 0) {
				if(counter<80)
					timer_value[counter-1] = counter_ir;
				counter_ir=0;
				HAL_GPIO_TogglePin(GPIOB, IR_OUT_Pin);
			}else {
				if(counter<80)
					timer_value[counter-1] = counter_ir;
				HAL_GPIO_TogglePin(GPIOB, IR_OUT_Pin);
				counter_ir=0;
			}
	//		HAL_UART_Transmit(&huart3, "IR\r\n", 4, 100);
			counter++;
		}
}

void HFShellCommand(char * command) {
    int index = 0;
    HF_CMD cmd;

    //printf("\n\rDebug Command message: %s\n\r\n\r", debug_command_buffer);
    HFParseArg(command,&cmd);

    if (strcmp(cmd.argv[0],"") != 0){
		//hf_print("\r\nDebug Command: ");
		for (index = 0; index < cmd.argc; index++) {
			hf_print(cmd.argv[index]);
			hf_print(" ");
		}
		hf_print("\r\n");

		process_shell_command(&cmd);
    }else{
    	hf_print("\r\n");
    }

    hf_print("hfboard>");
}

void process_shell_command(HF_CMD* cmd){
    if (strcmp(cmd->argv[0], "help") == 0) {
    	hf_print("This is a help message. Commands are:\r\n");
    	hf_print("  test: Test the board devices. Supported arguments are: all, usb, rs232, hacker, wifi, eeprom, ram, led. \r\n");
    	hf_print("  version: Print version. \r\n");
    } else if (strcmp(cmd->argv[0], "test") == 0) {
    	if (cmd->argc > 1){
    		if (strcmp(cmd->argv[1], "all") == 0) {
    			hf_print("Testing all the things!\r\n");
    			hf_print_usb("Test USB!\r\n");
    			hf_print_rs232("Test RS232!\r\n");
    			hf_print_hacker("Test Hacker!\r\n");
    			test_wifi();
    			test_eeprom_write();
    			test_eeprom_read();
    			test_ram();
    			test_led();
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
    			test_led();
    		}
    	} else {
        	hf_print("You must specify an argument:\r\n");
        	hf_print("  all: Test all devices.\r\n");
        	hf_print("  usb: Test USB. This test is interactive. \r\n");
        	hf_print("  rs232: Test RS232. This test is interactive. \r\n");
        	hf_print("  hacker: Test Hacker connector (near the supercapacitor). This test is interactive. \r\n");
        	hf_print("  wifi: Test wifi (You need a server running).\r\n");
        	hf_print("  eeprom: Test the eeprom. A string will be written and read.\r\n");
        	hf_print("  ram: Test the ram. A string will be written and read.\r\n");
        	hf_print("  led: Test the LEDs. Several pattern will be played.\r\n");

    	}
    } else if (strcmp(cmd->argv[0], "version") == 0) {
    	hf_print(VERSION_STRING);
    }
}

void hf_print(char* pBuffer){
	hf_print_usb(pBuffer);
	hf_print_rs232(pBuffer);
	hf_print_hacker(pBuffer);
	//HAL_Delay(2);
}

void hf_print_usb(char* pBuffer){
	CDC_Transmit_FS(pBuffer, strlen(pBuffer));
}

void hf_print_rs232(char* pBuffer){
	HAL_UART_Transmit(&huart3, pBuffer, strlen(pBuffer), 10);
}

void hf_print_hacker(char* pBuffer){
	HAL_UART_Transmit(&huart1, pBuffer, strlen(pBuffer), 10);
}

void print_on_start(){
	hf_print("\r\n"); HAL_Delay(2);
	hf_print("Hello all!\r\n"); HAL_Delay(2);
	hf_print_usb("Hello USB!\r\n"); HAL_Delay(2);
	hf_print_rs232("Hello RS232!\r\n"); HAL_Delay(2);
}

void uart_init(){
	HAL_UART_Receive_IT(&huart1, &rx_fifo_hacker_uart_port, 1);
	HAL_UART_Receive_IT(&huart3, &rx_fifo_rs232, 1);
}

void test_wifi(){
	char dataOut[32] = "Love is in the air.";
	uint8_t MyAddress[] = { 's', 'e', 'r', 'v', '1' };	/* My address */
	uint8_t TxAddress[] = { 'c', 'l', 'i', 'e', '1' };	/* Receiver address */

	NRF24L01_Init(&hspi2, 1, 32);// Definition des pins sont dans le fichier STM32CubeMX, fichier Nucleo64_Test1.ioc
	NRF24L01_SetRF(NRF24L01_DataRate_2M, NRF24L01_OutputPower_M18dBm);
	NRF24L01_SetMyAddress(MyAddress);
	NRF24L01_SetTxAddress(TxAddress);

	HAL_GPIO_WritePin(GPIOC, TEST_OUT_PIN_Pin, RESET);
	NRF24L01_Transmit(dataOut);				//WiFi send "System Start!" to host
	HAL_GPIO_WritePin(GPIOC, TEST_OUT_PIN_Pin, SET);
}

const char TEST_WRITE_BYTE_FS[] = "Writing %#x to address %#x\r\n";
const char TEST_WRITE_STR_FS[] = "Writing '%s' to address %#x\r\n";
void test_eeprom_write(){
	char test_byte = 0x55;
	char test_string[] = "Eep eep eep ROM";
	uint8_t page_addr = 0xA0;
	uint16_t byte_addr = 0x00;
	uint16_t string_addr = 0x01;
	char msg[100];

	memset(msg,0x00,strlen(msg));
	sprintf(msg, TEST_WRITE_BYTE_FS, test_byte, byte_addr);
	hf_print(msg);
	i2cWrite(page_addr, byte_addr, test_byte);
	//HAL_Delay(50);

	memset(msg,0x00,strlen(msg));
	sprintf(msg, TEST_WRITE_STR_FS, test_string, string_addr);
	hf_print(msg);
	i2cMemWriteSequence(page_addr, string_addr, test_string, strlen(test_string));
	//HAL_Delay(50);
}

const char TEST_READ_BYTE_FS[] = "Reading '%#x' from address %#x\r\n";
const char TEST_READ_STR_FS[] = "Reading '%s' from address %#x\r\n";
void test_eeprom_read(){
	char test_byte = 0x00;
	char test_string[32];
	uint8_t page_addr = 0xA0;
	uint16_t byte_addr = 0x00;
	uint16_t string_addr = 0x01;
	char msg[100];

	i2cRead(page_addr, byte_addr, 1, &test_byte);
	memset(msg,0x00,strlen(msg));
	sprintf(msg, TEST_READ_BYTE_FS, test_byte, byte_addr);
	hf_print(msg);

	memset(test_string,0x00,32);
	i2cRead(page_addr, string_addr, 15, test_string);
	memset(msg,0x00,strlen(msg));
	sprintf(msg, TEST_READ_STR_FS, test_string, string_addr);
	hf_print(msg);
}

void test_ram(){
	char dataOut[32] = "Dodge RAM.";

	dataOut[0] = 0x05;	//Read status register
	dataOut[1] = 0x00;
	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, dataOut, dataOut, 2, 100);
	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_SET);
	HAL_Delay(2);

	dataOut[0] = 0x01;	//Write status register
	dataOut[1] = 0x41;	//Set Sequential mode
	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, dataOut, dataOut, 2, 100);
	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_SET);
	HAL_Delay(2);

	dataOut[0] = 0x02;	//Write Instruction
	dataOut[1] = 0x00;  //addr low
	dataOut[2] = 0x00;  //addr high
	dataOut[3] = 0x02;
	dataOut[4] = 0x03;
	dataOut[5] = 0x04;
	dataOut[6] = 0x05;
	dataOut[7] = 0x06;
	dataOut[8] = 0x07;
	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, dataOut, dataOut, 10, 100);
	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_SET);
	HAL_Delay(50);

	dataOut[0] = 0x03;	//Read Instruction
	dataOut[1] = 0x00;
	dataOut[2] = 0x00;
	dataOut[3] = 0x00;
	dataOut[4] = 0x00;
	dataOut[5] = 0x00;
	dataOut[6] = 0x00;
	dataOut[7] = 0x00;
	dataOut[8] = 0x00;
	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, dataOut, dataOut, 15, 100);
	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_SET);
}

void test_led(){
	char dataOut[32];
}

