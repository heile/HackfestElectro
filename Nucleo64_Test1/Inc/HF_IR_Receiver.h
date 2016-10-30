#ifndef HF_IR_RECEIVER_H_
#define HF_IR_RECEIVER_H_

#include "HF_functions.h"


typedef enum{
	HF_IR_RECEIVER_STATE_OK,
	HF_IR_RECEIVER_STATE_PROCESSING,
	HF_IR_RECEIVER_STATE_ERROR_PACKAGE,
}HF_IR_RECEIVER_STATE;


typedef struct{
	union {
		uint32_t data;
		struct{
			uint8_t address;
			uint8_t address_inverse;
			uint8_t command;
			uint8_t command_inverse;
		};
	};
	union {
		uint8_t state;
	};
	struct{
		uint8_t lsb_during;
		uint8_t msb_during;
	}timer_stamp;
	volatile uint8_t timer_tick;	//NEC 56.25us	increment in timer 7
	uint8_t pulse_number;
	uint8_t bit_number_last;
}HF_IR_RECEIVER;

HF_IR_RECEIVER hf_ir_reciever;

#endif  /* HF_IR_RECEIVER_H_ */
