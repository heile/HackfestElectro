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

// Struct that bind a GPIO port and a GPIO pin.
typedef struct
{
  GPIO_TypeDef* gpio;
  uint16_t pin;
} HF_Led;

typedef struct
{
  HF_Led* on_list;
  uint8_t on_list_len;
  HF_Led* off_list;
  uint8_t off_list_len;
} HF_Led_Action;

typedef struct
{
  HF_Led_Action* action_list;
  uint8_t action_list_len;
} HF_Led_Pattern;

static const HF_Led HF_LED[7] = {
					  {.gpio=GPIOB, .pin=GPIO_PIN_0},
					  {.gpio=GPIOC, .pin=GPIO_PIN_1},
					  {.gpio=GPIOA, .pin=GPIO_PIN_4},
					  {.gpio=GPIOC, .pin=GPIO_PIN_0},
					  {.gpio=GPIOA, .pin=GPIO_PIN_1},
					  {.gpio=GPIOC, .pin=GPIO_PIN_3},
					  {.gpio=GPIOA, .pin=GPIO_PIN_0}
					};

LED_PATTERN_COUNTER = 0;

HF_Led_Action LEDS_ACTIONS_INFINITY[] = {
						   {.on_list=&HF_LED[0], .on_list_len=1, .off_list={0}, .off_list_len=0},
						   {.on_list={0}, .on_list_len=0, .off_list=&HF_LED[0], .off_list_len=1},
						   {.on_list=&HF_LED[1], .on_list_len=1, .off_list={0}, .off_list_len=0},
						   {.on_list={0}, .on_list_len=0, .off_list=&HF_LED[1], .off_list_len=1},
						   {.on_list=&HF_LED[3], .on_list_len=1, .off_list={0}, .off_list_len=0},
						   {.on_list={0}, .on_list_len=0, .off_list=&HF_LED[3], .off_list_len=1},
						   {.on_list=&HF_LED[5], .on_list_len=1, .off_list={0}, .off_list_len=0},
						   {.on_list={0}, .on_list_len=0, .off_list=&HF_LED[5], .off_list_len=1},
						   {.on_list=&HF_LED[6], .on_list_len=1, .off_list={0}, .off_list_len=0},
						   {.on_list={0}, .on_list_len=0, .off_list=&HF_LED[6], .off_list_len=1},
						   {.on_list=&HF_LED[4], .on_list_len=1, .off_list={0}, .off_list_len=0},
						   {.on_list={0}, .on_list_len=0, .off_list=&HF_LED[4], .off_list_len=1},
						   {.on_list=&HF_LED[3], .on_list_len=1, .off_list={0}, .off_list_len=0},
						   {.on_list={0}, .on_list_len=0, .off_list=&HF_LED[3], .off_list_len=1},
						   {.on_list=&HF_LED[2], .on_list_len=1, .off_list={0}, .off_list_len=0},
						   {.on_list={0}, .on_list_len=0, .off_list=&HF_LED[2], .off_list_len=1}
						  };

HF_Led LEDS_GROUP_0_6[] = {{.gpio=GPIOB, .pin=GPIO_PIN_0}, {.gpio=GPIOA, .pin=GPIO_PIN_0}};
HF_Led LEDS_GROUP_1_4[] = {{.gpio=GPIOC, .pin=GPIO_PIN_1}, {.gpio=GPIOA, .pin=GPIO_PIN_1}};
HF_Led LEDS_GROUP_2_5[] = {{.gpio=GPIOA, .pin=GPIO_PIN_4}, {.gpio=GPIOC, .pin=GPIO_PIN_3}};


HF_Led_Action LEDS_ACTIONS_DLOOP[] = {
						   {.on_list=LEDS_GROUP_0_6, .on_list_len=2, .off_list={0}, .off_list_len=0},
						   {.on_list={0}, .on_list_len=0, .off_list=LEDS_GROUP_0_6, .off_list_len=2},
						   {.on_list=LEDS_GROUP_1_4, .on_list_len=2, .off_list={0}, .off_list_len=0},
						   {.on_list={0}, .on_list_len=0, .off_list=LEDS_GROUP_1_4, .off_list_len=2},
						   {.on_list=&HF_LED[3], .on_list_len=1, .off_list={0}, .off_list_len=0},
						   {.on_list={0}, .on_list_len=0, .off_list=&HF_LED[3], .off_list_len=1},
						   {.on_list=LEDS_GROUP_2_5, .on_list_len=2, .off_list={0}, .off_list_len=0},
						   {.on_list={0}, .on_list_len=0, .off_list=LEDS_GROUP_2_5, .off_list_len=2}
						  };


HF_Led_Pattern LEDS_PATTERN_INFINITY = {
									 .action_list = LEDS_ACTIONS_INFINITY,
									 .action_list_len = 16
								   };

HF_Led_Pattern LEDS_PATTERN_DLOOP = {
									 .action_list = LEDS_ACTIONS_DLOOP,
									 .action_list_len = 8
								   };

/*
 * FUNCTIONS
 */

void turn_on(){
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
}
void turn_off(){
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
}

void test_all_leds(){
	for(int i=0; i<7; i++){
		HAL_GPIO_WritePin(HF_LED[i].gpio,HF_LED[i].pin,GPIO_PIN_SET);
		HAL_Delay(250);
	}
	for(int i=0; i<7; i++){
		HAL_GPIO_WritePin(HF_LED[i].gpio,HF_LED[i].pin,GPIO_PIN_RESET);
		HAL_Delay(250);
	}
}

void run_led_infinity(){
  tick_led_pattern(&LEDS_PATTERN_INFINITY);
}

void run_led_dloop(){
	tick_led_pattern(&LEDS_PATTERN_DLOOP);
}

void tick_led_pattern(HF_Led_Pattern* pattern) {
  HF_Led_Action *action;
  uint8_t action_len = pattern->action_list_len;
  uint8_t i;

  action = &(pattern->action_list[LED_PATTERN_COUNTER]);

  for(i=0;i<action->on_list_len;i++){
	  HAL_GPIO_WritePin(action->on_list[i].gpio,
			            action->on_list[i].pin,
						GPIO_PIN_SET);
  }

  for(i=0;i<action->off_list_len;i++){
	  HAL_GPIO_WritePin(action->off_list[i].gpio,
			            action->off_list[i].pin,
						GPIO_PIN_RESET);
  }

  LED_PATTERN_COUNTER++;

  //When all actions are done, reset counter to 0
  if (LED_PATTERN_COUNTER == action_len){
	  LED_PATTERN_COUNTER = 0;
  }
}
