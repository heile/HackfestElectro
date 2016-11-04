/*
 * HF_IR_Transmitter.c
 *
 *  Created on: 31 oct. 2016
 *      Author: HD-L
 */

#include "HF_IR_Transmitter.h"

extern TIM_HandleTypeDef htim6;

void HfIrTransmitterSendCode(){
	switch (hf_ir_transmitter.state){
	case HF_IR_TRANSMITTER_STATE_READY:
		break;
	case HF_IR_TRANSMITTER_STATE_SNED:
		HAL_TIM_Base_Start_IT(&htim6);
		if (hf_ir_transmitter.type_code==HF_IR_TRANSMITTER_CODE_TYPE_NEC){
			hf_ir_transmitter.pulse_during = HF_IR_TRANSMITTER_NEC_CODE_HEADER_PULSE;
			hf_ir_transmitter.space_during = HF_IR_TRANSMITTER_NEC_CODE_HEADER_SPACE;
			hf_ir_transmitter.bit_mask = 0x01<<31;
			hf_ir_transmitter.state = HF_IR_TRANSMITTER_STATE_SEND_HEADER;
		}else{
			hf_ir_transmitter.state = HF_IR_TRANSMITTER_STATE_READY;
		}
		break;
	case HF_IR_TRANSMITTER_STATE_SEND_HEADER:
		if (hf_ir_transmitter.pulse_during>0){
			hf_ir_transmitter.bit_state = HF_IR_TRANSMITTER_BIT_STATE_PULSE;
			hf_ir_transmitter.pulse_during --;
		}else if (hf_ir_transmitter.space_during>0){
			hf_ir_transmitter.bit_state = HF_IR_TRANSMITTER_BIT_STATE_SPACE;
			hf_ir_transmitter.space_during --;
		}else{
			if ((hf_ir_transmitter.data & hf_ir_transmitter.bit_mask) == 0){
				hf_ir_transmitter.pulse_during = HF_IR_TRANSMITTER_NEC_CODE_DATA_0_PULSE;
				hf_ir_transmitter.space_during = HF_IR_TRANSMITTER_NEC_CODE_DATA_0_SPACE;
			}else{
				hf_ir_transmitter.pulse_during = HF_IR_TRANSMITTER_NEC_CODE_DATA_1_PULSE;
				hf_ir_transmitter.space_during = HF_IR_TRANSMITTER_NEC_CODE_DATA_1_SPACE;
			}
			hf_ir_transmitter.pulse_during --;
			hf_ir_transmitter.bit_state = HF_IR_TRANSMITTER_BIT_STATE_PULSE;
			hf_ir_transmitter.state = HF_IR_TRANSMITTER_STATE_SEND_NEC_DATA;
		}
		break;
	case HF_IR_TRANSMITTER_STATE_SEND_NEC_DATA:
		if (hf_ir_transmitter.pulse_during>0){
			hf_ir_transmitter.bit_state = HF_IR_TRANSMITTER_BIT_STATE_PULSE;
			hf_ir_transmitter.pulse_during --;
		}else if (hf_ir_transmitter.space_during>0){
			hf_ir_transmitter.bit_state = HF_IR_TRANSMITTER_BIT_STATE_SPACE;
			hf_ir_transmitter.space_during --;
		}else if (hf_ir_transmitter.bit_mask>0){
			hf_ir_transmitter.bit_mask>>=1;
			if ((hf_ir_transmitter.data & hf_ir_transmitter.bit_mask) == 0){
				hf_ir_transmitter.pulse_during = HF_IR_TRANSMITTER_NEC_CODE_DATA_0_PULSE;
				hf_ir_transmitter.space_during = HF_IR_TRANSMITTER_NEC_CODE_DATA_0_SPACE;
			}else{
				hf_ir_transmitter.pulse_during = HF_IR_TRANSMITTER_NEC_CODE_DATA_1_PULSE;
				hf_ir_transmitter.space_during = HF_IR_TRANSMITTER_NEC_CODE_DATA_1_SPACE;
			}
			hf_ir_transmitter.pulse_during --;
			hf_ir_transmitter.bit_state = HF_IR_TRANSMITTER_BIT_STATE_PULSE;
		}else{
			hf_ir_transmitter.pulse_during = HF_IR_TRANSMITTER_NEC_CODE_END_PULSE;
			hf_ir_transmitter.space_during = HF_IR_TRANSMITTER_NEC_CODE_END_SPACE;
			hf_ir_transmitter.pulse_during --;
			hf_ir_transmitter.bit_state = HF_IR_TRANSMITTER_BIT_STATE_PULSE;
			hf_ir_transmitter.state = HF_IR_TRANSMITTER_STATE_SEND_NEC_END;
		}
		break;
	case HF_IR_TRANSMITTER_STATE_SEND_NEC_END:
		if (hf_ir_transmitter.pulse_during>0){
			hf_ir_transmitter.bit_state = HF_IR_TRANSMITTER_BIT_STATE_PULSE;
			hf_ir_transmitter.pulse_during --;
		}else if (hf_ir_transmitter.space_during>0){
			hf_ir_transmitter.bit_state = HF_IR_TRANSMITTER_BIT_STATE_SPACE;
			hf_ir_transmitter.space_during --;
		}else{
			HAL_TIM_Base_Stop_IT(&htim6);
			hf_ir_transmitter.state = HF_IR_TRANSMITTER_STATE_SEND_DATA_FINISH;
		}
		break;
	case HF_IR_TRANSMITTER_STATE_SEND_DATA_FINISH:
		hf_ir_transmitter.state = HF_IR_TRANSMITTER_STATE_READY;
		break;
	}
}


void HfIrTransmitterSetData(uint8_t address, uint8_t command, HF_IR_TRANSMITTER_CODE_TYPE type_code){
	switch (type_code){
	case HF_IR_TRANSMITTER_CODE_TYPE_NEC:
		hf_ir_transmitter.nec_code.address = address;
		hf_ir_transmitter.nec_code.address_inverse = ~address;
		hf_ir_transmitter.nec_code.command = command;
		hf_ir_transmitter.nec_code.command_inverse = ~command;
		hf_ir_transmitter.type_code = type_code;
	}
}

void HfIrTransmitterSetData32(uint32_t data, HF_IR_TRANSMITTER_CODE_TYPE type_code){
	switch (type_code){
	case HF_IR_TRANSMITTER_CODE_TYPE_NEC:
		hf_ir_transmitter.nec_code.address = (data >> 24) & 0xFF;
		hf_ir_transmitter.nec_code.address_inverse = (data >> 16) & 0xFF;
		hf_ir_transmitter.nec_code.command = (data >> 8) & 0xFF;
		hf_ir_transmitter.nec_code.command_inverse = data & 0xFF;
		hf_ir_transmitter.type_code = type_code;
	}
}
