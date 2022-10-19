#include "bcm2xxx_delay.h"
#include "ds1307_pi.h"
#include "max7219_pi.h"

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
	/* DS1307 Configuration */
	DS1307_Setup();
	
	/* MAX7219 Configuration */
	MAX7219_Setup();
	
	// *****************************
	// Main program
	// *****************************
	while(1)
	{
		// Read time from DS1307
		
		// Write to MAX7219 data according to time
		
	}
	/* USER CODE END -------------------------------------------------------------------*/
	
	/* CLOSE PERIPHERAL'S MEMORY */
	HAL_memory_unmap();
	
	/* EXIT */
	return 0;
}

