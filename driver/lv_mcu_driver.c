#include "lv_mcu_driver.h"
#include "st7789.h"
#include "at32f403a_407_clock.h"
#include "my_delay.h"



/*************SPI配置函数*******************
SCL空闲时低电平,第一个上升沿采样,模拟SPI
SPI模块发送函数
**************************************/

// tao:CPOL=0,CPHA=0
// tao:在第二个跳变沿(下降沿)改变数据,发送完重新置空闲状态(低),lcd驱动依赖
static void SPI_SendByte(unsigned char byte)				//向液晶屏写一个8位数据
{
	unsigned char counter;
	for(counter=0;counter<8;counter++)
	{ 
		SPI_SCK_0;		  
		if((byte&0x80)==0)
		{
			SPI_SDA_0;
		}
		else SPI_SDA_1;
		byte=byte<<1;	
		SPI_SCK_1;		
	}	
	SPI_SCK_0;
}

void lv_mcu_spiSendData(uint8_t *pData, uint16_t Size)
{
	SPI_CS_0;	// before send data
	for (uint16_t i = 0; i < Size; i++)
	{
		SPI_SendByte(pData[i]);
	}
	SPI_CS_1;	// send data over
}

void lv_mcu_spiSendColor(uint8_t *pData, uint16_t Size)
{
	SPI_CS_0;
	for (uint16_t i = 0; i < Size; i++)
	{
		SPI_SendByte(pData[i]);
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

