#ifndef __SOFTSPI_H
#define __SOFTSPI_H

#ifdef __cplusplus
extern "C" {
#endif

 
/* Includes ----------------------------------------------------------*/
#include "bcm2xxx.h"
#include "bcm2xxx_hal_gpio.h"

/* Exported types ----------------------------------------------------*/
typedef enum
{
	SOFTSPI_LSBFIRST=0,
	SOFTSPI_MSBFIRST
}eSoftSPIBitOrder;

typedef enum
{
	SOFTSPI_MODE0=0,
	SOFTSPI_MODE1,
	SOFTSPI_MODE2,
	SOFTSPI_MODE3
}eSoftSPIMode;

typedef struct
{
	// Pinout
	uint8_t miso;
	uint8_t mosi;
	uint8_t scl;
	
	// Protocol
	eGPIOState cpha;
	eGPIOState cpol;
	eSoftSPIBitOrder bitOrder;
	uint8_t numBits;
	
	// Timing
	uint32_t delay;
	
	// Buffer
	uint8_t buffer[32];
	
	//
} SoftSPI_t;

/* Exported constants ------------------------------------------------*/

/* Exported macros ---------------------------------------------------*/

/* Exported variables ------------------------------------------------*/

/* Exported functions ------------------------------------------------*/
void SoftSPI(SoftSPI_t *dev, uint8_t mosi, uint8_t miso, uint8_t sck);

// GPIO Init/Deinit
void SoftSPI_Begin(SoftSPI_t *dev);
void SoftSPI_End(SoftSPI_t *dev);

// Configure
void SoftSPI_SetNumBits(SoftSPI_t *dev, uint8_t numbits);
void SoftSPI_SetMode(SoftSPI_t *dev, eSoftSPIMode mode);
void SoftSPI_SetBitOrder(SoftSPI_t *dev, eSoftSPIBitOrder order);
void SoftSPI_SetClock(SoftSPI_t *dev, uint32_t us);

// Transfer data
uint32_t SoftSPI_Transfer(SoftSPI_t *dev, uint32_t val);

// Chip Select/Deselect
void SoftSPI_ChipSelect(uint8_t cs,   unsigned int ts, eGPIOState pol);

#ifdef __cplusplus
}
#endif


#endif /* __DMARD03_H */
