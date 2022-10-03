#ifndef __BCM2XXX_HAL_GPIO_H
#define __BCM2XXX_HAL_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------*/
#include "bcm2xxx.h"

/* Exported types ----------------------------------------------------*/
typedef struct{
	volatile uint32_t GPFSEL0;
	volatile uint32_t GPFSEL1;
	volatile uint32_t GPFSEL2;
	volatile uint32_t GPFSEL3;
	volatile uint32_t GPFSEL4;
	volatile uint32_t GPFSEL5;
		 uint32_t RESERVED0;
	volatile uint32_t GPSET0;
	volatile uint32_t GPSET1;
		 uint32_t RESERVED1;
	volatile uint32_t GPCLR0;
	volatile uint32_t GPCLR1;
		 uint32_t RESERVED2;
	volatile uint32_t GPLEV0;
	volatile uint32_t GPLEV1;
		 uint32_t RESERVED3;
	volatile uint32_t GPEDS0;
	volatile uint32_t GPEDS1;
		 uint32_t RESERVED4;
	volatile uint32_t GPREN0;
	volatile uint32_t GPREN1;
		 uint32_t RESERVED5;
	volatile uint32_t GPFEN0;
	volatile uint32_t GPFEN1;
		 uint32_t RESERVED6;
	volatile uint32_t GPHEN0;
	volatile uint32_t GPHEN1;
		 uint32_t RESERVED7;
	volatile uint32_t GPLEN0;
	volatile uint32_t GPLEN1;
		 uint32_t RESERVED8;
	volatile uint32_t GPAREN0;
	volatile uint32_t GPAREN1;
		 uint32_t RESERVED9;
	volatile uint32_t GPAFEN0;
	volatile uint32_t GPAFEN1;
		 uint32_t RESERVED10[4*5+1];
	volatile uint32_t GPPUD0;
	volatile uint32_t GPPUD1;
	volatile uint32_t GPPUD2;
	volatile uint32_t GPPUD3;
} GPIO_t;


typedef enum
{
    GPIO_INPUT   = 0,
    GPIO_OUTPUT,
    GPIO_FUNCTION_5,
    GPIO_FUNCTION_4,
    GPIO_FUNCTION_0,
    GPIO_FUNCTION_1,
    GPIO_FUNCTION_2,
    GPIO_FUNCTION_3
} eGPIOMode;


typedef enum
{
    GPIO_PPUD_DISABLE = 0,
    GPIO_PPUD_UP,
    GPIO_PPUD_DOWN
} eGPIOPuPd;


typedef enum
{
	GPIO_LOW = 0,
	GPIO_HIGH
} eGPIOState;

typedef enum
{
	GPIO_SLEW_LIMITED=0,
	GPIO_SLEW_FULL
}eGPIOSlew;

typedef enum
{
	PADS_HYST_DISABLE=0,
	PADS_HYST_ENABLE
}eGPIOHyst;

/*
 * A maximum of 16mA per pin
 * A total of current from all pins must be least or equal than 51mA:
 * e.g., if we have 5 leds connected to GPIO0 to GPIO4, each LED consumes
 * 10mA, the total current is 5*10mA = 50mA, then it is OK. However, if
 * we have 6 leds connected to GPIO, total current on GPIOs is 6*10mA = 60mA
 * Then, the Raspberry Pi may reset.
 */
typedef enum
{
	PADS_DRIVE_2mA=0,
	PADS_DRIVE_4mA,
	PADS_DRIVE_6mA,
	PADS_DRIVE_8mA,
	PADS_DRIVE_10mA,
	PADS_DRIVE_12mA,
	PADS_DRIVE_14mA,
	PADS_DRIVE_16mA
}eGPIODrive;

typedef enum
{
	GPIO_BANK_0,	/*!< GPIO0 to GPIO27 */
	GPIO_BANK_1,	/*!< GPIO28 to GPIO45 */
	GPIO_BANK_2		/*!< GPIO46 to GPIO53 */
}eGPIOBank;

/* Exported constants ------------------------------------------------*/
#define GPIO_PIN_0	0
#define GPIO_PIN_1	1
#define GPIO_PIN_2	2
#define GPIO_PIN_3	3
#define GPIO_PIN_4	4
#define GPIO_PIN_5	5
#define GPIO_PIN_6	6
#define GPIO_PIN_7	7
#define GPIO_PIN_8	8
#define GPIO_PIN_9	9
#define GPIO_PIN_10	10
#define GPIO_PIN_11	11
#define GPIO_PIN_12	12
#define GPIO_PIN_13	13
#define GPIO_PIN_14	14
#define GPIO_PIN_15	15
#define GPIO_PIN_16	16
#define GPIO_PIN_17	17
#define GPIO_PIN_18	18
#define GPIO_PIN_19	19
#define GPIO_PIN_20	20
#define GPIO_PIN_21	21
#define GPIO_PIN_22	22
#define GPIO_PIN_23	23
#define GPIO_PIN_24	24
#define GPIO_PIN_25	25
#define GPIO_PIN_26	26
#define GPIO_PIN_27	27


/* Exported macros ---------------------------------------------------*/

/* Exported variables ------------------------------------------------*/

/* Exported functions ------------------------------------------------*/
// GPIO Initializations
GPIO_t *HAL_GPIO_Init(void);

// GPIO IO operations
eGPIOState HAL_GPIO_Read(uint8_t pin);
void HAL_GPIO_Write(uint8_t pin, eGPIOState state);
void HAL_GPIO_Toggle(uint8_t pin);

// GPIO direction
void HAL_GPIO_Set_Mode(uint8_t pin, eGPIOMode mode);
void HAL_GPIO_Set_PuPd(uint8_t pin, eGPIOPuPd pmode);

// GPIO driver configurations
void HAL_GPIO_Set_Slew(eGPIOBank bank,  eGPIOSlew slewrate);
void HAL_GPIO_Set_Hyst(eGPIOBank bank,  eGPIOHyst state);
void HAL_GPIO_Set_Drive(eGPIOBank bank, eGPIODrive drive);

#ifdef __cplusplus
}
#endif


#endif /* __BCM2XXX_HAL_GPIO_H */
