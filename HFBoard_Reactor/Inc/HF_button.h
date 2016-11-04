/*
 * HF_button.h
 *
 *  Created on: 2 nov. 2016
 *      Author: GrozoneDev
 */

#ifndef HF_BUTTON_H_
#define HF_BUTTON_H_

#include "HF_functions.h"
#include "HF_timer_service.h"

typedef union{
	uint8_t state;
	struct{
		uint8_t debug_mode_flag_check : 1;
		uint8_t debug_mode_flag_valid : 1;
		uint8_t debug_mode_flag_processed : 1;
	};
}HF_BUTTON_MODE;

HF_BUTTON_MODE hf_button_mode;

#endif /* HF_BUTTON_H_ */
