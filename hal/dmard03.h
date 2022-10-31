#ifndef __DMARD03_H
#define __DMARD03_H

#ifdef __cplusplus
extern "C" {
#endif

 
/* Includes ----------------------------------------------------------*/
#include "bcm2xxx.h"
#include "bcm2xxx_hal_gpio.h"
#include "bcm2xxx_hal_i2c.h"
#include "bcm2xxx_hal_spi.h"
#include "bcm2xxx_delay.h"
#include "softspi.h"

/* Exported types ----------------------------------------------------*/
typedef struct
{
	float x;
	float y;
	float z;
}Point3_t;

typedef enum
{
	DMARD03_I2C=0,
	DMARD03_SPI
}eDMARD03Interface;

typedef	enum
{
	DMARD03_ORDER_8=0,
	DMARD03_ORDER_4,
	DMARD03_ORDER_2,
	DMARD03_ORDER_OFF
}eDMARD03FiltOrd;

typedef enum
{
	DMARD03_SAMPLING_342HZ=0,
	DMARD03_SAMPLING_684HZ
}eDMARD03Sampling;

typedef enum
{
	DMARD03_SPI_WRITE=0,
	DMARD03_SPI_READ
}eDMARD03SPIRW;


/* Exported constants ------------------------------------------------*/
// Data registers
#define DMARD03_T0		0x00
#define DMARD03_T1		0x01
#define DMARD03_XACCE0	0x02
#define DMARD03_XACCE1	0x03
#define DMARD03_YACCE0	0x04
#define DMARD03_YACCE1	0x05
#define DMARD03_ZACCE0	0x06
#define DMARD03_ZACCE1	0x07
// Control registers
#define DMARD03_CON0	0x08
#define DMARD03_CON1	0x09
#define DMARD03_CON2	0x0A
#define DMARD03_CON3	0x0B
// Interrupt status registers
#define DMARD03_INT0	0x0C
#define DMARD03_INT1	0x0D

/* Exported macros ---------------------------------------------------*/

/* Exported variables ------------------------------------------------*/

/* Exported functions ------------------------------------------------*/
// Initialization
void DMARD03_Begin(eDMARD03Interface interface, eDMARD03Sampling fs, eDMARD03FiltOrd ord);
void DMARD03_End(void);
// Registers
size_t DMARD03_Write(uint8_t reg, uint8_t *data, size_t n);
size_t DMARD03_Read(uint8_t reg, uint8_t *data, size_t n);
// Accelerometer
void DMARD03_ReadAccel(Point3_t *p);
float DMARD03_ReadXAccel(void);
float DMARD03_ReadYAccel(void);
float DMARD03_ReadZAccel(void);

uint32_t DMARD03_SPI_DataFormat(eDMARD03SPIRW rw, uint8_t reg, uint8_t data);

#ifdef __cplusplus
}
#endif


#endif /* __DMARD03_H */
