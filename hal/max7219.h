#ifndef __MAX7219_PI_H
#define __MAX7219_PI_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Sprite Generator:
 * https://xantorohara.github.io/led-matrix-editor/
 */
 
/* Includes ----------------------------------------------------------*/
#include "bcm2xxx.h"
#include "bcm2xxx_hal_gpio.h"
#include "bcm2xxx_hal_spi.h"
#include "bcm2xxx_delay.h"

/* Exported types ----------------------------------------------------*/
typedef enum
{
	MAX7219_DIR_LEFT = 1,
	MAX7219_DIR_RIGHT = -1,
}eMAX7219Dir;

/* Exported constants ------------------------------------------------*/
#define LED_OFF	GPIO_LOW
#define LED_ON	GPIO_HIGH

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

/* Exported macros ---------------------------------------------------*/

/* Exported variables ------------------------------------------------*/

/* Exported functions ------------------------------------------------*/
void MAX7219_Begin(SPI_t *SPIx, eSPIChip chip);

// Write a single led at row,col at state val
void MAX7219_Set_Led(uint8_t row, uint8_t col, eGPIOState val);

// Write a full row
void MAX7219_Write(uint8_t row, uint8_t data);

// Write to a reg
void MAX7219_WriteReg(uint8_t reg, uint8_t data);

// Clear the whole matrix
void MAX7219_Clear(void);

// Ascii
void MAX7219_PutChar(char c);

// Animation
void MAX7219_AnimateText(const char *msg, size_t mlen, uint32_t speed, eMAX7219Dir dir);
void MAX7219_AnimateData(const uint8_t data[][8], size_t frames, uint8_t spacing, uint32_t speed, eMAX7219Dir dir);

#ifdef __cplusplus
}
#endif


#endif /* __MAX7219_PI_H */
