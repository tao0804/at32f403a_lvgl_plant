/**
 * @file app_key.c
 * @brief
 * @author
 * @version 0.1
 * @date 2024-06-11
 *
 * @copyright
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2024-06-11 <td>0.1     <td>jeff     <td>内容
 * </table>
 */
#include "key.h"

static void KEY_scan_do(void);
static uint8_t KEY_scan_result(void);
static key_status_t *KEY_status_get(uint8_t status_index);
static void KEY_change_param(u8 dir);

uint8_t APP_KEY_Scan(void);
void KET_Timeout(void *p);
void TIM2_Init(void);

key_status_t key_status[KEY_MAX + 1] =
	{
		{.key_name = KEY_UP,    .triger = 0, .cont = 0},
		{.key_name = KEY_DOWN,  .triger = 0, .cont = 0},
		{.key_name = KEY_LEFT,  .triger = 0, .cont = 0},
		{.key_name = KEY_RIGHT, .triger = 0, .cont = 0},
		{.key_name = KEY_ENTER, .triger = 0, .cont = 0},
		{.key_name = KEY_SEND,  .triger = 0, .cont = 0},
		{.key_name = KEY_NONE,  .triger = 0, .cont = 0},
};

/**
 * @brief
 */
static void KEY_scan_do(void)
{
	uint8_t pin_value;

	pin_value = gpio_input_data_bit_read(KEY_UP_PORT, KEY_UP_PIN);
	pin_value = gpio_input_data_bit_read(KEY_UP_PORT, KEY_UP_PIN) ^ 0x01;
	key_status[KEY_UP].triger = pin_value & (pin_value ^ key_status[KEY_UP].cont);
	key_status[KEY_UP].cont = pin_value;

	pin_value = gpio_input_data_bit_read(KEY_DOWN_PORT, KEY_DOWN_PIN) ^ 0x01;
	key_status[KEY_DOWN].triger = pin_value & (pin_value ^ key_status[KEY_DOWN].cont);
	key_status[KEY_DOWN].cont = pin_value;

	pin_value = gpio_input_data_bit_read(KEY_LEFT_PORT, KEY_LEFT_PIN) ^ 0x01;
	key_status[KEY_LEFT].triger = pin_value & (pin_value ^ key_status[KEY_LEFT].cont);
	key_status[KEY_LEFT].cont = pin_value;

	pin_value = gpio_input_data_bit_read(KEY_RIGHT_PORT, KEY_RIGHT_PIN) ^ 0x01;
	key_status[KEY_RIGHT].triger = pin_value & (pin_value ^ key_status[KEY_RIGHT].cont);
	key_status[KEY_RIGHT].cont = pin_value;

	pin_value = gpio_input_data_bit_read(KEY_ENTER_PORT, KEY_ENTER_PIN) ^ 0x01;
	key_status[KEY_ENTER].triger = pin_value & (pin_value ^ key_status[KEY_ENTER].cont);
	key_status[KEY_ENTER].cont = pin_value;
}

/**
 * @brief
 * @param  status_index
 * @return key_status_t*
 */
static key_status_t *KEY_status_get(uint8_t status_index)
{
	return &key_status[status_index];
}

/**
 * @brief  configure button gpio
 * @param  button: specifies the button to be configured.
 * @retval none
 */
void KEY_Init(void)
{
	gpio_init_type gpio_init_struct;

	/* enable the button clock */
	crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(KEY_CRM_CLK, TRUE);
	crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

	/* disable the serial wire jtag debug port swj-dp */
	//	gpio_pin_remap_config(SWJTAG_MUX_010, TRUE);
	IOMUX->remap |= 0x02000000;
	IOMUX->remap7 |= 0x00020000;

	/* set default parameter */
	gpio_default_para_init(&gpio_init_struct);

	/* configure button pin as input with pull-up/pull-down */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
	gpio_init_struct.gpio_pins = KEY_PTT_PIN | KEY_ENTER_PIN | KEY_LEFT_PIN | KEY_UP_PIN | KEY_DOWN_PIN;
	gpio_init_struct.gpio_pull = GPIO_PULL_UP;
	gpio_init(KEY_USER_PORT, &gpio_init_struct);

	gpio_init_struct.gpio_pins = KEY_RIGHT_PIN;
	gpio_init(KEY_RIGHT_PORT, &gpio_init_struct);
}

uint8_t KEY_scan_result(void)
{
	key_status_t *p_key_status;
	uint8_t key_index = KEY_NONE, pos;
	static uint8_t key_table_index = KEY_MAX;
	static uint16_t key_press = 0;

	KEY_scan_do();

	for (pos = 0; pos < KEY_MAX; pos++) {
		p_key_status = KEY_status_get(pos);
		if (1 == p_key_status->triger) {
			key_table_index = pos;
			break;
		}
	}

	if (KEY_status_get(key_table_index)->cont == 1) {
		key_press++;
		if (key_press >= KEY_Debounce) {
			key_index = KEY_status_get(key_table_index)->key_name;
			key_table_index = KEY_MAX;
			key_press = 0;
		}
	} else {
		key_table_index = KEY_MAX;
		key_press = 0;
	}

	/*if (pos >= KEY_MAX)
	{
		key_index = KEY_NONE;
	}
	else
	{
		key_index = p_key_status->key_name;
	}*/

	return key_index;
}

static void KEY_change_param(u8 dir)
{
	u8 page_abs_index;
	sys_param_t *p_sys_param;
	rf_table_t *p_rf_table;

	p_sys_param = get_sys_param();
	p_rf_table = get_rf_table(p_sys_param->module_type);
	page_abs_index = p_sys_param->page_index % SHOW_PAGE_MAX;

	switch (p_sys_param->screen_index) {
	case 0:
		break;

	default:
		break;
	}
}

uint8_t APP_KEY_Scan(void)
{
	uint8_t key_index;
	sys_param_t *p_sys_param;
	p_sys_param = get_sys_param();

	key_index = KEY_scan_result();
	switch (key_index) {
	case KEY_UP:
		if (p_sys_param->page_selected == 1 && p_sys_param->screen_index == 0) {
			p_sys_param->page_index--;
			p_sys_param->page_index %= 8;
			p_sys_param->screen_index = (p_sys_param->page_index / SHOW_PAGE_MAX) % SHOW_SCREEN_MAX;
		} else {
			p_sys_param->page_index -= SHOW_PAGE_MAX;
			p_sys_param->screen_index = (p_sys_param->page_index / SHOW_PAGE_MAX) % SHOW_SCREEN_MAX;
		}
		break;
	case KEY_DOWN:
		if (p_sys_param->page_selected == 1 && p_sys_param->screen_index == 0) {
			p_sys_param->page_index++;
			p_sys_param->page_index %= 8;
			p_sys_param->screen_index = (p_sys_param->page_index / SHOW_PAGE_MAX) % SHOW_SCREEN_MAX;
		} else {
			p_sys_param->page_index += SHOW_PAGE_MAX;
			p_sys_param->screen_index = (p_sys_param->page_index / SHOW_PAGE_MAX) % SHOW_SCREEN_MAX;
		}
		break;
	case KEY_LEFT:
		if (p_sys_param->page_selected)
			KEY_change_param(0);
		break;
	case KEY_RIGHT:
		if (p_sys_param->page_selected)
			KEY_change_param(1);
		break;
	case KEY_ENTER:
		if (p_sys_param->screen_index == 0) {
			p_sys_param->page_selected = p_sys_param->page_selected ^ 0x01;
			//							if (!p_sys_param->page_selected)
			//									RF_Init();	//确认修改后重新初始化RF
		}
		break;
	}
	return key_index;
}

uint8_t Key_Index;
void TMR2_GLOBAL_IRQHandler(void)
{
	if (tmr_flag_get(TMR2, TMR_OVF_FLAG) != RESET) {
		tmr_flag_clear(TMR2, TMR_OVF_FLAG);
		Systim_Count++;

		Key_Index = KEY_scan_result();
	}
}

void TIM2_Init(void)
{
	crm_clocks_freq_type crm_clocks_freq_struct;

	/* get system clock */
	crm_clocks_freq_get(&crm_clocks_freq_struct);

	/* enable tmr2 clock */
	crm_periph_clock_enable(CRM_TMR2_PERIPH_CLOCK, TRUE);

	/* tmr2 configuration */
	/* time base configuration */
	/* systemclock/24/1000 = 1khz */
	tmr_cnt_dir_set(TMR2, TMR_COUNT_UP);
	tmr_clock_source_div_set(TMR2, TMR_CLOCK_DIV1);
	tmr_base_init(TMR2, 999, (crm_clocks_freq_struct.ahb_freq / 1000000) - 1);

	/* Period buffer enable */
	tmr_period_buffer_enable(TMR2, TRUE);

	/* overflow interrupt enable */
	tmr_interrupt_enable(TMR2, TMR_OVF_INT, TRUE);
	//	tmr_overflow_request_source_set(TMR2, TRUE);
	//	tmr_overflow_event_disable(TMR2, TRUE);

	/* tmr2 overflow interrupt nvic init */
	nvic_priority_group_config(NVIC_PRIORITY_GROUP_2);
	nvic_irq_enable(TMR2_GLOBAL_IRQn, 1, 0);

	/* enable tmr2 */
	tmr_counter_enable(TMR2, TRUE);
}

void KET_Timeout(void *p)
{
	Key_Index = APP_KEY_Scan();
}
