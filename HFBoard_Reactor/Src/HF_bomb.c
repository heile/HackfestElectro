/*
 * HF_bomb.c
 *
 *  Created on: Nov 4, 2016
 *      Author: HD-S855
 */

#include "HF_bomb.h"
#include "HF_timer_service.h"

void HfBombProcess(){
	static uint8_t alarm_times=0;
	if (bomb.state==BOMB_FLAG_STATE_VALID){
		bomb.state=BOMB_FLAG_STATE_PROCESSING_WITH_ALARM;
		systemTimerServiceSetTimer(HF_TMR_BOMB_alarm, HF_TIMER_MILLISECOND_AUTO_RESET, 500);
		systemTimerServiceSetTimer(HF_TMR_BOMB, HF_TIMER_SECOND, 30);
		printf("start bomb \r\n");
	}
	if (bomb.state==BOMB_FLAG_STATE_PROCESSING_WITH_ALARM){
		if (systemTimerServiceCheckEnd(HF_TMR_BOMB_alarm) == 0) {
			alarm_times++;
			if(alarm_times%2==0){
				systemTimerServiceSetTimer(HF_TMR_BOMB_alarm, HF_TIMER_MILLISECOND_AUTO_RESET, 500);
				printf("start bomb bibi\r\n");
			}else if (alarm_times>=12){
				systemTimerServiceSetTimer(HF_TMR_BOMB_alarm, HF_TIMER_MILLISECOND_AUTO_RESET, 500);
				bomb.state=BOMB_FLAG_STATE_PROCESSING_NON_ALARM;
				printf("start bomb bibi fin\r\n");
			}else{
				printf("start bomb silence\r\n");
			}
		}
	}
	if (bomb.state==BOMB_FLAG_STATE_PROCESSING_NON_ALARM){
		if (systemTimerServiceCheckEnd(HF_TMR_BOMB) == 0) {
			printf("stop bomb eau\r\n");
			bomb.state=BOMB_FLAG_STATE_FIN;
		}
	}
	if (bomb.state==BOMB_FLAG_STATE_FIN){
		printf("fin bomb\r\n");
		bomb.state=BOMB_FLAG_STATE_WATTING;
	}

}
