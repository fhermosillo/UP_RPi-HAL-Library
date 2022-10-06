#include "bcm2xxx_delay.h"
#include "bcm2xxx_hal_gpio.h"


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
	// Set GPIO5 as an Output
	HAL_GPIO_Set_Mode(LED_PIN, GPIO_OUTPUT);
	
	// Clear GPIO5
	HAL_GPIO_Write(LED_PIN, GPIO_LOW);
	
	// *****************************
	// Main program
	// *****************************
	while(1)
	{
		// GPIO5 Blinking every second
		HAL_GPIO_Toggle(LED_PIN);
		delay_ms(1000);
	}
	/* USER CODE END -------------------------------------------------------------------*/
	
	/* CLOSE PERIPHERAL'S MEMORY */
	HAL_memory_unmap();
	
	/* EXIT */
	return 0;
}

