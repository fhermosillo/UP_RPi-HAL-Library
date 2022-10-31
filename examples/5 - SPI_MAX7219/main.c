#include "bcm2xxx_delay.h"
#include "bcm2xxx_hal_gpio.h"
#include "bcm2xxx_hal_spi.h"
#include "bcm2xxx_hal_pwm.h"
#include "max7219.h"

#define LED_PIN		GPIO_PIN_5
       
int main()
{
	/* OPEN PERIPHERAL'S MEMORY */
	HAL_memory_map(RPI3_MODEL_B);
	
	/* USER CODE BEGIN -------------------------------------------------------------------*/
	// *****************************
	// HAL Peripheral Initialization
	// *****************************
	HAL_GPIO_Init();
	
	// *****************************
	// Peripheral Configuration
	// *****************************
	/* GPIO Config */
	HAL_GPIO_Set_Mode(LED_PIN, GPIO_OUTPUT);
	
	/* SPI Config */
	// Configure SPI0
	HAL_GPIO_Set_Mode(GPIO_PIN_10, GPIO_FUNCTION_0); // MOSI
	HAL_GPIO_Set_Mode(GPIO_PIN_9, GPIO_FUNCTION_0);// MISO
	HAL_GPIO_Set_Mode(GPIO_PIN_11, GPIO_FUNCTION_0);// SCLK
	HAL_GPIO_Set_Mode(GPIO_PIN_8, GPIO_FUNCTION_0);// CS0
	HAL_GPIO_Set_Mode(GPIO_PIN_7, GPIO_FUNCTION_0);// CS1
	
	SPI_t *SPI0 = HAL_SPI_Init(SPI0_DRIVER);
	HAL_SPI_Set_Baud(SPI0, MHZ(1));
	// *****************************
	// Main program
	// *****************************
	
	
	/* MAX7219 Initialization */
	MAX7219_Begin(SPI0, SPI_CHIP_0);	// Use CS0 from SPI0
	
	/* Main Loop */
	// Custom icon (created by https://xantorohara.github.io/led-matrix-editor/)
	uint8_t arrow[][8]={{0b00011000,0b00011000,0b00011000,0b00011000,0b00011000,0b01111110,0b00111100,0b00011000}};
	MAX7219_AnimateData(arrow, 1, 7, 60, MAX7219_DIR_LEFT);
	
	const char msg[] = "Hola, este es un mensaje";
	while(1)
	{
		MAX7219_AnimateText(msg, sizeof(msg), 40, MAX7219_DIR_RIGHT);
	}
	/* USER CODE END -------------------------------------------------------------------*/
	
	/* CLOSE PERIPHERAL'S MEMORY */
	HAL_memory_unmap();
	
	/* EXIT */
	return 0;
}

