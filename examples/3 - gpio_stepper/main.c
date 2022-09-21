#include "bcm2xxx_delay.h"
#include "bcm2xxx_hal_gpio.h"

#define MOTOR_S1	GPIO_PIN_17
#define MOTOR_S2	GPIO_PIN_27
#define MOTOR_S3	GPIO_PIN_22
#define MOTOR_S4	GPIO_PIN_5

#define __28BYJ_48_STEPS	2038
#define REV_PER_MICROSEC	(60*1000*1000)	/* Revolutions per microsecond */

int main()
{
	/* OPEN PERIPHERAL'S MEMORY */
	HAL_memory_map(RPI3_MODEL_B);
	
	/* USER CODE BEGIN -------------------------------------------------------------------*/
	// Peripheral Configurations
	HAL_GPIO_Init();
	
	HAL_GPIO_Set_Mode(MOTOR_S1, GPIO_OUTPUT);
	HAL_GPIO_Set_Mode(MOTOR_S2, GPIO_OUTPUT);
	HAL_GPIO_Set_Mode(MOTOR_S3, GPIO_OUTPUT);
	HAL_GPIO_Set_Mode(MOTOR_S4, GPIO_OUTPUT);
	
	uint32_t rpm = 12; // Max 14
	uint32_t us = REV_PER_MICROSEC / __28BYJ_48_STEPS / rpm;
	HAL_GPIO_Write(MOTOR_S1,GPIO_LOW);
	HAL_GPIO_Write(MOTOR_S2,GPIO_LOW);
	HAL_GPIO_Write(MOTOR_S3,GPIO_LOW);
	HAL_GPIO_Write(MOTOR_S4,GPIO_LOW);
	
	uint32_t rpm = 0;
	printf("Revoluciones por minuto (1 - 14): ");
	scanf("%d",&rpm);
	if(rpm > 14)
		rpm = 14;
		
	// Retardo en microsegundos (velocidad de giro)
	uint32_t rpus = REV_PER_MICROSEC / __28BYJ_48_STEPS / rpm;
	
	// Main program
	while(1)
	{
		/* Secuencia para giro en el sentido}
		 * de las manecillas (derecha)
		 */
		// Estado #1
		HAL_GPIO_Write(MOTOR_S1,GPIO_HIGH);
		HAL_GPIO_Write(MOTOR_S2,GPIO_LOW);
		HAL_GPIO_Write(MOTOR_S3,GPIO_HIGH);
		HAL_GPIO_Write(MOTOR_S4,GPIO_LOW);
		delay_us(rpus);
		// Estado #2
		HAL_GPIO_Write(MOTOR_S1,GPIO_LOW);
		HAL_GPIO_Write(MOTOR_S2,GPIO_HIGH);
		HAL_GPIO_Write(MOTOR_S3,GPIO_HIGH);
		HAL_GPIO_Write(MOTOR_S4,GPIO_LOW);
		delay_us(rpus);
		// Estado #3
		HAL_GPIO_Write(MOTOR_S1,GPIO_LOW);
		HAL_GPIO_Write(MOTOR_S2,GPIO_HIGH);
		HAL_GPIO_Write(MOTOR_S3,GPIO_LOW);
		HAL_GPIO_Write(MOTOR_S4,GPIO_HIGH);
		delay_us(rpus);
		// Estado #4
		HAL_GPIO_Write(MOTOR_S1,GPIO_HIGH);
		HAL_GPIO_Write(MOTOR_S2,GPIO_LOW);
		HAL_GPIO_Write(MOTOR_S3,GPIO_LOW);
		HAL_GPIO_Write(MOTOR_S4,GPIO_HIGH);
		delay_us(rpus);
	}
	/* USER CODE END -------------------------------------------------------------------*/
	
	/* CLOSE PERIPHERAL'S MEMORY */
	HAL_memory_unmap();
	
	/* EXIT */
	return 0;
}

