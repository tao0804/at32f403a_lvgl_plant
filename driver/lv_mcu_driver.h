#ifndef LV_MCU_DRIVER_H
#define LV_MCU_DRIVER_H

#include "st7789.h"

#define LV_GPIO_SET(Port, Pin) gpio_bits_set(Port, Pin)
#define LV_GPIO_CLR(Port, Pin) gpio_bits_reset(Port, Pin)

// #define LV_DELAY_MS(ms) HAL_Delay(ms)

#define SPI_SCK_0  GPIOA->odt &= ~GPIO_PINS_10       // 设置sck接口到PA10   清零
#define SPI_SCK_1  GPIOA->odt |= GPIO_PINS_10
#define SPI_SDA_0  GPIOB->odt &= ~GPIO_PINS_15     // 设置SDA接口到PB15
#define SPI_SDA_1  GPIOB->odt |= GPIO_PINS_15
#define SPI_CS_0   GPIOA-> odt &= ~GPIO_PINS_8			// 设置CS接口到PA8
#define SPI_CS_1   GPIOA-> odt |= GPIO_PINS_8

void lv_mcu_spiSendData(uint8_t *pData, uint16_t Size);
void lv_mcu_spiSendColor(uint8_t *pData, uint16_t Size);
uint8_t lv_mcu_spiIsReady(void);
void lv_mcu_spi_io_init(void);

#endif /*LV_MCU_DRIVER_H*/
