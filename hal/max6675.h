#ifndef __MAX6675_PI_H
#define __MAX6675_PI_H

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

/* Exported constants ------------------------------------------------*/
#define MAX6675_SAMPLING_MS	200
#define MAX6675_FAHRENHEIT(T) (((T)*1.8F)+32.0F)

/* Exported macros ---------------------------------------------------*/

/* Exported variables ------------------------------------------------*/

/* Exported functions ------------------------------------------------*/
void MAX6675_Begin(SPI_t *SPIx, eSPIChip chip);
float MAX6675_ReadTemp(void);

#ifdef __cplusplus
}
#endif


#endif /* __MAX6675_PI_H */
