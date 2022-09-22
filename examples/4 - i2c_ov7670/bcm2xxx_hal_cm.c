#include "bcm2xxx_hal_cm.h"
#include <stdlib.h>


#define CM_GPIO0_BASE	0x00101070
#define CM_GPIO1_BASE	0x00101078
#define CM_GPIO2_BASE	0x00101080

#define CM_GP0CTL	0x00101070
#define CM_GP0DIV	0x00101074
#define CM_GP1CTL	0x00101078
#define CM_GP1DIV	0x0010107C
#define CM_GP2CTL	0x00101080
#define CM_GP2DIV	0x00101084

#define CM_PASSWORD (0x5A << 24)


// GPCLK0,1,2 functions - GPIO4,5,6
/*
 * 0     0 Hz     Ground
 * 1     19.2 MHz oscillator
 * 2     0 Hz     testdebug0
 * 3     0 Hz     testdebug1
 * 4     0 Hz     PLLA
 * 5     1000 MHz PLLC (changes with overclock settings)
 * 6     500 MHz  PLLD*
 * 7     216 MHz  HDMI auxiliary
 * 8-15  0 Hz     Ground
 */
#define CM_OSC_FREQUENCY	19200000
#define CM_PLLC_FREQUENCY	1000000000
#define CM_PLLD_FREQUENCY	500000000
#define CM_HDMI_FREQUENCY	216000000


CM_GPIO_t HAL_CM_Init(eCMDrive drive)
{
	CM_GPIO_t *CM_GPIOx = (CM_GPIO_t *)malloc(sizeof(CM_GPIO_t));
	if(CM_GPIOx != NULL || HAL_get_peri_base() == MAP_FAILED)
	{
		return NULL;
	}
	
	switch(drive)
	{
		case CM_DRIVER_GPIO0:
			CM_GPIOx = (CM_GPIO_t *)(HAL_get_peri_base() + CM_GPIO0_BASE/4);
		break;
		
		case CM_DRIVER_GPIO1:
			CM_GPIOx = (CM_GPIO_t *)(HAL_get_peri_base() + CM_GPIO1_BASE/4);
		break;
		
		case CM_DRIVER_GPIO2:
			CM_GPIOx = (CM_GPIO_t *)(HAL_get_peri_base() + CM_GPIO2_BASE/4);
		break;
		
		default:
			return NULL;
		break;
	}
	
	HAL_CM_Stop(CM_GPIOx);
	
	return 0;

}

uint32_t HAL_CM_Set_ClkSrc(CM_GPIO_t * CM_GPIOx, eCMClkSource src)
{
	CM_GPIOx->GPCTL = CM_PASSWORD | (uint32_t)src;
	usleep(10);
	return HAL_CM_Get_Freq(CM_GPIOx);
}

void HAL_CM_Stop(CM_GPIO_t * CM_GPIOx)
{
	CM_GPIOx->GPCTL = CM_PASSWORD | (1 << 5);		// Kill clock
	while(bitcheck(CM_GPIOx->GPCTL,7));					// Wait until clock stop
}

void HAL_CM_Set_Freq(CM_GPIO_t * CM_GPIOx, uint32_t freq)
{
	uint32_t divisor = HAL_CM_Get_Freq(CM_GPIOx)/freq;
	if(divisor > CM_GPIO_MAXDIV)
		divisor = CM_GPIO_MAXDIV;
	else if(divisor < CM_GPIO_MINDIV)
		divisor = CM_GPIO_MINDIV;
	
	CM_GPIOx->GPDIV = CM_PASSWORD | (divisor << 12);
	usleep(10);
}

void HAL_CM_Start(CM_GPIO_t * CM_GPIOx)
{
	CM_GPIOx->GPCTL |= CM_PASSWORD | (1 << 4);
}

uint32_t HAL_CM_Get_Freq(CM_GPIO_t *CM_GPIOx)
{
	switch(CM_GPIOx->GPCTL & 0x0000000F)
	{	
		case CM_SOURCE_OSC:
			return CM_OSC_FREQUENCY;
		break;
		
		case CM_SOURCE_DBG0:
			return 0;
		break;
		
		case CM_SOURCE_DBG1:
			return 0;
		break;
		
		case CM_SOURCE_PLLA:
			return 0;
		break;
		
		case CM_SOURCE_PLLC:
			return CM_PLLC_FREQUENCY;
		break;
		
		case CM_SOURCE_PLLD:
			return CM_PLLD_FREQUENCY;
		break;
		
		case CM_SOURCE_HDMI:
			return CM_HDMI_FREQUENCY;
		break;
		
		default:
			return 0;
		break;
	}
}
