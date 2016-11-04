
#include "stm32f0xx_hal.h"
#include "stm32f072xb.h"
#include <HF_leds.h>
#include "main.h"

/* High level pins assignments on Nucleo64
 * Shape:
 *                  LED2      LED5
 *             LED1      LED4      LED7
 *                  LED3      LED6
 *
 *   LED0 - 28S7Q18 P3 - PB0
 *   LED1 - 28S7Q18 P2 - PC1
 *   LED2 - 28S7Q18 P4 - PA4
 *   LED3 - 28S7Q18 P1 - PC0
 *   LED4 - 28S7Q18 P5 - PA1
 *   LED5 - 28S7Q18 P7 - PA0
 *   LED6 - 28S7Q18 P6 - PC3
 *
 * */

LED_PATTERN_NAME LED_RUNNING_PATTERN;
uint8_t LED_PATTERN_TOTAL = 4;

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim17;

// Struct that bind a GPIO port and a GPIO pin.
typedef struct
{
  TIM_HandleTypeDef* timer_;
  uint32_t channel;
} HF_Led;

typedef struct
{
  HF_Led* list;
  uint8_t list_len;
  uint16_t value;
  uint8_t sens;
} HF_Led_Action;

typedef struct
{
  HF_Led_Action* action_list;
  uint8_t action_list_len;
} HF_Led_Pattern;


HF_Led LED_GROUP_1[2];	// For double loop patterns.
HF_Led LED_GROUP_2[2];	// For double loop patterns.
HF_Led LED_GROUP_3[2];	// For double loop patterns.
HF_Led LED_GROUP_4[7];	// For flash patterns.

static const HF_Led HF_LED[7] = {
					  {.timer_=&htim2, .channel=TIM_CHANNEL_1},
					  {.timer_=&htim2, .channel=TIM_CHANNEL_2},
					  {.timer_=&htim2, .channel=TIM_CHANNEL_3},
					  {.timer_=&htim2, .channel=TIM_CHANNEL_4},
					  {.timer_=&htim3, .channel=TIM_CHANNEL_1},
					  {.timer_=&htim3, .channel=TIM_CHANNEL_2},
					  {.timer_=&htim3, .channel=TIM_CHANNEL_3}
					};


volatile uint8_t LED_PATTERN_COUNTER = 0;

HF_Led_Action LED_ACTIONS_INFINITY[] = {
						   {.list=&HF_LED[0], .list_len=1, .value=800},
						   {.list=&HF_LED[0], .list_len=1, .value=150},
						   {.list=&HF_LED[1], .list_len=1, .value=800},
						   {.list=&HF_LED[1], .list_len=1, .value=150},
						   {.list=&HF_LED[2], .list_len=1, .value=800},
						   {.list=&HF_LED[2], .list_len=1, .value=150},
						   {.list=&HF_LED[3], .list_len=1, .value=800},
						   {.list=&HF_LED[3], .list_len=1, .value=150},
						   {.list=&HF_LED[0], .list_len=1, .value=800},
						   {.list=&HF_LED[0], .list_len=1, .value=150},
						   {.list=&HF_LED[4], .list_len=1, .value=800},
						   {.list=&HF_LED[4], .list_len=1, .value=150},
						   {.list=&HF_LED[5], .list_len=1, .value=800},
						   {.list=&HF_LED[5], .list_len=1, .value=150},
						   {.list=&HF_LED[6], .list_len=1, .value=800},
						   {.list=&HF_LED[6], .list_len=1, .value=150}
						  };
HF_Led_Pattern LED_PATTERN_INFINITY = {
									 .action_list = LED_ACTIONS_INFINITY,
									 .action_list_len = 16
								   };

HF_Led_Action LED_ACTIONS_DINFINITY[] = {
						   {.list=&HF_LED[3], .list_len=1, .value=800},
						   {.list=&HF_LED[3], .list_len=1, .value=150},
						   {.list=&HF_LED[2], .list_len=1, .value=800},
						   {.list=&HF_LED[2], .list_len=1, .value=150},
						   {.list=&HF_LED[1], .list_len=1, .value=800},
						   {.list=&HF_LED[1], .list_len=1, .value=150},
						   {.list=&HF_LED[0], .list_len=1, .value=800},
						   {.list=&HF_LED[0], .list_len=1, .value=150},

						   {.list=&HF_LED[3], .list_len=1, .value=800},
						   {.list=&HF_LED[3], .list_len=1, .value=150},
						   {.list=&HF_LED[2], .list_len=1, .value=800},
						   {.list=&HF_LED[2], .list_len=1, .value=150},
						   {.list=&HF_LED[1], .list_len=1, .value=800},
						   {.list=&HF_LED[1], .list_len=1, .value=150},
						   {.list=&HF_LED[0], .list_len=1, .value=800},
						   {.list=&HF_LED[0], .list_len=1, .value=150},

						   {.list=&HF_LED[6], .list_len=1, .value=800},
						   {.list=&HF_LED[6], .list_len=1, .value=150},
						   {.list=&HF_LED[5], .list_len=1, .value=800},
						   {.list=&HF_LED[5], .list_len=1, .value=150},
						   {.list=&HF_LED[4], .list_len=1, .value=800},
						   {.list=&HF_LED[4], .list_len=1, .value=150},
						   {.list=&HF_LED[0], .list_len=1, .value=800},
						   {.list=&HF_LED[0], .list_len=1, .value=150},

						   {.list=&HF_LED[6], .list_len=1, .value=800},
						   {.list=&HF_LED[6], .list_len=1, .value=150},
						   {.list=&HF_LED[5], .list_len=1, .value=800},
						   {.list=&HF_LED[5], .list_len=1, .value=150},
						   {.list=&HF_LED[4], .list_len=1, .value=800},
						   {.list=&HF_LED[4], .list_len=1, .value=150},
						   {.list=&HF_LED[0], .list_len=1, .value=800},
						   {.list=&HF_LED[0], .list_len=1, .value=150}
						  };
HF_Led_Pattern LED_PATTERN_DINFINITY = {
									 .action_list = LED_ACTIONS_DINFINITY,
									 .action_list_len = 32
								   };


HF_Led_Action LED_ACTIONS_DLOOP[] = {
						   {.list=LED_GROUP_1, .list_len=2, .value=800},
						   {.list=LED_GROUP_1, .list_len=2, .value=150},
						   {.list=LED_GROUP_2, .list_len=2, .value=800},
						   {.list=LED_GROUP_2, .list_len=2, .value=150},
						   {.list=&HF_LED[0], .list_len=1, .value=800},
						   {.list=&HF_LED[0], .list_len=1, .value=150},
						   {.list=LED_GROUP_3, .list_len=2, .value=800},
						   {.list=LED_GROUP_3, .list_len=2, .value=150},
						  };
HF_Led_Pattern LED_PATTERN_DLOOP = {
									 .action_list = LED_ACTIONS_DLOOP,
									 .action_list_len = 8
								   };

HF_Led_Action LED_ACTIONS_FLASH[] = {
						   {.list=HF_LED, .list_len=7, .value=800},
						   {.list=HF_LED, .list_len=7, .value=150},
						  };
HF_Led_Pattern LED_PATTERN_FLASH = {
									 .action_list = LED_ACTIONS_FLASH,
									 .action_list_len = 2
								   };
/*
 * FUNCTIONS
 */

void init_led_patterns(){
	LED_GROUP_1[0] = HF_LED[2];
	LED_GROUP_1[1] = HF_LED[5];

	LED_GROUP_2[0] = HF_LED[1];
	LED_GROUP_2[1] = HF_LED[4];

	LED_GROUP_3[0] = HF_LED[3];
	LED_GROUP_3[1] = HF_LED[6];
}

void run_next_led(){
	LED_RUNNING_PATTERN = (LED_RUNNING_PATTERN + 1)%(LED_PATTERN_TOTAL);
	run_new_led(LED_RUNNING_PATTERN);
}

// Safe way to change led pattern. Otherwise, it could cause an integer overflow.
void run_new_led(LED_PATTERN_NAME name){
	LED_PATTERN_COUNTER = 0;
	run_led(name);
}

void run_led(LED_PATTERN_NAME name){
	switch(name){
	case INFINITY: run_led_infinity(); break;
	case DOUBLE_INFINITY: run_led_dinfinity(); break;
	case DOUBLE_LOOP: run_led_dloop(); break;
	case FLASHALL: run_led_flash(); break;
	default: run_led_infinity();
	}
}

void run_led_infinity(){
	tick_led_pattern(&LED_PATTERN_INFINITY);
}

void run_led_dinfinity(){
	tick_led_pattern(&LED_PATTERN_DINFINITY);
}

void run_led_dloop(){
	tick_led_pattern(&LED_PATTERN_DLOOP);
}

void run_led_flash(){
	tick_led_pattern(&LED_PATTERN_FLASH);
}

void tick_led_pattern(HF_Led_Pattern* pattern) {
  HF_Led_Action *action;
  uint8_t action_len = pattern->action_list_len;
  uint8_t i;

  action = &(pattern->action_list[LED_PATTERN_COUNTER]);

  for(i=0;i<action->list_len;i++){
	  __HAL_TIM_SET_COMPARE(action->list[i].timer_,
			            action->list[i].channel,
						action->value);
  }

  LED_PATTERN_COUNTER++;

  //When all actions are done, reset counter to 0
  if (LED_PATTERN_COUNTER >= action_len){
	  LED_PATTERN_COUNTER = 0;
  }
}

void test_all_LED(){
	for(int i=0; i<7; i++){
		__HAL_TIM_SET_COMPARE(HF_LED[i].timer_,HF_LED[i].channel,50);
		HAL_Delay(250);
	}
}
