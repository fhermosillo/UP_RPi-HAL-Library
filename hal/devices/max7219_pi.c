#include "max7219_pi.h"

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


static SPI_t *SPI0;

// Array to store matrix state
static uint8_t matrix_array[8];


void MAX7219_Setup(void)
{
	/* GPIO Config */
	HAL_GPIO_Set_Mode(GPIO_PIN_10, GPIO_FUNCTION_0); // MOSI
	// MISO
	// SCLK
	// CS0
	// CS1
	
	/* Setup SPI at 1MHZ and MODE0 */
	
	/* MAX7219 Config */
	// Set Shutdown mode on register 0x0C
	
	// Enable 8 digits on register 0x0B
	
	// No decode for digits on register 0x09
	
	// No display test on register 0x0F
	
	// Choose the LED intensity (0 - 15) on register 0x0A

	/* Clear the whole matrix */
	MAX7219_Clear();
	
	/* Set Normal Mode on register 0x0C */
	
}

void MAX7219_Set_Led(uint8_t row, uint8_t col, eGPIOState val)
{
	if(row > 7 || col > 7)
	{
		return;
	}
	
	if(val)
		bitset(matrix_array[row],col);
	else
		bitclr(matrix_array[row],col);
	
	MAX7219_Write(row, matrix_array[row]);
}

void MAX7219_Write(uint8_t row, uint8_t data)
{
	if(row > 7)
	{
		return;
	}
	matrix_array[row] = data;
	
	// Transfer data with SPI
	MAX7219_WriteReg(MAX7219_DIG0_REG+row,data);
}

// Write to a reg
void MAX7219_WriteReg(uint8_t reg, uint8_t data)
{
	uint8_t txbuf[2] = {reg,data}; // Write to ith row register
	HAL_SPI_Transfern(SPI0,txbuf,NULL,2);
}

void MAX7219_Clear(void)
{
	for(int i = 0; i < 8; i++)
	{
		matrix_array[i] = 0x00;
		MAX7219_WriteReg(MAX7219_DIG0_REG+i, matrix_array[i]);
	}
}