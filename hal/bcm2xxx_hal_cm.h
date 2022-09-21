#ifndef __BCM2XXX_HAL_CM_H
#define __BCM2XXX_HAL_CM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------*/
#include "bcm2xxx.h"

/* Exported types ----------------------------------------------------*/
typedef enum
{
	CM_SOURCE_GND = 0,
	CM_SOURCE_OSC,
	CM_SOURCE_DBG0,
	CM_SOURCE_DBG1,
	CM_SOURCE_PLLA,
	CM_SOURCE_PLLC,
	CM_SOURCE_PLLD,
	CM_SOURCE_HDMI
}eCMClkSource;

typedef enum
{
	CM_DRIVER_GPIO0 = 0,
	CM_DRIVER_GPIO1,
	CM_DRIVER_GPIO2
}eCMDrive;

typedef struct
{
	volatile uint32_t GPCTL;
	volatile uint32_t GPDIV;
} CM_GPIO_t;

/* Exported constants ------------------------------------------------*/
#define CM_GPIO_MAXDIV	40995
#define CM_GPIO_MINDIV	1


/* Exported macros ---------------------------------------------------*/

/* Exported variables ------------------------------------------------*/
extern CM_GPIO_t *CM_GPIO0;
extern CM_GPIO_t *CM_GPIO1;
extern CM_GPIO_t *CM_GPIO2;

/* Exported functions ------------------------------------------------*/
int HAL_CM_Init(eCMDrive drive);

uint32_t HAL_CM_Set_ClkSrc(CM_GPIO_t *CM_GPIOx, eCMClkSource src);

void HAL_CM_Stop(CM_GPIO_t *CM_GPIOx);

void HAL_CM_Set_Freq(CM_GPIO_t *CM_GPIOx, uint32_t freq);

void HAL_CM_Start(CM_GPIO_t *CM_GPIOx);

uint32_t HAL_CM_Get_Freq(CM_GPIO_t *CM_GPIOx);

#ifdef __cplusplus
}
#endif


#endif /* __BCM2XXX_HAL_CM_H */
