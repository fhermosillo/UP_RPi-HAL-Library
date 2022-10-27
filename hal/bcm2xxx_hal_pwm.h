#ifndef __BCM2XXX_HAL_PWM_H
#define __BCM2XXX_HAL_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------*/
#include "bcm2xxx.h"
#include "bcm2xxx_hal_gpio.h"
#include "bcm2xxx_hal_cm.h"
#include "bcm2xxx_delay.h"
#include <stdlib.h>

/* Exported types ----------------------------------------------------*/
typedef enum
{
	PWM0_DRIVER = 0,
	PWM1_DRIVER			/*!< Not accessible by user */
}ePWMDrive;

typedef enum
{
	PWM_CHANNEL_0=0,
	PWM_CHANNEL_1
}ePWMChannel;

typedef enum
{
	PWM_MODE_BAL=0,
	PWM_MODE_MS
} ePWMMode;
typedef struct
{
	volatile uint32_t CTL;
	volatile uint32_t STA;
	volatile uint32_t DMAC;
			 uint32_t RESERVED0;
	volatile uint32_t RNG1;
	volatile uint32_t DAT1;
	volatile uint32_t FIF;
			 uint32_t RESERVED1;
	volatile uint32_t RNG2;
	volatile uint32_t DAT2;
	
} PWM_t;

/* Exported constants ------------------------------------------------*/
// PWM0A can be mapped to:
// * GPIO_PIN_12 via ALT0
// * GPIO_PIN_18 via ALT5
// PWM0B can be mapped to:
// * GPIO_PIN_13 via ALT0
// * GPIO_PIN_19 via ALT5

/* Exported macros ---------------------------------------------------*/

/* Exported variables ------------------------------------------------*/

/* Exported functions ------------------------------------------------*/
PWM_t *HAL_PWM_Init(ePWMDrive drive);
void HAL_PWM_DeInit(PWM_t *PWMx);

void HAL_PWM_Setup_Channel(PWM_t *PWMx, ePWMChannel channel, ePWMMode mode);

uint32_t HAL_PWM_Set_Frequency(PWM_t *PWMx, ePWMChannel channel, uint32_t freq);

void HAL_PWM_Set_Duty(PWM_t *PWMx, ePWMChannel channel, float duty);

void HAL_PWM_Start(PWM_t *PWMx, ePWMChannel channel);
void HAL_PWM_Stop(PWM_t *PWMx, ePWMChannel channel);

#ifdef __cplusplus
}
#endif


#endif /* __BCM2XXX_HAL_PWM_H */
