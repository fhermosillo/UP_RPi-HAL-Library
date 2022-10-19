#include "bcm2xxx_delay.h"
#include "bcm2xxx_hal_gpio.h"
#include "bcm2xxx_hal_spi.h"
#include "bcm2xxx_hal_pwm.h"

#define LED_PIN		GPIO_PIN_5
#define SPI0_MOSI	GPIO_PIN_10
#define SPI0_MISO	GPIO_PIN_9
#define SPI0_SCLK	GPIO_PIN_11
#define SPI0_CS0	GPIO_PIN_8
#define SPI0_CS1	GPIO_PIN_7
#define PWM0_CH0	GPIO_PIN_12

#define MAX7219_NOP_REG			0x00
#define MAX7219_DIG0_REG		0x01
#define MAX7219_DIG1_REG		0x02
#define MAX7219_DIG2_REG		0x03
#define MAX7219_DIG3_REG		0x04
#define MAX7219_DIG4_REG		0x05
#define MAX7219_DIG5_REG		0x06
#define MAX7219_DIG6_REG		0x07
#define MAX7219_DIG7_REG		0x08
#define MAX7219_DECMODE_REG		0x09
#define MAX7219_INT_REG			0x0A
#define MAX7219_SCANLIM_REG		0x0B
#define MAX7219_SHUTDOWN_REG	0x0C
#define MAX7219_DSPLTEST_REG	0x0F
       
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
	HAL_GPIO_Set_Mode(SPI0_MOSI, GPIO_FUNCTION_0);
	HAL_GPIO_Set_Mode(SPI0_MISO, GPIO_FUNCTION_0);
	HAL_GPIO_Set_Mode(SPI0_SCLK, GPIO_FUNCTION_0);
	HAL_GPIO_Set_Mode(SPI0_CS0, GPIO_FUNCTION_0);
	HAL_GPIO_Set_Mode(SPI0_CS1, GPIO_FUNCTION_0);
	HAL_GPIO_Set_Mode(PWM0_CH0, GPIO_FUNCTION_0);
	HAL_GPIO_Set_Mode(LED_PIN, GPIO_OUTPUT);
	
	/* SPI Config */
	SPI_t *SPI0=HAL_SPI_Init(SPI0_DRIVER);
	HAL_SPI_Set_Mode(SPI0,SPI_MODE_0);
	HAL_SPI_Set_Baud(SPI0, MHZ(1));
	HAL_SPI_Set_Chip(SPI0, SPI_CHIP_0, SPI_CHIP_POL_LOW);	
	
	// *****************************
	// Main program
	// *****************************
	/* LED OFF */
	HAL_GPIO_Write(LED_PIN, GPIO_LOW);
	
	/* MAX7219 Initialization */
	uint8_t txbuf[2];
	// Shutdown mode
	txbuf[0]=MAX7219_SHUTDOWN_REG;
	txbuf[1]=0;
	HAL_SPI_Transfern(SPI0,txbuf,NULL,2);
	
	// show 8 digits
	txbuf[0]=MAX7219_SCANLIM_REG;
	txbuf[1]=7;
	HAL_SPI_Transfern(SPI0,txbuf,NULL,2);
	
	// use bit patterns
	txbuf[0]=MAX7219_DECMODE_REG;
	txbuf[1]=0;
	HAL_SPI_Transfern(SPI0,txbuf,NULL,2);
	
	// no display test
	txbuf[0]=MAX7219_DSPLTEST_REG;
	txbuf[1]=0;
	HAL_SPI_Transfern(SPI0,txbuf,NULL,2);
	
	// character intensity: range: 0 to 15
	txbuf[0]=MAX7219_INT_REG;
	txbuf[1]=4;
	HAL_SPI_Transfern(SPI0,txbuf,NULL,2);
	
	// clear display
	for(int i = 0; i < 8; i++)
	{
		txbuf[0]= MAX7219_DIG0_REG + i;
		txbuf[1]=0;
		HAL_SPI_Transfern(SPI0,txbuf,NULL,2);
	}
	
	// Set to normal mode
	txbuf[0]=MAX7219_SHUTDOWN_REG;
	txbuf[1]=1;
	HAL_SPI_Transfern(SPI0,txbuf,NULL,2);
	
	/* Main Loop */
	while(1)
	{
		/* Snake animation left to right */
		txbuf[0]=MAX7219_DIG0_REG;
		for(int i = 0; i < 6; i++)
		{
			txbuf[1] = 7 << i;
			HAL_SPI_Transfern(SPI0,txbuf,NULL,2);
			delay_ms(250);
		}
		
		/* Snake animation right to left */
		txbuf[0]=MAX7219_DIG0_REG;
		for(int i = 1; i < 6; i++)
		{
			txbuf[1] = 0xD0 >> i;
			HAL_SPI_Transfern(SPI0,txbuf,NULL,2);
			delay_ms(250);
		}
	}
	/* USER CODE END -------------------------------------------------------------------*/
	
	/* CLOSE PERIPHERAL'S MEMORY */
	HAL_memory_unmap();
	
	/* EXIT */
	return 0;
}

