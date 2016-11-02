#include "main.h"
#include "stdio.h"
#include <stdbool.h>
#include "nRF24L01.h"
#include "aes.h"
#include <HF_flags.h>
#include <HF_leds.h>
#include <HF_player.h>
#include <HF_print.h>
#include <HF_shell.h>
#include "HF_IR_Transmitter.h"
#include "HF_IR_Receiver.h"
#include "HF_I2C_Software.h"
#include "HF_button.h"
#include "HF_eeprom_ram.h"

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

char fifo_command;

char rx_fifo_hacker_uart_port;
char rx_fifo_rs232;

uint8_t counter_ir;

extern SHELL_COMMAND_CONSOLE_TYPE;


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


bool debug_mode_enabled(){
	if (hf_button_mode.debug_mode_flag_processed == 1){
		return 1;
	}
	return 0;
}

void hf_print_all_dbg(char* pBuffer){
	if(debug_mode_enabled()){
		hf_print_all(pBuffer);
	}
}

// USB + RS232 + Hacker
// WIFI
void handle_incoming_message(){
	char buf[128];

	memset(buf,0x00,128);

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
		hf_print_all_dbg("\r\nRF Receive: ");
		hf_print_all_dbg(HF_rf_buffer.rx_buffer.buffer);
		hf_print_all_dbg("\r\n");
//			HAL_GPIO_TogglePin(GPIOC, TEST_OUT_PIN_Pin);
//			NRF24L01_Transmit(HF_rf_buffer.rx_buffer.buffer);		//WiFi send "System Start!" to host
//			HAL_GPIO_TogglePin(GPIOC, TEST_OUT_PIN_Pin);
	}

	//IR Receiver
	if (hf_ir_reciever.state ==  HF_IR_RECEIVER_STATE_DATA_READY){
		hf_ir_reciever.state =  HF_IR_RECEIVER_STATE_DATA_READ;
		sprintf(buf, "IR Receive: %#X, Address %#X, Command %#X\r\n",hf_ir_reciever.data, hf_ir_reciever.address,hf_ir_reciever.command);
		hf_print_all_dbg(buf);
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

// IR Receiver
// Button 1
// Button 2
void exti_callback(uint16_t GPIO_Pin){
	if (GPIO_Pin == GPIO_PIN_0) {			//Button Fn1 interruption
		if (HAL_GPIO_ReadPin(Button_Fn1_GPIO_Port,Button_Fn1_Pin)==GPIO_PIN_SET){
			if (HAL_GPIO_ReadPin(Button_Fn2_GPIO_Port,Button_Fn2_Pin)==GPIO_PIN_SET){
				systemTimerServiceSetTimer(HF_TMR_BUTTON, HF_TIMER_MILLISECOND, 3000);
				hf_button_mode.debug_mode_flag_check = 1;
			}
			hf_print_all("Button 1 pressed\r\n");
		}else{
			hf_print_all("Button 1 released\r\n");
		}
	} else if (GPIO_Pin == GPIO_PIN_1) {	//Button Fn2 interruption
		if (HAL_GPIO_ReadPin(Button_Fn2_GPIO_Port,Button_Fn2_Pin)==GPIO_PIN_SET){
			if (HAL_GPIO_ReadPin(Button_Fn1_GPIO_Port,Button_Fn1_Pin)==GPIO_PIN_SET){
				systemTimerServiceSetTimer(HF_TMR_BUTTON, HF_TIMER_MILLISECOND, 3000);
				hf_button_mode.debug_mode_flag_check = 1;
			}
			hf_print_all("Button 2 pressed\r\n");
			run_next_led();
		}else{
			hf_print_all("Button 2 released\r\n");
		}
	} else if (GPIO_Pin == GPIO_PIN_2) {	//IR receiver interruption
		if (HAL_GPIO_ReadPin(IR_REC_GPIO_Port,IR_REC_Pin)==RESET){
			HfIrReceiverEdgeInterruption(HF_INTERRUPTION_EDGE_FALLING);
		}else{
			HfIrReceiverEdgeInterruption(HF_INTERRUPTION_EDGE_RSING);
		}
	}
}

// timer 6 -> IR Transmitter
// timer 7 -> IR Transmitter, IR receiver
// timer 16 -> base du temps millisecond
// timer 17 -> Leds
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;
void timer_callback(TIM_HandleTypeDef *htim){
	if (htim == &htim17) {
		/*
			static char sens = 0;
			static uint32_t value = 0;
			uint16_t min_value = 0;
			uint16_t max_value = 800;

			if (sens == 0) {
				if (value < max_value) {
					value++;
				} else {
					sens = 1;
				}
			} else {
				if (value > min_value) {
					value--;
				} else {
					sens = 0;
				}
			}
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, value);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, value);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, value);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, value);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, value);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, value);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, value);
		*/
	}else if (htim == &htim7){
		hf_ir_reciever.timer_tick++;
		HfIrTransmitterSendCode();
	}else if (htim == &htim6){
		if (hf_ir_transmitter.state!=HF_IR_TRANSMITTER_STATE_READY){
			if (hf_ir_transmitter.bit_state==HF_IR_TRANSMITTER_BIT_STATE_PULSE){
				HAL_GPIO_TogglePin(GPIOB, IR_OUT_Pin);
			}
			else{
				HAL_GPIO_WritePin(GPIOB, IR_OUT_Pin,RESET);
			}
		}
	}
}

void init_uart(){
	HAL_UART_Receive_IT(&huart1, &rx_fifo_hacker_uart_port, 1);
	HAL_UART_Receive_IT(&huart3, &rx_fifo_rs232, 1);
}

void init_flag_on_rom(){
	int i;
	char keyword[] = "Flag:           ";
	char buf[16];
	char flag_buffer[FLAG_LEN];
	char flag_part1[17];
	char flag_part2[17];
	MEM_ADDR flag4_eeprom_addr = {.page=0xA0, .addr=0x00};

	// Check if the flag was written
	eeprom_read_str(&flag4_eeprom_addr, buf, strlen(keyword));

	// If not exist, write it and print message
	//if(memcmp(keyword,buf,5) != 0){
	if(1){
		//Print
		hf_print_all("The flag4 does not exist. Writing it to EEPROM.\r\n");

		//Decrypt the flag
		get_flag(FLAG4,flag_buffer);

		//Write the keyword
		eeprom_write_str(&flag4_eeprom_addr, keyword);

		// Generate part 1 and 2
		for (i=0; i<16; i++){
			flag_part1[i] = flag_buffer[i];
			flag_part2[i] = flag_buffer[i+16];
		}
		flag_part1[16] = 0x00;
		flag_part2[16] = 0x00;

		//Write the flag
		flag4_eeprom_addr.page += 1;
		eeprom_write_str(&flag4_eeprom_addr, flag_part1);

		flag4_eeprom_addr.page += 1;
		eeprom_write_str(&flag4_eeprom_addr, flag_part2);
	}
}

void init_wifi(){
	uint8_t MyAddress[] = { 's', 'e', 'r', 'v', '1' };	/* My address */
	uint8_t TxAddress[] = { 'c', 'l', 'i', 'e', '1' };	/* Receiver address */


	NRF24L01_Init(&hspi2, 1, 32);// Definition des pins sont dans le fichier STM32CubeMX, fichier Nucleo64_Test1.ioc
	NRF24L01_SetRF(NRF24L01_DataRate_2M, NRF24L01_OutputPower_M18dBm);
	NRF24L01_SetMyAddress(MyAddress);
	NRF24L01_SetTxAddress(TxAddress);
}

// NOTE: THIS FLAG IS VERY SENSITIVE. BE CAREFUL WHEN UPDATING THIS FUNCTION.
void send_wifi_flag(){
	char msg_buffer[FLAG_LEN+6];
	char flag_buffer[FLAG_LEN];

	memset(msg_buffer, 0x00, FLAG_LEN+6);
	memset(flag_buffer, 0x00, FLAG_LEN);

	get_flag(FLAG2,flag_buffer);
	sprintf(msg_buffer,"Flag: %s",flag_buffer);

	hf_print_all("Sending something in the air.\r\n");
	HAL_GPIO_WritePin(GPIOC, TEST_OUT_PIN_Pin, RESET);
	NRF24L01_Transmit(msg_buffer);
	HAL_GPIO_WritePin(GPIOC, TEST_OUT_PIN_Pin, SET);
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



