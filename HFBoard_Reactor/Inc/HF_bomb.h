/*
 * HF_bomb.h
 *
 *  Created on: Nov 4, 2016
 *      Author: HD-S855
 */

#ifndef HF_BOMB_H_
#define HF_BOMB_H_

#include "stdint.h"
#include "stdio.h"

enum {
	BOMB_FLAG_STATE_WATTING,
	BOMB_FLAG_STATE_VALID,
	BOMB_FLAG_STATE_PROCESSING_WITH_ALARM,
	BOMB_FLAG_STATE_PROCESSING_NON_ALARM,
	BOMB_FLAG_STATE_FIN,
}BOMB_FLAG_STATE;

struct{
	uint8_t state;
}bomb;


#endif /* HF_BOMB_H_ */
