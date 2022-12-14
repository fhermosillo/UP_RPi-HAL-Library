#include "bcm2xxx_hal_gpio.h"
#include <stdlib.h>

/* Private constants -------------------------------------------------*/
#define GPIO_BASE		0x00200000U
#define GPIO_GPFSEL0	0x00000000U
#define GPIO_GPFSEL1	0x00000004U
#define GPIO_GPFSEL2	0x00000008U
#define GPIO_GPFSEL3	0x0000000CU
#define GPIO_GPFSEL4	0x00000010U
#define GPIO_GPFSEL5	0x00000014U
#define GPIO_GPSET0		0x0000001CU
#define GPIO_GPSET1		0x00000020U
#define GPIO_GPCLR0		0x00000028U
#define GPIO_GPCLR1		0x0000002CU
#define GPIO_GPLEV0		0x00000034U
#define GPIO_GPLEV1		0x00000038U
#define GPIO_GPEDS0		0x00000040U
#define GPIO_GPEDS1		0x00000044U
#define GPIO_GPREN0		0x0000004CU
#define GPIO_GPREN1		0x00000050U
#define GPIO_GPFEN0		0x00000058U
#define GPIO_GPFEN1		0x0000005CU
#define GPIO_GPHEN0		0x00000064U
#define GPIO_GPHEN1		0x00000068U
#define GPIO_GPLEN0		0x00000070U
#define GPIO_GPLEN1		0x00000074U
#define GPIO_GPAREN0	0x0000007CU
#define GPIO_GPAREN1	0x00000080U
#define GPIO_GPAFEN0	0x00000088U
#define GPIO_GPAFEN1	0x0000008CU
#define GPIO_GPPUD0		0x000000E4U
#define GPIO_GPPUD1		0x000000E8U
#define GPIO_GPPUD2		0x000000ECU
#define GPIO_GPPUD3		0x000000F0U


#define PADS0_BASE	0x0010002CU
#define PADS1_BASE	0x0010030CU
#define PADS2_BASE	0x00100034U

#define PADS_PASSWORD	0x5A000000U
#define PADS_SLEW_MASK	0x00000010U
#define PADS_HYST_MASK	0x00000008U
#define PADS_DRIVE_MASK	0x00000007U


GPIO_t *GPIO;
volatile uint32_t *PADS0 = NULL;
volatile uint32_t *PADS1 = NULL;
volatile uint32_t *PADS2 = NULL;

static void HAL_PADS_Init(void)
{
	PADS0 = HAL_get_peri_base() + PADS0_BASE/4;
	PADS1 = HAL_get_peri_base() + PADS1_BASE/4;
	PADS2 = HAL_get_peri_base() + PADS2_BASE/4;
}

GPIO_t *HAL_GPIO_Init(void)
{
	if(HAL_get_peri_base() == MAP_FAILED)
		return NULL;
	
	GPIO = (GPIO_t *)(HAL_get_peri_base() + GPIO_BASE/4);
	HAL_PADS_Init();
	
	return GPIO;
}

void HAL_GPIO_Set_Mode(uint8_t pin, eGPIOMode mode)
{
	uint8_t nreg = pin/10;
	switch(nreg)
	{
		case 0:
			GPIO->GPFSEL0 &= ~(0x00000007U << pin*3);
			GPIO->GPFSEL0 |= ((uint32_t)mode << pin*3);
		break;
		
		case 1:
			pin -= 10;
			GPIO->GPFSEL1 &= ~(0x00000007U << pin*3);
			GPIO->GPFSEL1 |= ((uint32_t)mode << pin*3);
		break;
		
		case 2:
			pin -= 20;
			GPIO->GPFSEL2 &= ~(0x00000007U << pin*3);
			GPIO->GPFSEL2 |= ((uint32_t)mode << pin*3);
		break;
		
		case 3:
			pin -= 30;
			GPIO->GPFSEL3 &= ~(0x00000007U << pin*3);
			GPIO->GPFSEL3 |= ((uint32_t)mode << pin*3);
		break;
		
		case 4:
			pin -= 40;
			GPIO->GPFSEL4 &= ~(0x00000007U << pin*3);
			GPIO->GPFSEL4 |= ((uint32_t)mode << pin*3);
		break;
		
		case 5:
			pin -= 50;
			GPIO->GPFSEL5 &= ~(0x00000007U << pin*3);
			GPIO->GPFSEL5 |= ((uint32_t)mode << pin*3);
		break;
		
		default:
		break;
	}
}

void HAL_GPIO_Set_PuPd(uint8_t pin, eGPIOPuPd mode)
{
	uint8_t nreg = pin/16;
	switch(nreg)
	{
		case 0:
			GPIO->GPPUD0 &= ~(0x00000003U << pin*2);
			GPIO->GPPUD0 |= ((uint32_t)mode << pin*2);
		break;
		
		case 1:
			pin -= 16;
			GPIO->GPPUD1 &= ~(0x00000003U << pin*2);
			GPIO->GPPUD1 |= ((uint32_t)mode << pin*2);
		break;
		
		case 2:
			pin -= 32;
			GPIO->GPPUD2 &= ~(0x00000003U << pin*2);
			GPIO->GPPUD2 |= ((uint32_t)mode << pin*2);
		break;
		
		case 3:
			pin -= 48;
			GPIO->GPPUD3 &= ~(0x00000003U << pin*2);
			GPIO->GPPUD3 |= ((uint32_t)mode << pin*2);
		break;
		
		default:
		break;
	}
}

eGPIOState HAL_GPIO_Read(uint8_t pin)
{
	uint8_t nreg = pin/32;
	
	if(nreg == 0)
	{
		return bitcheck(GPIO->GPLEV0,pin) ? GPIO_HIGH : GPIO_LOW;
	} else
	{
		return bitcheck(GPIO->GPLEV1,pin-32) ? GPIO_HIGH : GPIO_LOW;
	}
}

void HAL_GPIO_Write(uint8_t pin, eGPIOState state)
{
	uint8_t nreg = pin/32;
	
	if(nreg == 0)
	{
		if(state == GPIO_HIGH)
			GPIO->GPSET0 = 1 << pin;
		else
			GPIO->GPCLR0 = 1 << pin;
	} else
	{
		if(state == GPIO_HIGH)
			GPIO->GPSET1 = 1 << (pin-32);
		else
			GPIO->GPCLR1 = 1 << (pin-32);
	}
}

void HAL_GPIO_Toggle(uint8_t pin)
{
	HAL_GPIO_Write(pin,HAL_GPIO_Read(pin)^1);
}


void HAL_GPIO_Set_Slew(eGPIOBank bank,  eGPIOSlew slewrate)
{
	uint32_t padsmask = 0;
	switch(bank)
	{
		case GPIO_BANK_0:
			padsmask = *PADS0 & ~PADS_SLEW_MASK;
			*PADS0 = PADS_PASSWORD | padsmask | ((uint32_t)slewrate << 4);
		break;
		case GPIO_BANK_1:
			padsmask = *PADS1 & ~PADS_SLEW_MASK;
			*PADS1 = PADS_PASSWORD | padsmask | ((uint32_t)slewrate << 4);
		break;
		case GPIO_BANK_2:
			padsmask = *PADS2 & ~PADS_SLEW_MASK;
			*PADS2 = PADS_PASSWORD | padsmask | ((uint32_t)slewrate << 4);
		break;
		default:
		break;
	}
}

void HAL_GPIO_Set_Hyst(eGPIOBank bank,  eGPIOHyst state)
{
	uint32_t padsmask = 0;
	switch(bank)
	{
		case GPIO_BANK_0:
			padsmask = *PADS0 & ~PADS_HYST_MASK;
			*PADS0 = PADS_PASSWORD | padsmask | ((uint32_t)state << 3);
		break;
		case GPIO_BANK_1:
			padsmask = *PADS1 & ~PADS_HYST_MASK;
			*PADS1 = PADS_PASSWORD | padsmask | ((uint32_t)state << 3);
		break;
		case GPIO_BANK_2:
			padsmask = *PADS2 & ~PADS_HYST_MASK;
			*PADS2 = PADS_PASSWORD | padsmask | ((uint32_t)state << 3);
		break;
		default:
		break;
	}
}

void HAL_GPIO_Set_Drive(eGPIOBank bank, eGPIODrive drive)
{
	uint32_t padsmask = 0;
	switch(bank)
	{
		case GPIO_BANK_0:
			padsmask = *PADS0 & ~PADS_DRIVE_MASK;
			*PADS0 = PADS_PASSWORD | padsmask | (uint32_t)drive;
		break;
		case GPIO_BANK_1:
			padsmask = *PADS1 & ~PADS_DRIVE_MASK;
			*PADS1 = PADS_PASSWORD | padsmask | (uint32_t)drive;
		break;
		case GPIO_BANK_2:
			padsmask = *PADS2 & ~PADS_DRIVE_MASK;
			*PADS2 = PADS_PASSWORD | padsmask | (uint32_t)drive;
		break;
		default:
		break;
	}
}
