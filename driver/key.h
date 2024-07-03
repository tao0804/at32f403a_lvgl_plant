#ifndef __KEY_H
#define __KEY_H
#include "at32f403a_407.h"

#define KEY_UP_PORT	GPIOB
#define KEY_UP_PIN	GPIO_PINS_4

#define KEY_DOWN_PORT	GPIOB
#define KEY_DOWN_PIN	GPIO_PINS_3

#define KEY_LEFT_PORT	GPIOB
#define KEY_LEFT_PIN	GPIO_PINS_5

#define KEY_RIGHT_PORT	GPIOA
#define KEY_RIGHT_PIN	GPIO_PINS_15

#define KEY_ENTER_PORT	GPIOB
#define KEY_ENTER_PIN	GPIO_PINS_6

#define KEY_PTT_PORT	GPIOB
#define KEY_PTT_PIN	GPIO_PINS_7
#define KEY_USER_PORT	GPIOB
#define KEY_CRM_CLK	CRM_GPIOB_PERIPH_CLOCK
#define Read_PTT()	gpio_input_data_bit_read(KEY_USER_PORT, KEY_PTT_PIN)

#define KEY_Debounce 20	// 消抖计数

typedef struct
{
	u8 key_name;
	u8 triger;	// 按键触发
	u8 cont;	// 持续按下
}key_status_t;

enum {
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_ENTER,
	KEY_SEND,

	KEY_MAX,
	KEY_NONE,
};

#endif

