#ifndef __MAX7219_PI_H
#define __MAX7219_PI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------*/
#include "bcm2xxx.h"
#include "bcm2xxx_hal_gpio.h"
#include "bcm2xxx_hal_spi.h"

/* Exported types ----------------------------------------------------*/

/* Exported constants ------------------------------------------------*/
#define LED_OFF	GPIO_LOW
#define LED_ON	GPIO_HIGH

/* Exported macros ---------------------------------------------------*/

/* Exported variables ------------------------------------------------*/

/* Exported functions ------------------------------------------------*/
void MAX7219_Setup(void);

// Write a single led at row,col at state val
void MAX7219_Set_Led(uint8_t row, uint8_t col, eGPIOState val);

// Write a full row
void MAX7219_Write(uint8_t row, uint8_t data);

// Write to a reg
void MAX7219_WriteReg(uint8_t reg, uint8_t data);

// Clear the whole matrix
void MAX7219_Clear(void);

#ifdef __cplusplus
}
#endif


#endif /* __MAX7219_PI_H */
