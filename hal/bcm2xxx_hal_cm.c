#include "bcm2xxx_hal_cm.h"
#include <stdlib.h>

#define CLOCK_BASE		0x00101000U
#define CM_GPIO0_BASE	(CLOCK_BASE + 0x00000070U)
#define CM_GPIO1_BASE	(CLOCK_BASE + 0x00000078U)
#define CM_GPIO2_BASE	(CLOCK_BASE + 0x00000080U)
#define CM_PWM_BASE		(CLOCK_BASE + 0x000000A0U)
#define CM_PCM_BASE		(CLOCK_BASE + 0x00000098U)
#define CM_UART_BASE	(CLOCK_BASE + 0x000000F0U)
#define CM_TIMER_BASE	(CLOCK_BASE + 0x000000E8U)

#define CM_CTL 	0x00000000U
#define CM_DIV 	0x00000004U

#define CM_CTL_DISABLE	0x0000060FU
#define CM_CTL_ENABLE	0x00000010U
#define CM_CTL_BUSY		0x00000080U
#define CM_CTL_PASSWD	0x5A000000U
#define CM_CTL_MASH		0x00000600U
#define CM_CTL_MASH_0	0x00000000U
#define CM_CTL_MASH_1	0x00000200U
#define CM_CTL_MASH_2	0x00000400U
#define CM_CTL_MASH_3	0x00000600U

#define CM_DIV_DIVI		0x00FFF000U
#define CM_DIV_DIVF		0x00000FFFU





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

#define CM_DIVMAX	4095
#define CM_DIVMIN	1

static uint32_t CM_Bus_Freq(CM_t *CMx);

CM_t *HAL_CM_Init(eCMDriver drive)
{
	CM_t *CMx = (CM_t *)malloc(sizeof(CM_t));
	if(CMx == NULL || HAL_get_peri_base() == MAP_FAILED)
	{
		return NULL;
	}
	
	switch(drive)
	{
		case CM_GPIO0_DRIVER:
			CMx = (CM_t *)(HAL_get_peri_base() + CM_GPIO0_BASE/4);
		break;
		
		case CM_GPIO1_DRIVER:
			CMx = (CM_t *)(HAL_get_peri_base() + CM_GPIO1_BASE/4);
		break;
		
		case CM_GPIO2_DRIVER:
			CMx = (CM_t *)(HAL_get_peri_base() + CM_GPIO2_BASE/4);
		break;
		
		case CM_PWM_DRIVER:
			CMx = (CM_t *)(HAL_get_peri_base() + CM_PWM_BASE/4);
		break;
		
		case CM_PCM_DRIVER:
			CMx = (CM_t *)(HAL_get_peri_base() + CM_PCM_BASE/4);
		break;
		
		case CM_UART_DRIVER:
			CMx = (CM_t *)(HAL_get_peri_base() + CM_UART_BASE/4);
		break;
		
		case CM_TIMER_DRIVER:
			CMx = (CM_t *)(HAL_get_peri_base() + CM_TIMER_BASE/4);
		break;
		
		default:
			return NULL;
		break;
	}
	
	//HAL_CM_Stop(CMx);
	
	return CMx;

}

void HAL_CM_DeInit(CM_t *CMx)
{
	if(CMx != NULL)
	{
		free(CMx);
	}
}

void HAL_CM_Stop(CM_t * CMx)
{
	// Disable clock
	uint32_t cm_disable = CMx->CTL & CM_CTL_DISABLE;
	CMx->CTL = CM_CTL_PASSWD | cm_disable;
	
	// Wait until clock will be ready
	while(CMx->CTL & CM_CTL_BUSY);
}

void HAL_CM_Start(CM_t * CMx)
{
	CMx->CTL |= CM_CTL_PASSWD | CM_CTL_ENABLE;
}

uint32_t HAL_CM_Set_Clock(CM_t * CMx, eCMClock clk)
{
	// Stop Clock
	HAL_CM_Stop(CMx);
	
	// Set new clock source
	CMx->CTL = CM_CTL_PASSWD | (uint32_t)clk;
	
	// Start clock
	HAL_CM_Start(CMx);
	
	return CM_Bus_Freq(CMx);
}

uint32_t HAL_CM_Set_Freq(CM_t * CMx, uint32_t freq)
{
	uint32_t freq_src = CM_Bus_Freq(CMx);
	uint32_t cdiv = freq_src/freq;
	
	cdiv = MAX(cdiv,CM_DIVMIN);
	cdiv = MIN(cdiv,CM_DIVMAX);
	
	// Stop Clock
	HAL_CM_Stop(CMx);
	
	// Setup clock frequency (MASH-0)
	CMx->DIV = CM_CTL_PASSWD | (cdiv << 12);
	
	// Start clock
	HAL_CM_Start(CMx);
	
	return freq_src/cdiv;
}

uint32_t HAL_CM_Get_Freq(CM_t *CMx)
{
	uint32_t fsrc = CM_Bus_Freq(CMx);
	
	uint32_t MASH = (CMx->CTL & CM_CTL_MASH);
	uint32_t DIVI = (CMx->DIV & CM_DIV_DIVI) >> 12;
	uint32_t DIVF = (CMx->DIV & CM_DIV_DIVF);
	
	uint32_t f = 0;
	switch(MASH)
	{
		case CM_CTL_MASH_0:
			f = fsrc/DIVI;
		break;
		
		case CM_CTL_MASH_1:
		case CM_CTL_MASH_2:
		case CM_CTL_MASH_3:
			f = (uint32_t)((float)(fsrc)/((float)(DIVI) + (float)(DIVF)/1024.0F));
		break;
	}
	
	return f;
}


static uint32_t CM_Bus_Freq(CM_t *CMx)
{
	switch(CMx->CTL & 0x0000000F)
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
