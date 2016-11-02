/*
 * HF_IR_Receiver.c
 *
 *  Created on: Oct 30, 2016
 *      Author: HD-S855
 */
#include <stdint.h>
#include "HF_IR_Receiver.h"

void HF_IrReceiverInitialize(){
	hf_ir_reciever.state = HF_IR_RECEIVER_STATE_GET_HEADER;
}

void HfIrReceiverEdgeInterruption(uint8_t edge_tpe) {
	static uint8_t timer_stamp_falling = 0;
	static uint8_t timer_stamp_rising = 0;
	switch (edge_tpe) {
	case HF_INTERRUPTION_EDGE_FALLING:
		timer_stamp_falling = hf_ir_reciever.timer_tick;
			hf_ir_reciever.timer_stamp.msb_during = timer_stamp_falling - timer_stamp_rising;
			switch (hf_ir_reciever.state) {
			case HF_IR_RECEIVER_STATE_GET_HEADER:
					if ((hf_ir_reciever.timer_stamp.lsb_during - 155) > 0 && (hf_ir_reciever.timer_stamp.lsb_during - 155)< 35) {	// 9ms= 160 * 56.25us NEC 1e header
						if ((hf_ir_reciever.timer_stamp.msb_during - 75) > 0 && (hf_ir_reciever.timer_stamp.msb_during - 75)< 18) {	//4.5ms = 80 * 56.25  NEC 2e header
							hf_ir_reciever.bit_number_last = 31;
							hf_ir_reciever.data = 0;
							hf_ir_reciever.type_code = HF_IR_RECEIVER_CODE_TYPE_NEC;
							systemTimerServiceSetTimer(HF_TMR_IR_RECEIVER,HF_TIMER_MILLISECOND,60);
							hf_ir_reciever.state =HF_IR_RECEIVER_STATE_GET_NEC_DATA;
						} else if ((hf_ir_reciever.timer_stamp.msb_during - 35) > 0 && (hf_ir_reciever.timer_stamp.msb_during - 35)< 15) {	//2.25ms = 40 * 56.25  NEC Repeat header
							hf_ir_reciever.bit_number_last = 0;
							hf_ir_reciever.type_code = HF_IR_RECEIVER_CODE_TYPE_NEC;
							systemTimerServiceSetTimer(HF_TMR_IR_RECEIVER,HF_TIMER_MILLISECOND,5);
							hf_ir_reciever.state =HF_IR_RECEIVER_STATE_GET_REPEAT;
						}
					} else {

					}
				break;
			case HF_IR_RECEIVER_STATE_GET_NEC_DATA:
				if ((hf_ir_reciever.timer_stamp.msb_during- hf_ir_reciever.timer_stamp.lsb_during)>7){
					hf_ir_reciever.data |= (1<<hf_ir_reciever.bit_number_last);
				}
				if (hf_ir_reciever.bit_number_last>0){
					hf_ir_reciever.bit_number_last--;
				}else{
					hf_ir_reciever.state = HF_IR_RECEIVER_STATE_DATA_READY;
				}
				break;
			case HF_IR_RECEIVER_STATE_GET_REPEAT:
				hf_ir_reciever.state = HF_IR_RECEIVER_STATE_REPEAT_READY;
				break;
			case HF_IR_RECEIVER_STATE_DATA_READY:

				break;
			case HF_IR_RECEIVER_STATE_REPEAT_READY:

				break;
			case HF_IR_RECEIVER_STATE_DATA_READ:
				hf_ir_reciever.state = HF_IR_RECEIVER_STATE_GET_HEADER;
				break;
		}
		break;
	case HF_INTERRUPTION_EDGE_RSING:
		timer_stamp_rising = hf_ir_reciever.timer_tick;
		hf_ir_reciever.timer_stamp.lsb_during = timer_stamp_rising - timer_stamp_falling;
		if (systemTimerServiceCheckEnd(HF_TMR_IR_RECEIVER)==0){
			hf_ir_reciever.state = HF_IR_RECEIVER_STATE_GET_HEADER;
		}
		break;
	}
}
