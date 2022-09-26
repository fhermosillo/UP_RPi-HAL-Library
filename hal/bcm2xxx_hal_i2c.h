#ifndef __BCM2XXX_HAL_I2C_H
#define __BCM2XXX_HAL_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------*/
#include "bcm2xxx.h"

/* Exported types ----------------------------------------------------*/
typedef enum {
	I2C0_DRIVER = 0,
	I2C1_DRIVER,
	I2C2_DRIVER,
	I2C3_DRIVER,
	I2C4_DRIVER,
	I2C5_DRIVER,
	I2C6_DRIVER,
	I2C7_DRIVER
} eI2CDrive;

// SCL = core_clock / CDIV
// core_clock = 150MHz
typedef enum
{
    I2C_STANDARD_MODE   = 2500,	/*!< 100 Kbs */
    I2C_FULL_SPEED_MODE	= 625,	/*!< 400 Kbs */
    I2C_FAST_MODE    	= 250,	/*!< 1.0 Mbs */
    I2C_HIGH_SPEED_MODE = 79	/*!< 3.2 Mbs */
} I2CSpeedMode;

typedef enum
{
	I2C_ERROR_NONE = 0,
	I2C_ERROR_ACK,		/*!<  Slave has not acknowledged its address */
	I2C_ERROR_TIMEOUT	/*!<  Clock stretch timeout */
} eI2CError;

typedef struct
{
	volatile uint32_t C;
	volatile uint32_t S;
	volatile uint32_t DLEN;
	volatile uint32_t A;
	volatile uint32_t FIFO;
	volatile uint32_t DIV;
	volatile uint32_t DEL;
	volatile uint32_t CLKT;
} I2C_t;


/* Exported constants ------------------------------------------------*/
#define I2C_FIFO_SIZE	16

/* Exported macros ---------------------------------------------------*/

/* Exported variables ------------------------------------------------*/
//extern I2C_t *I2C0;	/*!<  I2C0 peripheral */
//extern I2C_t *I2C1;	/*!<  I2C1 peripheral */
//extern I2C_t *I2C3;	/*!<  I2C3 peripheral */
//extern I2C_t *I2C4;	/*!<  I2C4 peripheral */
//extern I2C_t *I2C5;	/*!<  I2C5 peripheral */
//extern I2C_t *I2C6;	/*!<  I2C6 peripheral */

/* Exported functions ------------------------------------------------*/
I2C_t *HAL_I2C_Init(eI2CDrive ni2cx);
void HAL_I2C_DeInit(I2C_t * i2cx);

void HAL_I2C_Set_Address(I2C_t *I2Cx, uint32_t addr);

void HAL_I2C_Set_Speed(I2C_t *i2cx, I2CSpeedMode mode);

void HAL_I2C_Set_Baud(I2C_t *i2cx, uint32_t baudios);

eI2CError HAL_I2C_Write(I2C_t *i2cx, uint8_t *buf, size_t len);

eI2CError HAL_I2C_Read (I2C_t *i2cx, uint8_t *buf, size_t len);

eI2CError HAL_I2C_WriteReg(I2C_t *i2cx, uint8_t addr, uint8_t data);

eI2CError HAL_I2C_ReadReg(I2C_t *i2cx, uint8_t addr, uint8_t *data);

int HAL_I2C_Is_Connected(I2C_t *i2cx, uint32_t addr);

#ifdef __cplusplus
}
#endif


#endif /* __BCM2XXX_HAL_I2C_H */
