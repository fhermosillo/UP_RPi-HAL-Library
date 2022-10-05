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
	CM_SOURCE_GND = 0,	/*!< Freq = 0Hz */
	CM_SOURCE_OSC,		/*!< Freq = 19,200,000 Hz */
	CM_SOURCE_DBG0,		/*!< Freq = Hz */
	CM_SOURCE_DBG1,		/*!< Freq = Hz */
	CM_SOURCE_PLLA,		/*!< Freq = Hz */
	CM_SOURCE_PLLC,		/*!< Freq = 1,000,000,000 Hz */
	CM_SOURCE_PLLD,		/*!< Freq = 500,000,000 Hz */
	CM_SOURCE_HDMI		/*!< Freq = 216,000,000 Hz */
} eCMClock;

typedef enum
{
	CM_GPIO0_CLOCK = 0,
	CM_GPIO1_CLOCL,
	CM_GPIO2_CLOCK,
	CM_PWM_CLOCK,
	CM_PCM_CLOCK,
	CM_UART_CLOCK,
	CM_TIMER_CLOCK
} eCMDriver;

typedef struct
{
	volatile uint32_t CTL;
	volatile uint32_t DIV;
} CM_t;

/* Exported constants ------------------------------------------------*/

/* Exported macros ---------------------------------------------------*/

/* Exported variables ------------------------------------------------*/

/* Exported functions ------------------------------------------------*/
CM_t *HAL_CM_Init(eCMDriver drive);

void HAL_CM_DeInit(CM_t *CMx);

uint32_t HAL_CM_Set_Clock(CM_t *CMx, eCMClock clk);

void HAL_CM_Stop(CM_t *CMx);

uint32_t HAL_CM_Set_Freq(CM_t *CMx, uint32_t freq);

void HAL_CM_Start(CM_t *CMx);

uint32_t HAL_CM_Get_Freq(CM_t *CMx);

#ifdef __cplusplus
}
#endif


#endif /* __BCM2XXX_HAL_CM_H */
