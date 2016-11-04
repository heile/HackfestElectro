#include "main.h"
#include "stdio.h"
#include "nRF24L01.h"
#include "aes.h"
#include <HF_flags.h>
#include <HF_leds.h>
#include <HF_print.h>
#include <HF_shell.h>
#include "HF_I2C_Software.h"
#include "HF_IR_Transmitter.h"
#include "HF_IR_Receiver.h"
#include "HF_unit_tests.h"
#include "HF_button.h"

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

const uint8_t ENCRYPTION_KEY[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xaa, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xbb, 0x4f, 0x3c }; 	//16
const uint8_t ENCRYPTION_IV[] = { 0x00, 0x01, 0x02, 0x03, 0x03, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x01, 0x0f };		//16

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
		hf_print_all("\r\nRF Receive: ");
		hf_print_all(HF_rf_buffer.rx_buffer.buffer);
		hf_print_all("\r\n");
//			HAL_GPIO_TogglePin(GPIOC, TEST_OUT_PIN_Pin);
//			NRF24L01_Transmit(HF_rf_buffer.rx_buffer.buffer);		//WiFi send "System Start!" to host
//			HAL_GPIO_TogglePin(GPIOC, TEST_OUT_PIN_Pin);
	}

	//IR Receiver
	if (hf_ir_reciever.state ==  HF_IR_RECEIVER_STATE_DATA_READY){
		hf_ir_reciever.state =  HF_IR_RECEIVER_STATE_DATA_READ;
		hf_print_all("\r\IR Receive: ");
		printf("DATA: %#X, Address %#X, Command %#X\r\n",hf_ir_reciever.data, hf_ir_reciever.address,hf_ir_reciever.command);
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

// IR Receiver
// Button 1
// Button 2
void old_exti_callback(uint16_t GPIO_Pin){
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
			//printf("\r\n%#X\r\n",value);
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

void init_uart(){
	HAL_UART_Receive_IT(&huart1, &rx_fifo_hacker_uart_port, 1);
	HAL_UART_Receive_IT(&huart3, &rx_fifo_rs232, 1);
}

void test_wifi(){
	char dataOut[32] = "Love is in the air.";
	uint8_t MyAddress[] = { 'c', 'l', 'i', 'e', '1' };	/* My address */
	uint8_t TxAddress[] = { 's', 'e', 'r', 'v', '1' };	/* Receiver address */

	NRF24L01_Init(&hspi2, 1, 32);// Definition des pins sont dans le fichier STM32CubeMX, fichier Nucleo64_Test1.ioc
	NRF24L01_SetRF(NRF24L01_DataRate_2M, NRF24L01_OutputPower_M6dBm);
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
	char test_string[] = "Eep_eep_eep_ROM";
	uint8_t page_addr = 0xA0;
	uint16_t byte_addr = 0x00;
	uint16_t string_addr = 0x01;
	char msg[100];

	memset(msg,0x00,strlen(msg));
	sprintf(msg, TEST_WRITE_BYTE_FS, test_byte, byte_addr);
	hf_print_all(msg);
	i2cWrite(page_addr, byte_addr, test_byte);
	//HAL_Delay(50);

	memset(msg,0x00,strlen(msg));
	sprintf(msg, TEST_WRITE_STR_FS, test_string, string_addr);
	hf_print_all(msg);
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
	hf_print_all(msg);

	memset(test_string,0x00,32);
	i2cRead(page_addr, string_addr, 15, test_string);
	memset(msg,0x00,strlen(msg));
	sprintf(msg, TEST_READ_STR_FS, test_string, string_addr);
	hf_print_all(msg);
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


void encrypt_32byte_str(char* input, uint8_t* output){
	char in[32];
	memset(in, 0x00, 32);
	sprintf(in, input);
	AES128_CBC_encrypt_str(output, in, 32, ENCRYPTION_KEY, ENCRYPTION_IV);
}

void decrypt_32byte_str(uint8_t* input, char* output){
	char out[32];
	memset(out, 0x00, 32);
	AES128_CBC_decrypt_str(out, input, 32, ENCRYPTION_KEY, ENCRYPTION_IV);
	sprintf(output, out);
}

void print_32byte_str(char* input){
	char fmt_char[4];

	// Print Message
	hf_print_all("uint8_t out[] = { ");
	for(int i=0; i<strlen(input); i++){
	  sprintf(fmt_char, "%#x", input[i]);
	  hf_print_all(fmt_char);
	  if (i < strlen(input)-1){
		  hf_print_all(", ");
	  }

	  if ((i % 16) == 15){
		  hf_print_all("\r\n                  ");
	  }
	  memset(fmt_char, 0x00, 4);
	}
	hf_print_all("};\r\n");
}

void test_encrypt_message(char* input){
  uint8_t buffer[32];
  char out[32];
  memset(buffer, 0x00, 32);
  memset(out, 0x00, 32);
  encrypt_32byte_str(input,buffer);
  print_32byte_str(buffer);
  decrypt_32byte_str(buffer,out);
  hf_print_all("Decrypted string: ");
  hf_print_all(out);
  hf_print_all("\r\n");
}

void test_encrypt_cbc(void){
	/*
  uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };	//16
  uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };	//16
  uint8_t in[]  = { 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
                    0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
                    0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
                    0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10 };	//64
  uint8_t out[] = { 0x76, 0x49, 0xab, 0xac, 0x81, 0x19, 0xb2, 0x46, 0xce, 0xe9, 0x8e, 0x9b, 0x12, 0xe9, 0x19, 0x7d,
                    0x50, 0x86, 0xcb, 0x9b, 0x50, 0x72, 0x19, 0xee, 0x95, 0xdb, 0x11, 0x3a, 0x91, 0x76, 0x78, 0xb2,
                    0x73, 0xbe, 0xd6, 0xb8, 0xe3, 0xc1, 0x74, 0x3b, 0x71, 0x16, 0xe6, 0x9e, 0x22, 0x22, 0x95, 0x16,
                    0x3f, 0xf1, 0xca, 0xa1, 0x68, 0x1f, 0xac, 0x09, 0x12, 0x0e, 0xca, 0x30, 0x75, 0x86, 0xe1, 0xa7 };	//64
  uint8_t buffer[64];
  */

  uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xaa, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xbb, 0x4f, 0x3c };	//16
  uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x03, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x01, 0x0f };	//16
  char in[32];
  char fmt_char[4];
  char buffer[32];
  uint8_t out[] = { 0x57, 0xf2, 0xd5, 0x62, 0xee, 0x1d, 0x7e, 0x74, 0x65, 0x24, 0x18, 0xd3, 0xd1, 0xe5, 0x23, 0x28,
                    0xfd, 0xb4, 0x88, 0x54, 0xbf, 0xbc, 0x92, 0xbe, 0xf9, 0xff, 0x5f, 0x90, 0x2d, 0xa7, 0xc0, 0xed };

  memset(in, 0x00, 32);
  memset(buffer, 0x00, 32);

  sprintf(in, "Message to encrypt");

  hf_print_all("Plain text: ");
  hf_print_all(in);
  hf_print_all("\r\n");

  AES128_CBC_encrypt_str(buffer, in, 32, key, iv);

  print_32byte_str(buffer);


  if(0 == memcmp((char*) out, (char*) buffer, 32))
  {
	  hf_print_all("SUCCESS!\r\n");
  }
  else
  {
	  hf_print_all("FAILURE!\r\n");
  }
}

void test_decrypt_cbc(void)
{
  // Example "simulating" a smaller buffer...

  uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
  uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
  uint8_t in[]  = { 0x76, 0x49, 0xab, 0xac, 0x81, 0x19, 0xb2, 0x46, 0xce, 0xe9, 0x8e, 0x9b, 0x12, 0xe9, 0x19, 0x7d,
                    0x50, 0x86, 0xcb, 0x9b, 0x50, 0x72, 0x19, 0xee, 0x95, 0xdb, 0x11, 0x3a, 0x91, 0x76, 0x78, 0xb2,
                    0x73, 0xbe, 0xd6, 0xb8, 0xe3, 0xc1, 0x74, 0x3b, 0x71, 0x16, 0xe6, 0x9e, 0x22, 0x22, 0x95, 0x16,
                    0x3f, 0xf1, 0xca, 0xa1, 0x68, 0x1f, 0xac, 0x09, 0x12, 0x0e, 0xca, 0x30, 0x75, 0x86, 0xe1, 0xa7 };
  uint8_t out[] = { 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
                    0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
                    0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
                    0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10 };
  uint8_t buffer[64];

  AES128_CBC_decrypt_buffer(buffer+0, in+0,  16, key, iv);
  AES128_CBC_decrypt_buffer(buffer+16, in+16, 16, 0, 0);
  AES128_CBC_decrypt_buffer(buffer+32, in+32, 16, 0, 0);
  AES128_CBC_decrypt_buffer(buffer+48, in+48, 16, 0, 0);

  hf_print_all("CBC decrypt: ");

  if(0 == memcmp((char*) out, (char*) buffer, 64))
  {
	  hf_print_all("SUCCESS!\r\n");
  }
  else
  {
	  hf_print_all("FAILURE!\r\n");
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
