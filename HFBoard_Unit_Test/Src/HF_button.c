/*
 * HF_button.c
 *
 *  Created on: 2 nov. 2016
 *      Author: GrozoneDev
 */

#include "HF_button.h"

void HfButtonInit(){
	hf_button_mode.state = 0;
}

void HfButtonProcess(){
	if (hf_button_mode.debug_mode_flag_check==1){
		if (systemTimerServiceCheckEnd(HF_TMR_BUTTON)==0){
			hf_button_mode.debug_mode_flag_check = 0;
			hf_button_mode.debug_mode_flag_valid = 1;
		}
	}
	if (hf_button_mode.debug_mode_flag_valid){
		hf_button_mode.debug_mode_flag_valid = 0;
		hf_button_mode.debug_mode_flag_processed = 1;
		hf_print_all("HF Debug Mode Valid\r\n");
	}
}
