#include "bcm2xxx_hal_pwm.h"
#include <stdlib.h>

// PWM Base
#define PWM0_BASE		0x0020C000U
#define PWM1_BASE		0x0020C800U

// PWM Registers
#define PWM_CTL_OFFSET 	0x00000000U
#define PWM_STA_OFFSET 	0x00000004U
#define PWM_DMAC_OFFSET 0x00000008U
#define PWM_RNG1_OFFSET 0x00000010U
#define PWM_DAT1_OFFSET 0x00000014U
#define PWM_FIF1_OFFSET 0x00000018U
#define PWM_RNG2_OFFSET 0x00000020U
#define PWM_DAT2_OFFSET 0x00000024U

#define PWM_CTL_PWM0EN	0x00000001U
#define PWM_CTL_PWM1EN	0x00000100U

#define PWM_CHANNEL_0_MASK	0x000000BF
#define PWM_CHANNEL_1_MASK	0xFFFFBFFF


PWM_t *HAL_PWM_Init(ePWMDrive drive)
{
	PWM_t *PWMx = (PWM_t *)malloc(sizeof(PWM_t));
	
	if(PWMx == NULL || HAL_get_peri_base() == MAP_FAILED)
	{
		return NULL;
	}
	
	switch(drive)
	{
		case PWM0_DRIVER:
			PWMx = (PWM_t *)(HAL_get_peri_base() + PWM0_BASE/4);
		break;
		
		case PWM1_DRIVER:
			PWMx = (PWM_t *)(HAL_get_peri_base() + PWM1_BASE/4);
			printf("Warning(@PWM1): Not accesible by user\n");
		break;
		
		default:
			return NULL;
		break;
	}
	
	
	
	// Reset Control Register
	PWMx->CTL = 0x00000000U;
	PWMx->RNG1 = 0x00000020U;
	PWMx->RNG2 = 0x00000020U;
	PWMx->DAT1 = 0x00000000U;
	PWMx->DAT2 = 0x00000000U;
	PWMx->STA =  0x00000FFFU;
	PWMx->DMAC = 0x00000707U;
	
	return PWMx;
}

void HAL_PWM_DeInit(PWM_t *PWMx)
{
	if(PWMx != NULL)
	{
		free(PWMx);
	}
}

void HAL_PWM_Setup_Channel(PWM_t *PWMx, ePWMChannel channel)
{
	// Reset channel configuration and setup channel configuration
	// Output polarity 0=low -> 1:high
	// PWM Mode
	// Data register is transmitted
	// PWM algorithm is used
	if(channel == PWM_CHANNEL_0)
	{
		PWMx->CTL &= ~PWM_CHANNEL_0_MASK;
	}
	else
	{
		PWMx->CTL &= ~PWM_CHANNEL_1_MASK;
	}
}

uint32_t HAL_PWM_Set_Frequency(PWM_t *PWMx, ePWMChannel channel, uint32_t freq)
{
	// Stop PWM
	HAL_PWM_Stop(PWMx, channel);
	
	// Setup PWM Clock Manager
	CM_t *CM_PWM = HAL_CM_Init(CM_PWM_DRIVER);
	HAL_CM_Set_Clock(CM_PWM, CM_SOURCE_PLLD);
	HAL_CM_Set_Freq(CM_PWM,20000000);
	HAL_CM_Start(CM_PWM);

	// Read frequency
	uint32_t fsrc = HAL_CM_Get_Freq(CM_PWM);	// PWM clock frequency
	uint32_t rngx = fsrc/freq;		// Range register value
	uint32_t fact = fsrc/rngx;	// Actual frequency	
	
	// Setup RANGE & Duty Cycle
	int bittmp;
	if(channel == PWM_CHANNEL_0)
	{
		// Set frequency and duty cycle
		PWMx->RNG1 = rngx;
		PWMx->DAT1 = 0;
		
		bittmp = 9;
	} else{
		PWMx->RNG2 = rngx;
		PWMx->DAT2 = 0;
		
		bittmp = 10;
	}
	
	if ( !bitcheck(PWMx->STA,bittmp) ) {
		if ( bitcheck(PWMx->STA,3) ) bitset(PWMx->STA,3); // Clear RERR bit if read occured on empty FIFO while channel was transmitting
		if ( bitcheck(PWMx->STA,2) ) bitset(PWMx->STA,2); // Clear WERR bit if write occured on full FIFO while channel was transmitting
		if ( bitcheck(PWMx->STA,8) )  bitset(PWMx->STA,8); // Clear BERR bit if write to registers via APB occured while channel was transmitting
	}
	delay_us(100);
	    
	// Start PWM
	HAL_PWM_Start(PWMx, channel);
	
	// Return actual frequency
	return fact;
}

void HAL_PWM_Set_Duty(PWM_t *PWMx, ePWMChannel channel, float duty)
{
	if(duty > 100.0F || duty < 0.0F)
		return;
	
	HAL_PWM_Stop(PWMx,channel);
	
	float dc = duty/100.0F;
	if(channel == PWM_CHANNEL_0)
	{
		PWMx->DAT1 = (uint32_t)(PWMx->RNG1 * dc);
	} else
	{
		PWMx->DAT2 = (uint32_t)(PWMx->RNG2 * dc);
	}
	
	HAL_PWM_Start(PWMx,channel);
}

void HAL_PWM_Start(PWM_t *PWMx, ePWMChannel channel)
{
	if(channel == PWM_CHANNEL_0)
		bitset(PWMx->CTL,0);
	else
		bitset(PWMx->CTL,8);
}

void HAL_PWM_Stop(PWM_t *PWMx, ePWMChannel channel)
{
	if(channel == PWM_CHANNEL_0)
		bitclr(PWMx->CTL,0);
	else
		bitclr(PWMx->CTL,8);
}
