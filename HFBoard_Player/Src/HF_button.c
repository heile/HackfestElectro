/*
 * HF_button.c
 *
 *  Created on: 2 nov. 2016
 *      Author: GrozoneDev
 */

#include "HF_button.h"
#include "HF_flags.h"
#include "HF_leds.h"

extern LED_PATTERN_NAME LED_RUNNING_PATTERN;

void HfButtonInit(){
	hf_button_mode.state = 0;
}

void HfButtonProcess(){
	char flag_buffer[FLAG_LEN];

	if (hf_button_mode.debug_mode_flag_check==1){
		if (systemTimerServiceCheckEnd(HF_TMR_BUTTON)==0){
			hf_button_mode.debug_mode_flag_check = 0;
			hf_button_mode.debug_mode_flag_valid = 1;
		}
	}
	if (hf_button_mode.debug_mode_flag_valid){
		hf_button_mode.debug_mode_flag_valid = 0;
		hf_button_mode.debug_mode_flag_processed = 1;
		hf_print_all("HF Debug Mode Enabled\r\n");

		// Print a flag when the debug mode is found.
    	memset(flag_buffer, 0x00, FLAG_LEN);
    	get_flag(FLAG6,flag_buffer);
    	hf_print_all("Flag: ");
    	hf_print_all(flag_buffer);
    	hf_print_all("\r\n");

    	// Change the led pattern
    	LED_RUNNING_PATTERN = DOUBLE_LOOP;

	}
}
