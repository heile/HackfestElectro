#include "stm32f0xx_hal.h"
#include "stm32f072xb.h"

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

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
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

static const HF_Led HF_LED[7] = {
					  {.timer_=&htim2, .channel=TIM_CHANNEL_1},
					  {.timer_=&htim2, .channel=TIM_CHANNEL_2},
					  {.timer_=&htim2, .channel=TIM_CHANNEL_3},
					  {.timer_=&htim2, .channel=TIM_CHANNEL_4},
					  {.timer_=&htim3, .channel=TIM_CHANNEL_1},
					  {.timer_=&htim3, .channel=TIM_CHANNEL_2},
					  {.timer_=&htim3, .channel=TIM_CHANNEL_3}
					};

LED_PATTERN_COUNTER = 0;

HF_Led_Action LEDS_ACTIONS_INFINITY[] = {
						   {.list=&HF_LED[0], .list_len=1, .value=1000},
						   {.list=&HF_LED[0], .list_len=1, .value=300},
						   {.list=&HF_LED[1], .list_len=1, .value=1000},
						   {.list=&HF_LED[1], .list_len=1, .value=300},
						   {.list=&HF_LED[2], .list_len=1, .value=1000},
						   {.list=&HF_LED[2], .list_len=1, .value=300},
						   {.list=&HF_LED[3], .list_len=1, .value=1000},
						   {.list=&HF_LED[3], .list_len=1, .value=300},
						   {.list=&HF_LED[0], .list_len=1, .value=1000},
						   {.list=&HF_LED[0], .list_len=1, .value=300},
						   {.list=&HF_LED[4], .list_len=1, .value=1000},
						   {.list=&HF_LED[4], .list_len=1, .value=300},
						   {.list=&HF_LED[5], .list_len=1, .value=1000},
						   {.list=&HF_LED[5], .list_len=1, .value=300},
						   {.list=&HF_LED[6], .list_len=1, .value=1000},
						   {.list=&HF_LED[6], .list_len=1, .value=300}
						  };

//HF_Led LEDS_GROUP_0_6[] = {{.timer_=GPIOB, .channel=GPIO_PIN_0}, {.timer_=GPIOA, .channel=GPIO_PIN_0}};
//HF_Led LEDS_GROUP_1_4[] = {{.timer_=GPIOC, .channel=GPIO_PIN_1}, {.timer_=GPIOA, .channel=GPIO_PIN_1}};
//HF_Led LEDS_GROUP_2_5[] = {{.timer_=GPIOA, .channel=GPIO_PIN_4}, {.timer_=GPIOC, .channel=GPIO_PIN_3}};


//HF_Led_Action LEDS_ACTIONS_DLOOP[] = {
//						   {.list=LEDS_GROUP_0_6, .list_len=2, .off_list={0}, .off_list_len=0},
//						   {.list={0}, .list_len=0, .off_list=LEDS_GROUP_0_6, .off_list_len=2},
//						   {.list=LEDS_GROUP_1_4, .list_len=2, .off_list={0}, .off_list_len=0},
//						   {.list={0}, .list_len=0, .off_list=LEDS_GROUP_1_4, .off_list_len=2},
//						   {.list=&HF_LED[3], .list_len=1, .off_list={0}, .off_list_len=0},
//						   {.list={0}, .list_len=0, .off_list=&HF_LED[3], .off_list_len=1},
//						   {.list=LEDS_GROUP_2_5, .list_len=2, .off_list={0}, .off_list_len=0},
//						   {.list={0}, .list_len=0, .off_list=LEDS_GROUP_2_5, .off_list_len=2}
//						  };


HF_Led_Pattern LEDS_PATTERN_INFINITY = {
									 .action_list = LEDS_ACTIONS_INFINITY,
									 .action_list_len = 16
								   };

//HF_Led_Pattern LEDS_PATTERN_DLOOP = {
//									 .action_list = LEDS_ACTIONS_DLOOP,
//									 .action_list_len = 8
//								   };

/*
 * FUNCTIONS
 */

void turn_on(){
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
}
void turn_off(){
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
}

//void test_all_leds(){
//	for(int i=0; i<7; i++){
//		__HAL_TIM_SET_COMPARE(HF_LED[i].timer_,HF_LED[i].channel,500);
//		HAL_Delay(250);
//	}
//
//}

void run_led_infinity(){
  tick_led_pattern(&LEDS_PATTERN_INFINITY);
}

//void run_led_dloop(){
//	tick_led_pattern(&LEDS_PATTERN_DLOOP);
//}

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
  if (LED_PATTERN_COUNTER == action_len){
	  LED_PATTERN_COUNTER = 0;
  }
}
