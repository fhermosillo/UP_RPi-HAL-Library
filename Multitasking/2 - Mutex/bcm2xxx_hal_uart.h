#ifndef __BCM2XXX_HAL_UART_H
#define __BCM2XXX_HAL_UART_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------*/
#include "bcm2xxx.h"
#include "bcm2xxx_hal_gpio.h"
#include "bcm2xxx_hal_cm.h"
/* Exported types ----------------------------------------------------*/
typedef enum {
	UART0_DRIVER = 0,	/*  */
	UART2_DRIVER,		/* Only RPI4 */
	UART3_DRIVER,		/* Only RPI4 */
	UART4_DRIVER,		/* Only RPI4 */
	UART5_DRIVER,		/* Only RPI4 */
} eUARTDriver;

typedef struct
{
	volatile uint32_t DR;
	volatile uint32_t RSRECR;
	volatile uint32_t RESERVED0[4];
	volatile uint32_t FR;
	volatile uint32_t ILPR;
	volatile uint32_t IBRD;
	volatile uint32_t FBRD;
	volatile uint32_t LCRH;
	volatile uint32_t CR;
	volatile uint32_t IFLS;
	volatile uint32_t IMSC;
	volatile uint32_t RIS;
	volatile uint32_t MIS;
	volatile uint32_t ICR;
	volatile uint32_t DMACR;
	/*
	// TEST REGISTERS
	volatile uint32_t RESERVED1[17];
	volatile uint32_t ITCR;
	volatile uint32_t ITIP;
	volatile uint32_t ITOP;
	volatile uint32_t TDR;
	*/
} UART_t;


typedef enum
{
	UART_PARITY_DISABLE = 0,
	UART_PARITY_ODD,
	UART_PARITY_EVEN
}eUARTParity;

typedef enum
{
	UART_FIFO_DISABLE=0,
	UART_FIFO_ENABLE
}eUARTFifo;

typedef enum
{
	UART_WORD_5BITS=0,
	UART_WORD_6BITS,
	UART_WORD_7BITS,
	UART_WORD_8BITS
}eUARTWord;

typedef struct
{
	eUARTParity parity;
	eUARTWord nbits;
	eUARTFifo fifo;
} UARTConfig_t;

/* Exported constants ------------------------------------------------*/
#define UART_TX_FIFO_SIZE	16
#define UART_RX_FIFO_SIZE	16
#define UART_ORDER_MSBFIRST
#define UART_MAX_SPEED MHZ(50)

/* Exported macros ---------------------------------------------------*/

/* Exported variables ------------------------------------------------*/

/* Exported functions ------------------------------------------------*/

// Initialization functions
UART_t *HAL_UART_Init(eUARTDriver nUARTx);

// Control functions
void HAL_UART_Config(UART_t *UARTx, UARTConfig_t cfg);
void HAL_UART_Set_Baud(UART_t *UARTx, uint32_t baudios);
void HAL_UART_Start(UART_t *UARTx);
void HAL_UART_Stop(UART_t *UARTx);

// Read & Write functions
void HAL_UART_Write(UART_t *UARTx, uint8_t data);
uint8_t HAL_UART_Read(UART_t *UARTx);
void HAL_UART_MultiWrite(UART_t *UARTx, uint8_t *buf, size_t len);
void HAL_UART_MultiRead(UART_t *UARTx, uint8_t *buf, size_t len);

#ifdef __cplusplus
}
#endif


#endif /* __BCM2XXX_HAL_UART_H */
