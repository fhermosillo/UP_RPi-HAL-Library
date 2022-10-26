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
	
	// *****************************
	// Main program
	// *****************************
	/* LED OFF */
	HAL_GPIO_Write(LED_PIN, GPIO_LOW);
	
	/* MAX7219 Initialization */
	MAX7219_Begin();
	//MAX6675_Begin();
	
	/* Main Loop */
	// Custom icon (created in https://xantorohara.github.io/led-matrix-editor/)
	uint8_t arrow[][8]={{0b00011000,0b00011000,0b00011000,0b00011000,0b00011000,0b01111110,0b00111100,0b00011000}};
	MAX7219_AnimateData(arrow, 1, 0, 60, MAX7219_DIR_LEFT);
	
	const char msg[] = "Hola, este es un mensaje";
	while(1)
	{
		MAX7219_AnimateText(msg, sizeof(msg), 60, MAX7219_DIR_RIGHT);
	}
	/* USER CODE END -------------------------------------------------------------------*/
	
	/* CLOSE PERIPHERAL'S MEMORY */
	HAL_memory_unmap();
	
	/* EXIT */
	return 0;
}

