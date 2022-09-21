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


GPIO_t *GPIO;

void HAL_GPIO_Init(void)
{
	GPIO = (GPIO_t *)(HAL_get_peri_base() + GPIO_BASE/4);
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
			bitset(GPIO->GPSET0,pin);
		else
			bitset(GPIO->GPCLR0,pin);
	} else
	{
		if(state == GPIO_HIGH)
			bitset(GPIO->GPSET1,pin-32);
		else
			bitset(GPIO->GPCLR1,pin-32);
	}
}

void HAL_GPIO_Toggle(uint8_t pin)
{
	uint8_t nreg = pin/32;
	
	if(nreg == 0)
	{
		if(HAL_GPIO_Read(pin))
			bitset(GPIO->GPCLR0,pin);
		else
			bitset(GPIO->GPSET0,pin);
	} else
	{
		if(HAL_GPIO_Read(pin))
			bitset(GPIO->GPCLR1,pin-32);
		else
			bitset(GPIO->GPSET1,pin-32);

	}
}

