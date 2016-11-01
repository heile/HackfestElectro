/*
 * HF_timer_service.h
 *
 *  Created on: 22 oct. 2016
 *      Author: HD-L
 */

#ifndef HF_TIMER_SERVICE_H_
#define HF_TIMER_SERVICE_H_

#include "stdint.h"

typedef enum {
	HF_TMR_DELAY, // Timer delais d'attente
	HF_led_timer,
	HF_msg_timer,
	HF_NUMBER_TIMER
} HF_SYSTEM_TIMER_SERVICE_ID;

typedef enum {
	HF_TIMER_MILLISECOND,
	HF_TIMER_MILLISECOND_AUTO_RESET,
	HF_TIMER_SECOND,
	HF_TIMER_SECOND_AUTO_RESET,
	HF_TIMER_MINUTE,
	HF_TIMER_MINUTE_AUTO_RESET
} HF_SYSTEM_TIMER_SERVICE_MODE;

typedef struct {
	uint32_t minute;
	uint32_t second;
	uint32_t millisecond;
} HF_SYSTEM_TIMER;

typedef struct {
	HF_SYSTEM_TIMER_SERVICE_MODE timerMode;
	uint32_t startTime;
	uint32_t lastingTime;
} HF_SYSTEM_TIMER_SERVICE;


extern HF_SYSTEM_TIMER hd_system_timer;

HF_SYSTEM_TIMER_SERVICE timer_service_table[HF_NUMBER_TIMER];



void systemTimerServiceSetTimer(HF_SYSTEM_TIMER_SERVICE_ID tiemr_id, HF_SYSTEM_TIMER_SERVICE_MODE timer_mode, uint32_t lastingTime);

uint8_t systemTimerServiceCheckEnd(HF_SYSTEM_TIMER_SERVICE_ID tiemr_id);

uint32_t systemTimerServiGetDifTime(HF_SYSTEM_TIMER_SERVICE_ID tiemr_id);


#endif /* HF_TIMER_SERVICE_H_ */
