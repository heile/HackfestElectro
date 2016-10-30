/*
 * HF_IR_Receiver.c
 *
 *  Created on: Oct 30, 2016
 *      Author: HD-S855
 */
#include <stdint.h>
#include "HF_IR_Receiver.h"

void HfIrReceiverEdgeInterruption(uint8_t edge_tpe){
	static uint8_t timer_stamp_falling=0;
	static uint8_t timer_stamp_rising=0;
	switch (edge_tpe){
	case HF_INTERRUPTION_EDGE_FALLING:
		timer_stamp_falling = hf_ir_reciever.timer_tick;
		if (hf_ir_reciever.pulse_number!=0){
			hf_ir_reciever.timer_stamp.msb_during = timer_stamp_falling - timer_stamp_rising;
//			if ()
//			hf_ir_reciever.data |=
			hf_ir_reciever.bit_number_last--;
		}else{
			hf_ir_reciever.bit_number_last = 31;
		}
		break;
	case HF_INTERRUPTION_EDGE_RSING:
		timer_stamp_rising = hf_ir_reciever.timer_tick;
		hf_ir_reciever.timer_stamp.lsb_during = timer_stamp_rising- timer_stamp_falling;
		break;
	}
	hf_ir_reciever.pulse_number++;
}
