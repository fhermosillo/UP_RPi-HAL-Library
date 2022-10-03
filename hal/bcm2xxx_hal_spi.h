/*
 *
 * @example for ADXL345
 *
 * #define POWER_CTL 0x2D
 * .
 * .
 * .
 *
 * int main()
 * { 
 * .
 * .
 * .
 * SPI_t *SPI0=HAL_SPI_Init(SPI0_DRIVER);
 * HAL_SPI_Set_Mode(SPI0,SPI_MODE_3);	// CPOL=CPH=HIGH
 * HAL_SPI_Set_Baud(SPI0, MHZ(3));		// Setup to 3MHz
 * HAL_SPI_Set_Chip(SPI0, SPI_CHIP_0, SPI_CHIP_POL_LOW);
 *
 *  // Write transfer
 *  // Write POWER_CTL = 0x00
 *  uint8_t txbuf[2] = {POWER_CTL, 0x00};
 * 	HAL_SPI_Transfern(SPI0,txbuf,2);
 *  // Write POWER_CTL = 0x10
 *  txbuf[1] = 0x10;
 * 	HAL_SPI_Transfern(SPI0,txbuf,2);
 *  // Write POWER_CTL = 0x08
 *  txbuf[1] = 0x08;
 * 	HAL_SPI_Transfern(SPI0,txbuf,2);
 *
 * // Read Transfer
 * 	uint8_t devid = HAL_SPI_Transfer(SPI0,0x00);
 *  printf("DEVICE ID: 0x%02X\n",devid);
 *
 * .
 * .
 * .
 *
 * return 0;
 * }
 */
#ifndef __BCM2XXX_HAL_SPI_H
#define __BCM2XXX_HAL_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------*/
#include "bcm2xxx.h"
#include "bcm2xxx_hal_gpio.h"

/* Exported types ----------------------------------------------------*/
typedef enum {
	SPI0_DRIVER = 0,	/* Only two CS Pines */
	SPI3_DRIVER,		/* Only RPI4 */
	SPI4_DRIVER,		/* Only RPI4 */
	SPI5_DRIVER,		/* Only RPI4 */
	SPI6_DRIVER			/* Only RPI4 */
} eSPIDriver;

// SCL = core_clock / CDIV

typedef enum
{
	SPI_ERROR_NONE = 0
} eSPIError;

typedef enum
{
	SPI_MODE_0=0,	/*!< CPOL=LOW, CPH=LOW */
	SPI_MODE_1,		/*!< CPOL=LOW, CPH=HIGH */
	SPI_MODE_2,		/*!< CPOL=HIGH, CPH=LOW */
	SPI_MODE_3,		/*!< CPOL=HIGH, CPH=HIGH */
}eSPIMode;

typedef enum 
{
	SPI_CHIP_0 = 0,
	SPI_CHIP_1,
	SPI_CHIP_2
} eSPIChip;

typedef enum
{
	SPI_CHIP_POL_LOW=0,
	SPI_CHIP_POL_HIGH
} eSPIChipPol;

typedef struct
{
	volatile uint32_t CS;
	volatile uint32_t FIFO;
	volatile uint32_t CLK;
	volatile uint32_t DLEN;
	volatile uint32_t LTOH;
	volatile uint32_t DC;
} SPI_t;

/* Exported constants ------------------------------------------------*/
#define SPI_FIFO_SIZE	16
#define SPI_ORDER_MSBFIRST
#define SPI_MAX_SPEED MHZ(50)

/* Exported macros ---------------------------------------------------*/

/* Exported variables ------------------------------------------------*/

/* Exported functions ------------------------------------------------*/

// Initialization functions
SPI_t *HAL_SPI_Init(eSPIDriver nSPIx);

// Control functions
void HAL_SPI_Set_Mode(SPI_t *SPIx, eSPIMode mode);
void HAL_SPI_Set_Chip(SPI_t *SPIx, eSPIChip chip, eSPIChipPol chip_pol);
void HAL_SPI_Set_Baud(SPI_t *SPIx, uint32_t baudios);


// Read & Write functions
uint8_t HAL_SPI_Transfer(SPI_t *SPIx, uint8_t data);
void HAL_SPI_Transfern(SPI_t *SPIx, uint8_t *txbuf, uint8_t *rxbuf, size_t num_bytes);


#ifdef __cplusplus
}
#endif


#endif /* __BCM2XXX_HAL_SPI_H */
