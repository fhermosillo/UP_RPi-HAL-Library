#include "bcm2xxx_delay.h"
#include "bcm2xxx_hal_gpio.h"


#define MOTOR_B1	GPIO_PIN_17
#define MOTOR_B2	GPIO_PIN_27
#define MOTOR_B3	GPIO_PIN_22
#define MOTOR_B4	GPIO_PIN_5

#define __28BYJ_48_STEPS	2038
#define REV_PER_MICROSEC	(60*1000*1000)	/* Revolutions per microsecond */

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
	HAL_GPIO_Set_Mode(MOTOR_B1, GPIO_OUTPUT);
	HAL_GPIO_Set_Mode(MOTOR_B2, GPIO_OUTPUT);
	HAL_GPIO_Set_Mode(MOTOR_B3, GPIO_OUTPUT);
	HAL_GPIO_Set_Mode(MOTOR_B4, GPIO_OUTPUT);
	
	HAL_GPIO_Write(MOTOR_B1,GPIO_LOW);
	HAL_GPIO_Write(MOTOR_B2,GPIO_LOW);
	HAL_GPIO_Write(MOTOR_B3,GPIO_LOW);
	HAL_GPIO_Write(MOTOR_B4,GPIO_LOW);
	
	
	// *****************************
	// Main program
	// *****************************
	uint32_t rpm = 12; // 1 - 14
	// Retardo en microsegundos (velocidad de giro)
	uint32_t rpus = REV_PER_MICROSEC / __28BYJ_48_STEPS / rpm;
	while(1)
	{
		/* Secuencia para giro en el sentido}
		 * de las manecillas (derecha)
		 */
		// Estado S1
		HAL_GPIO_Write(MOTOR_B1,GPIO_HIGH);
		HAL_GPIO_Write(MOTOR_B2,GPIO_LOW);
		HAL_GPIO_Write(MOTOR_B3,GPIO_HIGH);
		HAL_GPIO_Write(MOTOR_B4,GPIO_LOW);
		delay_us(rpus);
		// Estado S2
		HAL_GPIO_Write(MOTOR_B1,GPIO_LOW);
		HAL_GPIO_Write(MOTOR_B2,GPIO_HIGH);
		HAL_GPIO_Write(MOTOR_B3,GPIO_HIGH);
		HAL_GPIO_Write(MOTOR_B4,GPIO_LOW);
		delay_us(rpus);
		// Estado S3
		HAL_GPIO_Write(MOTOR_B1,GPIO_LOW);
		HAL_GPIO_Write(MOTOR_B2,GPIO_HIGH);
		HAL_GPIO_Write(MOTOR_B3,GPIO_LOW);
		HAL_GPIO_Write(MOTOR_B4,GPIO_HIGH);
		delay_us(rpus);
		// Estado S4
		HAL_GPIO_Write(MOTOR_B1,GPIO_HIGH);
		HAL_GPIO_Write(MOTOR_B2,GPIO_LOW);
		HAL_GPIO_Write(MOTOR_B3,GPIO_LOW);
		HAL_GPIO_Write(MOTOR_B4,GPIO_HIGH);
		delay_us(rpus);
	}
	/* USER CODE END -------------------------------------------------------------------*/
	
	/* CLOSE PERIPHERAL'S MEMORY */
	HAL_memory_unmap();
	
	/* EXIT */
	return 0;
}

