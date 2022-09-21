#include "bcm2xxx_hal_gpio.h"

#define LED_PIN		GPIO_PIN_5
#define BUTTON_PIN	GPIO_PIN_6

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
	
	// Set GPIO6 as Input
	HAL_GPIO_Set_Mode(BUTTON_PIN, GPIO_INPUT);
	// Enable PullUps on GPIO5
	HAL_GPIO_Set_PuPd(BUTTON_PIN, GPIO_PPUD_UP);
	
	// Clear GPIO5
	HAL_GPIO_Write(LED_PIN, GPIO_LOW);
	
	// *****************************
	// Main program
	// *****************************
	while(1)
	{
		// GPIO5 takes the value of GPIO6
		eGPIOState bnt_state = HAL_GPIO_Read(BUTTON_PIN);
		HAL_GPIO_Write(LED_PIN, btn_state);
	}
	/* USER CODE END -------------------------------------------------------------------*/
	
	/* CLOSE PERIPHERAL'S MEMORY */
	HAL_memory_unmap();
	
	/* EXIT */
	return 0;
}

