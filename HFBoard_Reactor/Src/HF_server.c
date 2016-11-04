#include "main.h"
#include "stdio.h"
#include <stdbool.h>
#include "nRF24L01.h"
#include "aes.h"
#include <HF_flags.h>
#include <HF_leds.h>
#include <HF_print.h>
#include <HF_server.h>
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
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

// IR Variables.
// Solution: 1, 2, 3, Power
#define IR_PWR_VALUES_COUNT 3
uint32_t IR_PWR_VALUES[] = {0xFFA25D, 0xFFA25D, 0xFFA25D};
uint32_t IR_ACTIVATION_CODE = 0xdeadc0fe;
uint8_t IR_CHAL_INDEX = 0;
uint32_t IR_CHAL_USER_INPUT[IR_PWR_VALUES_COUNT];


void hf_print_all_dbg(char* pBuffer){
	hf_print_all(pBuffer);
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
		ir_callback(hf_ir_reciever.data);
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
			HAL_GPIO_WritePin(GPIOC, R3_Liquid_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, R2_Sirene_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, R1_Light_Pin, GPIO_PIN_RESET);
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
			HAL_GPIO_WritePin(GPIOC, R3_Liquid_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, R2_Sirene_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, R1_Light_Pin, GPIO_PIN_SET);
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

void init_wifi(){
	uint8_t MyAddress[] = { 'c', 'l', 'i', 'e', '1' };	/* Receiver address */
	uint8_t TxAddress[] = { 's', 'e', 'r', 'v', '1' };	/* My address */

	NRF24L01_Init(&hspi2, 30, 32);// Definition des pins sont dans le fichier STM32CubeMX, fichier Nucleo64_Test1.ioc
	NRF24L01_SetRF(NRF24L01_DataRate_250k, NRF24L01_OutputPower_0dBm);
	NRF24L01_SetMyAddress(MyAddress);
	NRF24L01_SetTxAddress(TxAddress);
}

// IR Challenge
void ir_updatePlayerInput(uint32_t input){
  uint8_t i;
  char fmt_char[32];
  memset(fmt_char, 0x00, 32);

  IR_CHAL_USER_INPUT[IR_CHAL_INDEX] = input;
  IR_CHAL_INDEX = (IR_CHAL_INDEX + 1) % IR_PWR_VALUES_COUNT;

  hf_print_all_dbg("Updating IR Challenge. index=");
  hf_print_all_dbg(IR_CHAL_INDEX);
  hf_print_all_dbg("\r\n");
  hf_print_all_dbg("IR Challenge values: ");
  for (i = 0; i < IR_PWR_VALUES_COUNT; i++){
	  sprintf(fmt_char,"a[%i]=%#x ", i, IR_CHAL_USER_INPUT[i]);
	  hf_print_all_dbg(fmt_char);
	  memset(fmt_char, 0x00, 32);
  }
  hf_print_all_dbg("\r\n");
}

bool ir_is_pwr_Valid(void){
  uint8_t i;
  for (i = 0; i < IR_PWR_VALUES_COUNT; i++){
     if (IR_CHAL_USER_INPUT[0] == IR_PWR_VALUES[i] &&
         IR_CHAL_USER_INPUT[1] == IR_PWR_VALUES[(i+1)%IR_PWR_VALUES_COUNT] &&
         IR_CHAL_USER_INPUT[2] == IR_PWR_VALUES[(i+2)%IR_PWR_VALUES_COUNT]){
          	  return 1;
        }
  }

  return 0;
}

bool ir_is_activation_Valid(void){
  uint8_t i;
  for (i = 0; i < IR_PWR_VALUES_COUNT; i++){
     if (IR_CHAL_USER_INPUT[i] == IR_ACTIVATION_CODE){
          	  return 1;
        }
  }

  return 0;
}

void ir_send_activation_code(void){
	if (hf_ir_transmitter.state == HF_IR_TRANSMITTER_STATE_READY){
		HfIrTransmitterSetData32(IR_ACTIVATION_CODE,HF_IR_TRANSMITTER_CODE_TYPE_NEC);
		hf_ir_transmitter.state = HF_IR_TRANSMITTER_STATE_SNED;
	}
	hf_print_all("Message sent..\r\n");
}

void ir_send_flag(void){
	char flag_buffer[FLAG_LEN];
	char msg_buffer[64];
	uint8_t sub_buffer[4];

	memset(flag_buffer,0x00,FLAG_LEN);
	memset(msg_buffer,0x00,64);

	get_flag(FLAG10,flag_buffer);
	sprintf(msg_buffer, "Flag:   %s", flag_buffer);

	for(int i=0; i<strlen(msg_buffer); i=i+4){
		sub_buffer[0] = msg_buffer[i];
		sub_buffer[1] = msg_buffer[i+1];
		sub_buffer[2] = msg_buffer[i+2];
		sub_buffer[3] = msg_buffer[i+3];

		printf("Transmiter state: %#x\r\n",hf_ir_transmitter.state);
		HAL_Delay(10);
		// Send IR
		if(hf_ir_transmitter.state == HF_IR_TRANSMITTER_STATE_READY){
			HfIrTransmitterSetData32(sub_buffer,HF_IR_TRANSMITTER_CODE_TYPE_NEC);
			hf_ir_transmitter.state = HF_IR_TRANSMITTER_STATE_SNED;
		} else {
			while (hf_ir_transmitter.state != HF_IR_TRANSMITTER_STATE_READY){
			}

			HfIrTransmitterSetData32(sub_buffer,HF_IR_TRANSMITTER_CODE_TYPE_NEC);
			hf_ir_transmitter.state = HF_IR_TRANSMITTER_STATE_SNED;
		}

		HAL_Delay(100);
	}

	hf_print_all("Flag sent..\r\n");
}

void ir_callback(uint32_t input){
	ir_updatePlayerInput(input);

    if (ir_is_pwr_Valid()) {
    	ir_send_activation_code();
    	IR_CHAL_USER_INPUT[0] = 0x00;
    	IR_CHAL_USER_INPUT[1] = 0x01;
    	IR_CHAL_USER_INPUT[2] = 0x02;
    }

    if(ir_is_activation_Valid()){
    	ir_send_flag();
    }
}

/*
 * Keeping this just for the values.
 *
void translateIR(char* buf, int value) {

  switch(value) {

  case 0xFFA25D:
    strcpy(buf,"POWER");
    break;

  case 0xFF629D:
    strcpy(buf,"MODE");
    break;

  case 0xFFE21D:
    strcpy(buf,"MUTE");
    break;

  case 0xFF22DD:
    strcpy(buf,"PLAY/PAUSE");
    break;

  case 0xFF02FD:
    strcpy(buf,"PREV");
    break;

  case 0xFFC23D:
    strcpy(buf,"NEXT");
    break;

  case 0xFFE01F:
    strcpy(buf,"EQ");
    break;

  case 0xFFA857:
    strcpy(buf,"VOL-");
    break;

  case 0xFF906F:
    strcpy(buf,"VOL+");
    break;

  case 0xFF6897:
    strcpy(buf,"0");
    break;

  case 0xFF9867:
    strcpy(buf,"100+");
    break;

  case 0xFFB04F:
    strcpy(buf,"200+");
    break;

  case 0xFF30CF:
    strcpy(buf,"1");
    break;

  case 0xFF18E7:
    strcpy(buf,"2");
    break;

  case 0xFF7A85:
    strcpy(buf,"3");
    break;

  case 0xFF10EF:
    strcpy(buf,"4");
    break;

  case 0xFF38C7:
    strcpy(buf,"5");
    break;

  case 0xFF5AA5:
    strcpy(buf,"6");
    break;

  case 0xFF42BD:
    strcpy(buf,"7");
    break;

  case 0xFF4AB5:
    strcpy(buf,"8");
    break;

  case 0xFF52AD:
    strcpy(buf,"9");
    break;

  default:
    strcpy(buf,"other button");
  }
}*/



