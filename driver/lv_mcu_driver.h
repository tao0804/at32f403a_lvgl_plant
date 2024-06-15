#ifndef LV_MCU_DRIVER_H
#define LV_MCU_DRIVER_H

#include "st7789.h"

#define LV_GPIO_SET(Port, Pin) gpio_bits_set(Port, Pin)
#define LV_GPIO_CLR(Port, Pin) gpio_bits_reset(Port, Pin)

// #define LV_DELAY_MS(ms) HAL_Delay(ms)

void lv_mcu_spiSendData(uint8_t *pData, uint16_t Size);
void lv_mcu_spiSendColor(uint8_t *pData, uint16_t Size);
uint8_t lv_mcu_spiIsReady(void);
void lv_mcu_spi_io_init(void);

#endif /*LV_MCU_DRIVER_H*/
