/*
 * HF_timer_service.c
 *
 *  Created on: 22 oct. 2016
 *      Author: HD-L
 */
#include "HF_timer_service.h"

HF_SYSTEM_TIMER hd_system_timer;

void systemTimerServiceSetTimer(HF_SYSTEM_TIMER_SERVICE_ID tiemr_id, HF_SYSTEM_TIMER_SERVICE_MODE timer_mode, uint32_t lastingTime) {
    timer_service_table[tiemr_id].lastingTime = lastingTime;
    timer_service_table[tiemr_id].timerMode = timer_mode;
    if (timer_mode == HF_TIMER_MILLISECOND || timer_mode == HF_TIMER_MILLISECOND_AUTO_RESET) {
        timer_service_table[tiemr_id].startTime = hd_system_timer.millisecond;
    } else if (timer_mode == HF_TIMER_SECOND || timer_mode == HF_TIMER_SECOND_AUTO_RESET) {
        timer_service_table[tiemr_id].startTime = hd_system_timer.second;
    } else if (timer_mode == HF_TIMER_MINUTE || timer_mode == HF_TIMER_MINUTE_AUTO_RESET) {
        timer_service_table[tiemr_id].startTime = hd_system_timer.minute;
    }
}

uint8_t systemTimerServiceCheckEnd(HF_SYSTEM_TIMER_SERVICE_ID tiemr_id) {
    if (timer_service_table[tiemr_id].timerMode == HF_TIMER_MILLISECOND
            || timer_service_table[tiemr_id].timerMode == HF_TIMER_MILLISECOND_AUTO_RESET) {
        if ((hd_system_timer.millisecond - timer_service_table[tiemr_id].startTime) >= timer_service_table[tiemr_id].lastingTime) {
            if (timer_service_table[tiemr_id].timerMode == HF_TIMER_MILLISECOND_AUTO_RESET) {
                timer_service_table[tiemr_id].startTime += timer_service_table[tiemr_id].lastingTime;
            }
            return 0;
        } else {
            return 1;
        }
    } else if (timer_service_table[tiemr_id].timerMode == HF_TIMER_SECOND
            || timer_service_table[tiemr_id].timerMode == HF_TIMER_SECOND_AUTO_RESET) {
        if ((hd_system_timer.second - timer_service_table[tiemr_id].startTime) >= timer_service_table[tiemr_id].lastingTime) {
            if (timer_service_table[tiemr_id].timerMode == HF_TIMER_SECOND_AUTO_RESET) {
                timer_service_table[tiemr_id].startTime += timer_service_table[tiemr_id].lastingTime;
            }
            return 0;
        } else {
            return 1;
        }
    } else if (timer_service_table[tiemr_id].timerMode == HF_TIMER_MINUTE
            || timer_service_table[tiemr_id].timerMode == HF_TIMER_MINUTE_AUTO_RESET) {
        if ((hd_system_timer.minute - timer_service_table[tiemr_id].startTime) >= timer_service_table[tiemr_id].lastingTime) {
            if (timer_service_table[tiemr_id].timerMode == HF_TIMER_MINUTE_AUTO_RESET) {
                timer_service_table[tiemr_id].startTime += timer_service_table[tiemr_id].lastingTime;
            }
            return 0;
        } else {
            return 1;
        }
    }
}

uint32_t systemTimerServiGetDifTime(HF_SYSTEM_TIMER_SERVICE_ID tiemr_id) {
    uint32_t instance = 0;
    uint32_t distance = 0;
    uint32_t startTime = 0;
//    startTime = timer_service_table[tiemr_id].startTime;
//    if (timer_service_table[tiemr_id].timerMode == TIMER_MILLISECOND
//            || timer_service_table[tiemr_id].timerMode == TIMER_MILLISECOND_AUTO_RESET) {
//        PIE2bits.TMR3IE = 0;
//        instance = system_timer.millisecond;
//        PIE2bits.TMR3IE = 1;
//    } else if (timer_service_table[tiemr_id].timerMode == TIMER_SECOND
//            || timer_service_table[tiemr_id].timerMode == TIMER_SECOND_AUTO_RESET) {
//        INTCONbits.TMR0IE = 0;
//        instance = system_timer.second;
//        INTCONbits.TMR0IE = 1;
//    } else if (timer_service_table[tiemr_id].timerMode == TIMER_MINUTE
//            || timer_service_table[tiemr_id].timerMode == TIMER_MINUTE_AUTO_RESET) {
//        INTCONbits.TMR0IE = 0;
//        instance = system_timer.minute;
//        INTCONbits.TMR0IE = 1;
//    }
//    if (timer_service_table[tiemr_id].timerMode == TIMER_MILLISECOND_AUTO_RESET ||
//            timer_service_table[tiemr_id].timerMode == TIMER_SECOND_AUTO_RESET ||
//            timer_service_table[tiemr_id].timerMode == TIMER_MINUTE_AUTO_RESET) {
//        timer_service_table[tiemr_id].startTime = instance;
//    }
//    distance = instance - startTime;
    return distance;
}
