#ifndef __HF_LEDS_H
#define __HF_LEDS_H

#include "stm32f0xx_hal.h"
#include "stm32f072xb.h"

typedef enum LED_PATTERN_NAME {
	INFINITY,
	DOUBLE_INFINITY,
	DOUBLE_LOOP,
	FLASHALL
} LED_PATTERN_NAME;

void run_led(LED_PATTERN_NAME name);
void run_next_led();
void run_led_infinity();
void run_led_dinfinity();
void run_led_dloop();
void run_led_flash();
void test_all_leds();
//void tick_led_pattern(HF_Led_Pattern* pattern);


#endif /* HF_LEDS_H */
