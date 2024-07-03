/**
 * @file lv_port_indev_templ.c
 *
 */

/*Copy this file as "lv_port_indev.c" and set this value to "1" to enable content*/
#if 1

	/*********************
	 *      INCLUDES
	 *********************/
#include "lv_port_indev.h"
#include "lvgl.h"
#include "key.h"
#include "app_key.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void keypad_init(void);
static void keypad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
// static uint32_t keypad_get_key(void);

/**********************
 *  STATIC VARIABLES
 **********************/

lv_indev_t *indev_keypad;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_indev_init(void)
{
	static lv_indev_drv_t indev_drv;
	/*Initialize your keypad or keyboard if you have*/
	keypad_init();
	/*Register a keypad input device*/
	lv_indev_drv_init(&indev_drv);
	indev_drv.type = LV_INDEV_TYPE_KEYPAD;
	indev_drv.read_cb = keypad_read;
	indev_keypad = lv_indev_drv_register(&indev_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
/*Initialize your keypad*/
static void keypad_init(void)
{
	/*Your code comes here*/
	KEY_Init();
	TIM2_Init();
}

volatile lv_indev_data_t test;

/*Will be called by the library to read the mouse*/
static void keypad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
	/*Get the current x and y coordinates*/
	// mouse_get_xy(&data->point.x, &data->point.y);

	/*Get whether the a key is pressed and save the pressed key*/
	uint8_t act_key = key_readCont();
	if (act_key != KEY_NONE) {
		data->state = LV_INDEV_STATE_PR;

		/*Translate the keys to LVGL control characters according to your key definitions*/
		switch (act_key) {
		case KEY_DOWN:
			act_key = LV_KEY_NEXT;
			break;
		case KEY_UP:
			act_key = LV_KEY_PREV;
			break;
		case KEY_LEFT:
			act_key = LV_KEY_LEFT;
			break;
		case KEY_RIGHT:
			act_key = LV_KEY_RIGHT;
			break;
		case KEY_ENTER:
			act_key = LV_KEY_ENTER;
			break;
		}
	} else {
		data->state = LV_INDEV_STATE_REL;
	}

	data->key = act_key;
	test = *data;
}

/*Get the currently being pressed key.  0 if no key is pressed*/
// static uint32_t keypad_get_key(void)
// {
// 	/*Your code comes here*/
// 	return 0;
// }

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif

