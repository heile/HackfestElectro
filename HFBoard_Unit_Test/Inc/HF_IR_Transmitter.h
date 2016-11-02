/*
 * HF_IR_Transmitter.h
 *
 *  Created on: 31 oct. 2016
 *      Author: HD-L
 */

#ifndef HF_IR_TRANSMITTER_H_
#define HF_IR_TRANSMITTER_H_

#include <stdint.h>
#include "stm32f0xx_hal.h"

#define HF_IR_TRANSMITTER_NEC_CODE_HEADER_PULSE 160	// 9ms= 160 * 56.25us NEC 1e header
#define HF_IR_TRANSMITTER_NEC_CODE_HEADER_SPACE 80	// //4.5ms = 80 * 56.25  NEC 2e header
#define HF_IR_TRANSMITTER_NEC_CODE_DATA_1_PULSE 10	// //0.5625ms = 10 * 56.25  NEC logic 1 pulse
#define HF_IR_TRANSMITTER_NEC_CODE_DATA_1_SPACE 30	// //1.6875ms = 30 * 56.25  NEC logic 1 space
#define HF_IR_TRANSMITTER_NEC_CODE_DATA_0_PULSE 10	// //0.5625ms = 10 * 56.25  NEC logic 0 pulse
#define HF_IR_TRANSMITTER_NEC_CODE_DATA_0_SPACE 10	// //0.5625ms = 10 * 56.25  NEC logic 0space
#define HF_IR_TRANSMITTER_NEC_CODE_END_PULSE 10	// //0.5625ms = 10 * 56.25  NEC logic 0space
#define HF_IR_TRANSMITTER_NEC_CODE_END_SPACE 1	// //0.5625ms = 10 * 56.25  NEC logic 0space

typedef enum{
	HF_IR_TRANSMITTER_STATE_READY,
	HF_IR_TRANSMITTER_STATE_SNED,
	HF_IR_TRANSMITTER_STATE_SEND_HEADER,
	HF_IR_TRANSMITTER_STATE_SEND_NEC_DATA,
	HF_IR_TRANSMITTER_STATE_SEND_REPEAT,
	HF_IR_TRANSMITTER_STATE_SEND_NEC_END,
	HF_IR_TRANSMITTER_STATE_SEND_DATA_FINISH,
	HF_IR_TRANSMITTER_STATE_SEND_REPEAT_FINISH,
	HF_IR_TRANSMITTER_STATE_ERROR_PACKAGE,
}HF_IR_TRANSMITTER_STATE;

typedef enum{
	HF_IR_TRANSMITTER_CODE_TYPE_NEC,
}HF_IR_TRANSMITTER_CODE_TYPE;

typedef enum{
	HF_IR_TRANSMITTER_BIT_STATE_PULSE,
	HF_IR_TRANSMITTER_BIT_STATE_SPACE,
}HF_IR_TRANSMITTER_BIT_STATE;

typedef struct{
	union {
		uint32_t data;
		struct{
			uint8_t command_inverse;
			uint8_t command;
			uint8_t address_inverse;
			uint8_t address;
		}nec_code;
	};
	union {
		uint8_t state;
	};
	uint8_t bit_state;
	struct{
		uint8_t pulse_during;
		uint8_t space_during;
	};
	volatile uint8_t timer_tick;	//NEC 56.25us	increment in timer 7
	uint8_t type_code;
	uint32_t bit_mask;
}HF_IR_TRANSMITTER;

HF_IR_TRANSMITTER hf_ir_transmitter;


void HfIrTransmitterSendCode();

void HfIrTransmitterSetData(uint8_t address, uint8_t command, HF_IR_TRANSMITTER_CODE_TYPE type_code);

#endif /* HF_IR_TRANSMITTER_H_ */
