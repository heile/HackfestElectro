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

char VERSION_STRING[] = "HF Board v0.01 by Lei and Martin";

char dataOut[32];
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
}
void HFShellCommand(char * command) {
    int index = 0;

    //printf("\n\rDebug Command message: %s\n\r\n\r", debug_command_buffer);
    HFsetArg(command);

    if (strcmp(HF_argv[0],"") != 0){
		hf_print("\r\nDebug Command: ");
		for (index = 0; index < HF_argc; index++) {
			hf_print(" ");
			HAL_Delay(2);
			hf_print(HF_argv[index]);
			HAL_Delay(2);
		}
		hf_print("\r\n");
		HAL_Delay(2);

		hf_print("Triggering process shell command.");
		process_shell_command(HF_argv, HF_argc);
		HAL_Delay(2);
    }

    hf_print("hfboard>");
	HAL_Delay(2);
}

void process_shell_command(char** argv, int argc){
    if (strcmp(argv[0], "help") == 0) {
    	hf_print("This is a help message. Commands are:\r\n");
    	hf_print("  test: Test the board devices. Supported arguments are: all, usb, rs232, hacker, wifi, eeprom, ram, led. \r\n");
    	hf_print("  version: Print version. \r\n");
    } else if (strcmp(argv[0], "test") == 0) {
    	if (argc > 1){
    		if (strcmp(argv[1], "all") == 0) {

    		} else if (strcmp(argv[1], "all") == 0) {
    			hf_print("Testing all the things!\r\n");
    			hf_print_usb("Test USB!\r\n");
    			hf_print_rs232("Test RS232!\r\n");
    			hf_print_rs232("Test Hacker!\r\n");
    		} else if (strcmp(argv[1], "usb") == 0) {
    			hf_print_usb("Test USB!\r\n");
    		} else if (strcmp(argv[1], "rs232") == 0) {
    			hf_print_rs232("Test RS232!\r\n");
    		} else if (strcmp(argv[1], "hacker") == 0) {
    			hf_print_rs232("Test Hacker!\r\n");
    		} else if (strcmp(argv[1], "wifi") == 0) {

    		} else if (strcmp(argv[1], "eeprom") == 0) {

    		} else if (strcmp(argv[1], "ram") == 0) {

    		} else if (strcmp(argv[1], "led") == 0) {

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
    } else if (strcmp(argv[0], "version") == 0) {
    	hf_print(VERSION_STRING);
    }
}

void hf_print(char* pBuffer){
	hf_print_usb(pBuffer);
	hf_print_rs232(pBuffer);
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


void uart_init(){
	HAL_UART_Receive_IT(&huart3, &rx_fifo_rs232, 1);
}
// RS232 Receiver
// UART Receiver
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

void test_eeprom(){
	i2cWrite(0xA0, 0x00, 0x55);
	HAL_Delay(50);
	i2cMemWriteSequence(0xA0, 0x01, dataOut, 15);
	HAL_Delay(50);
	i2cRead(0xA0, 0x00, 20, dataOut);
}

void test_ram(SPI_HandleTypeDef* hspi){
	dataOut[0] = 0x05;	//Read status register
	dataOut[1] = 0x00;
	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi, dataOut, dataOut, 2, 100);
	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_SET);
	HAL_Delay(2);
	dataOut[0] = 0x01;	//Write status register
	dataOut[1] = 0x41;	//Set Sequential mode
	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi, dataOut, dataOut, 2, 100);
	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_SET);
	HAL_Delay(2);
	dataOut[0] = 0x02;	//Write Instruction
	dataOut[1] = 0x00;
	dataOut[2] = 0x00;
	dataOut[3] = 0x02;
	dataOut[4] = 0x03;
	dataOut[5] = 0x04;
	dataOut[6] = 0x05;
	dataOut[7] = 0x06;
	dataOut[8] = 0x07;
	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(hspi, dataOut, dataOut, 10, 100);
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
	HAL_SPI_TransmitReceive(hspi, dataOut, dataOut, 15, 100);
	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_SET);
}

void test_wifi(SPI_HandleTypeDef* hspi){
	uint8_t MyAddress[] = { 's', 'e', 'r', 'v', '1' };	/* My address */
	uint8_t TxAddress[] = { 'c', 'l', 'i', 'e', '1' };	/* Receiver address */

	NRF24L01_Init(hspi, 1, 32);// Definition des pins sont dans le fichier STM32CubeMX, fichier Nucleo64_Test1.ioc
	NRF24L01_SetRF(NRF24L01_DataRate_2M, NRF24L01_OutputPower_M18dBm);
	NRF24L01_SetMyAddress(MyAddress);
	NRF24L01_SetTxAddress(TxAddress);

	HAL_GPIO_TogglePin(GPIOC, TEST_OUT_PIN_Pin);
	NRF24L01_Transmit(dataOut);				//WiFi send "System Start!" to host
	HAL_GPIO_TogglePin(GPIOC, TEST_OUT_PIN_Pin);
}


