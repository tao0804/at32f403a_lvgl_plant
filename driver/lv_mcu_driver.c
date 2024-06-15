#include "lv_mcu_driver.h"
#include "st7789.h"
#include "at32f403a_407_clock.h"
#include "my_delay.h"

// #define HARDWARE_SPI2

#define SPI_SCK_0 GPIOA->odt &= ~GPIO_PINS_10    // 设置sck接口到PA10
#define SPI_SCK_1 GPIOA->odt |= GPIO_PINS_10
#define SPI_SDA_0 GPIOB->odt &= ~GPIO_PINS_15    // 设置SDA接口到PB15
#define SPI_SDA_1 GPIOB->odt |= GPIO_PINS_15
#define SPI_CS_0  GPIOA->odt &= ~GPIO_PINS_8    // 设置CS接口到PA8
#define SPI_CS_1  GPIOA->odt |= GPIO_PINS_8

/*************SPI配置函数*******************
SCL空闲时低电平,第一个上升沿采样,模拟SPI
SPI模块发送函数
**************************************/
// tao:CPOL=0,CPHA=0
// tao:在第二个跳变沿(下降沿)改变数据,发送完重新置空闲状态(低),lcd驱动依赖
#ifndef HARDWARE_SPI2
static void SPI_SendByte(unsigned char byte)    // 向液晶屏写一个8位数据
{
	unsigned char counter;
	for (counter = 0; counter < 8; counter++) {
		SPI_SCK_0;
		if ((byte & 0x80) == 0) {
			SPI_SDA_0;
		} else
			SPI_SDA_1;
		byte = byte << 1;
		SPI_SCK_1;
	}
	SPI_SCK_0;
}
#endif

/**
 * @brief  spi configuration.
 * @param  none
 * @retval none
 */
#ifdef HARDWARE_SPI2
static void spi_config(void)
{
	spi_init_type spi_init_struct;
	crm_periph_clock_enable(CRM_SPI2_PERIPH_CLOCK, TRUE);
	spi_default_para_init(&spi_init_struct);
	spi_init_struct.transmission_mode = SPI_TRANSMIT_FULL_DUPLEX;
	spi_init_struct.master_slave_mode = SPI_MODE_MASTER;
	spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_8;    // focus lcd manual the highest MCLK and verify it. the higher, the shorter.
	spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_LSB;
	spi_init_struct.frame_bit_num = SPI_FRAME_8BIT;
	spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_LOW;
	spi_init_struct.clock_phase = SPI_CLOCK_PHASE_1EDGE;    // mcu hardware communicates with lcd spi
	spi_init_struct.cs_mode_selection = SPI_CS_SOFTWARE_MODE;
	spi_init(SPI2, &spi_init_struct);
	spi_enable(SPI2, TRUE);
}
#endif


void lv_mcu_spiSendData(uint8_t *pData, uint16_t Size)
{
	SPI_CS_0;
	for (uint16_t i = 0; i < Size; i++) {
#ifdef HARDWARE_SPI2
		while (spi_i2s_flag_get(SPI2, SPI_I2S_TDBE_FLAG) == RESET)
			;
		spi_i2s_data_transmit(SPI2, pData[i]);
#else
		SPI_SendByte(pData[i]);
#endif
	}
	SPI_CS_1;
}

void lv_mcu_spiSendColor(uint8_t *pData, uint16_t Size)
{
	SPI_CS_0;
	for (uint16_t i = 0; i < Size; i++) {
#ifdef HARDWARE_SPI2
		while (spi_i2s_flag_get(SPI2, SPI_I2S_TDBE_FLAG) == RESET)
			;
		spi_i2s_data_transmit(SPI2, pData[i]);
#else
		SPI_SendByte(pData[i]);
#endif
	}
	SPI_CS_1;
}

uint8_t lv_mcu_spiIsReady(void)
{
	return 1;
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
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_9 | GPIO_PINS_11;    // lcd dc, rst
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOA, &gpio_init_struct);

	gpio_init_struct.gpio_pins = GPIO_PINS_14;    // lcd blk
	gpio_init(GPIOB, &gpio_init_struct);

	gpio_init_struct.gpio_pins = GPIO_PINS_8;    // cs
	gpio_init(GPIOA, &gpio_init_struct);

#ifdef HARDWARE_SPI2
	gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
	gpio_init_struct.gpio_pins = GPIO_PINS_13 | GPIO_PINS_15;    // sck, mosi
	gpio_init(GPIOB, &gpio_init_struct);
	spi_config();
#else
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_10;    // sck
	gpio_init(GPIOA, &gpio_init_struct);

	gpio_init_struct.gpio_pins = GPIO_PINS_15;    // mosi
	gpio_init(GPIOB, &gpio_init_struct);
#endif
}
