#include "lv_mcu_driver.h"
#include "st7789.h"
#include "at32f403a_407_clock.h"
#include "at32f403a_407_board.h"


void lv_mcu_spiSendData(uint8_t *pData, uint16_t Size)
{
	// while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY);
	// HAL_SPI_Transmit(&hspi2, pData, Size, 1000);
}

void lv_mcu_spiSendColor(uint8_t *pData, uint16_t Size)
{
	// while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY);
	// HAL_SPI_Transmit_DMA(&hspi2, pData, Size);
}

uint8_t lv_mcu_spiIsReady(void)
{
	// uint8_t isReady = HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_READY;
	// return isReady;
	return 0;
}

void lv_mcu_spi_io_init(void)
{
	gpio_init_type gpio_init_struct;	
	/* enable the gpioa clock */
	crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);	
	/* set default parameter */
	gpio_default_para_init(&gpio_init_struct);	
	/* configure the gpio */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_9 | GPIO_PINS_8 | GPIO_PINS_10 | GPIO_PINS_11;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOA, &gpio_init_struct);
	
	
	gpio_default_para_init(&gpio_init_struct);
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_14 | GPIO_PINS_15;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOB, &gpio_init_struct);
}

