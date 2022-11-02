#include "max6675.h"

#define MAX6675_DEVICE_ID	0x0001
#define MAX6675_T_OPEN		0x0004

// SPI Communication
static SPI_t *MAX6675_SPI = NULL;
static eSPIChip MAX6675_Chip = SPI_CHIP_0;

void MAX6675_Begin(SPI_t *SPIx, eSPIChip chip)
{
    MAX6675_SPI = SPIx;
    MAX6675_Chip = chip;
}

float MAX6675_ReadTemp(void)
{
    // Set SPI parameters
    HAL_SPI_Set_Chip(MAX6675_SPI, MAX6675_Chip, SPI_CHIP_POL_LOW);
    HAL_SPI_Set_Mode(MAX6675_SPI,SPI_MODE_1);
    
    // Read SPI data
    uint8_t rxbuf[2];
    HAL_SPI_Transfern(MAX6675_SPI, NULL, rxbuf, 2);
    
    // Make a 16-bit word
    uint16_t data = ((uint16_t)rxbuf[0] << 8) | (uint16_t)rxbuf[1];
    
    // Check for MAX6675 bit 2
    if(data & MAX6675_T_OPEN)
    {
	return -1.0;
    }
    
    // Convert to Celsius degrees
    return (float)(data >> 3)*0.25F;
}
