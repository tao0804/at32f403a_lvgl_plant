#include "my_delay.h"
#include "at32f403a_407_clock.h"
#include "lv_port_disp_template.h"

#include <stdlib.h>
// #include <unistd.h>

#include "lvgl.h"
#include "my_gui.h"

crm_clocks_freq_type crm_clocks_freq_struct = {0};

static void tmr_clk_config(void)
{
	/* get system clock */
	crm_clocks_freq_get(&crm_clocks_freq_struct);

	/* enable TMR7 clock */
	crm_periph_clock_enable(CRM_TMR7_PERIPH_CLOCK, TRUE);

	/* TMR7 configuration */
	/* time base configuration */
	/* systemclock/24000/10000 = 1hz */
	tmr_base_init(TMR7, 9, (crm_clocks_freq_struct.apb1_freq / 10000) - 1);
	tmr_cnt_dir_set(TMR7, TMR_COUNT_UP);

	/* overflow interrupt enable */
	tmr_interrupt_enable(TMR7, TMR_OVF_INT, TRUE);

	/* TMR7 overflow interrupt nvic init */
	nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
	nvic_irq_enable(TMR7_GLOBAL_IRQn, 0, 0);

	/* enable TMR7 */
	tmr_counter_enable(TMR7, TRUE);
}

int main(void)
{
	system_clock_config();
	delay_init();
	tmr_clk_config();
	delay_ms(500);
	lv_init(); /* lvgl 系统初始化 */
	lv_port_disp_init(); /* lvgl 显示接口初始化,放在 lv_init()的后面 */
	LV_LOG_USER("LVGL initialization completed!");

	/*Run the demo*/
	my_gui_init();

	while(1) {
		/* Periodically call the lv_task handler.
		 * It could be done in a timer interrupt or an OS task too.*/
		lv_task_handler();
		delay_ms(10);       /*Just to let the system breath*/
	}
	return 0;
}
