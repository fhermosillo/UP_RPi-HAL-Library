#include "bcm2xxx_delay.h"
#include "bcm2xxx_hal_gpio.h"
#include "bcm2xxx_hal_i2c.h"
#include "bcm2xxx_hal_cm.h"


// Define GPIO PINES
#define GPIO_SDA	GPIO_PIN_2
#define GPIO_SCL	GPIO_PIN_3
#define GPIO_XCLK	GPIO_PIN_4


// I2C Constants
#define OV7670_ADDRESS		0x21
#define OV7670_LOWER_REG	0x00
#define OV7670_UPPER_REG	0xC9

int main()
{
	/* OPEN PERIPHERAL'S MEMORY */
	HAL_memory_map(RPI3_MODEL_B);
	
	/* USER CODE BEGIN -------------------------------------------------------------------*/
	// Peripheral Configurations
	HAL_GPIO_Init();
	I2C_t *I2C1 = HAL_I2C_Init(I2C1_DRIVE);
	HAL_CM_Init(CM_DRIVER_GPIO0);
	
	// Set GPIO2 and GPIO3 as SDA and SCL pines
	// Set GPIO4 as GPIO_CLOCK
	HAL_GPIO_Set_Mode(GPIO_SDA, GPIO_FUNCTION_0);
	HAL_GPIO_Set_Mode(GPIO_SCL, GPIO_FUNCTION_0);
	HAL_GPIO_Set_Mode(GPIO_XCLK, GPIO_FUNCTION_0);
	
	// Setup XCLK signal for OV7670 as a clock of 10MHz
	HAL_CM_Set_ClkSrc(CM_GPIO0, CM_SOURCE_PLLD);
	HAL_CM_Set_Freq(CM_GPIO0, 10000000);
	HAL_CM_Start(CM_GPIO0);
	delay_ms(100);
	
	// Setup I2C with a clock of 100KHz to transmit 100Kbps
	HAL_I2C_Set_Baud(I2C1, 100000);
	HAL_I2C_Set_Address(I2C1, OV7670_ADDRESS);
	
	// Main program
	while(1)
	{
		// Verify if OV7670 is connected to the i2c bus
		if( !HAL_I2C_Is_Connected(I2C1, OV7670_ADDRESS) )
		{
			printf("Error(@I2C) - OV7670 was not found!\n");
			break;
		}
		
		// Ask user for register
		uint32_t reg_addr = 0x00;
		uint32_t data = 0x00;
		printf("Register to write: ");
		scanf("%d", &reg_addr);
		printf("Value to write: ");
		scanf("%d", &data);
		
		// Reg validation
		if(reg_addr >= OV7670_LOWER_REG && reg_addr <= OV7670_UPPER_REG)
		{
			// Write to register
			eI2CError err = HAL_I2C_WriteReg(I2C1, reg_addr, data);
			
			// Verify writing errors
			if(err != I2C_ERROR_NONE)
			{
				printf("Error(@I2C): Unable to write\n");
			}
		} else
		{
			printf("Error(@OV7670): Invalid register address\n");
		}
		
		printf("Register to read: ");
		scanf("%d", &reg_addr);
		// Reg validation
		if(reg_addr >= OV7670_LOWER_REG && reg_addr <= OV7670_UPPER_REG)
		{
			// Read register
			uint8_t rxdata;
			eI2CError err = HAL_I2C_ReadReg(I2C1, reg_addr, &rxdata);
			
			// Verify reading errors
			if(err != I2C_ERROR_NONE)
			{
				printf("Error(@I2C): Unable to read\n");
			} else 
			{
				printf("Register 0x%02X contains 0x%02X\n", reg_addr, rxdata);
			}
		} else
		{
			printf("Error(@OV7670): Invalid register address\n");
		}
	}
	/* USER CODE END -------------------------------------------------------------------*/
	
	/* CLOSE PERIPHERAL'S MEMORY */
	HAL_memory_unmap();
	
	/* EXIT */
	return 0;
}
