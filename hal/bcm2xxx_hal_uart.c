#include "bcm2xxx_hal_uart.h"
#include "bcm2xxx_hal_cm.h"
#include <stdlib.h>

#define UART0_BASE		0x00201000U
#define UART2_BASE		0x00201400U
#define UART3_BASE		0x00201600U
#define UART4_BASE		0x00201800U
#define UART5_BASE		0x00201A00U

#define UART_DR		0x00000000U
#define UART_RSRECR	0x00000004U
#define UART_FR		0x00000018U
#define UART_ILPR	0x00000020U
#define UART_IBRD	0x00000024U
#define UART_FBRD	0x00000028U
#define UART_LCRH	0x0000002CU
#define UART_CR		0x00000030U
#define UART_IFLS	0x00000034U
#define UART_IMSC	0x00000038U
#define UART_RIS	0x0000003CU
#define UART_MIS	0x00000040U
#define UART_ICR	0x00000044U
#define UART_DMACR	0x00000048U
#define UART_ITCR	0x00000080U
#define UART_ITIP	0x00000084U
#define UART_ITOP	0x00000088U
#define UART_TDR	0x0000008CU

#define UART_INT_DIVMAX 65535
#define UART_FRAC_DIVMAX 63



#ifndef DIV_ROUND_UP
#define DIV_ROUND_UP(NUM,DEN)  (((NUM) + (DEN) - 1) / (DEN))
#endif

UART_t *HAL_UART_Init(eUARTDriver UARTx_driver)
{
	// Set driver
	UART_t *UARTx = (UART_t *)malloc(sizeof(UART_t));
	if(HAL_get_peri_base() == MAP_FAILED || UARTx == NULL)
	{
		return NULL;
	}
	
	switch(UARTx_driver)
	{
		case UART0_DRIVER:
			UARTx = (UART_t *)(HAL_get_peri_base() + UART0_BASE/4);
		break;
		
		case UART2_DRIVER:
			UARTx = (UART_t *)(HAL_get_peri_base() + UART2_BASE/4);
		break;
		
		case UART3_DRIVER:
			UARTx = (UART_t *)(HAL_get_peri_base() + UART3_BASE/4);
		break;
		
		case UART4_DRIVER:
			UARTx = (UART_t *)(HAL_get_peri_base() + UART4_BASE/4);
		break;
		
		case UART5_DRIVER:
			UARTx = (UART_t *)(HAL_get_peri_base() + UART5_BASE/4);
		break;
		
		default:
			return NULL;
		break;
	}
	
	// Reset registers
	// UARTx->IMSC INTERRUPT DISABLE (R/W)
	// UARTx->ICR INTERRUPT CLEAR (W)
	// UARTx->DMACR DMA ENABLE
	
	// Disable UART
	UARTx->CR = 0x00000000U;
	
	// PULL UP DISABLE
	
	// Clear Pending Interrupts
	UARTx->ICR = 0x7FF;
	UARTx->IMSC = 0x00000000U;
	
	// Disable DMA
	UARTx->DMACR = 0x00000000U;
	
	return UARTx;
}

void HAL_UART_DeInit(UART_t * UARTx)
{
	if(UARTx != NULL)
	{
		//free(UARTx);
	}
}

void HAL_UART_Start(UART_t *UARTx)
{
	// Enable UART0, receive & transfer part of UART.
	bitset(UARTx->CR,9); // RX
	bitset(UARTx->CR,8); // TX
	bitset(UARTx->CR,0); // UART_ENABLE
}

void HAL_UART_Stop(UART_t *UARTx)
{
	// Disable UART0, receive & transfer part of UART.
	bitclr(UARTx->CR,9); // RX
	bitclr(UARTx->CR,8); // TX
	bitclr(UARTx->CR,0); // UART_ENABLE
	
	// Wait to complete transaction
	while ( bitcheck(UARTx->FR,3) );
	
	// Flush fifo
	//bitclr(UARTx->LCRH,4);
}

void HAL_UART_Config(UART_t *UARTx, UARTConfig_t cfg)
{
	//UARTx->LCRH<7>:Stick Parity Bit Enable
	//UARTx->LCRH<2>:Parity selection (0: ODD, 1:EVEN)
	//UARTx->LCRH<2>:Parity Checking&Gen Enable
	
	// Stop UART
	HAL_UART_Stop(UARTx);
	
	// Reset Line Control register
	UARTx->LCRH = 0x00000000U;
	
	// Word length
	UARTx->LCRH |= (uint32_t)cfg.nbits << 5;
	
	// FIFO Enable
	UARTx->LCRH |= (uint32_t)cfg.fifo << 4;
	
	// Parity
	if(cfg.parity != UART_PARITY_DISABLE)
	{
		UARTx->LCRH |= 0x00000082U;
		UARTx->LCRH |= (((uint32_t)cfg.fifo)-1) << 2;
	}
}

void HAL_UART_Set_Baud(UART_t *UARTx, uint32_t baudrate)
{
	// baudios = CORE_CLOCK/cdiv
	// BAUDDIV = FUARTCLK/(16*BAUD_RATE)
	// FBRD = 
	// E.G BAUDDIV = 1.45
	// IBRD = 1
	// FBDR = (0.45*64) + 0.5
	
	// Stop UART
	HAL_UART_Stop(UARTx);
	
	// Setup UART baud rate
	CM_t *CM_UART = HAL_CM_Init(CM_UART_CLOCK);
	float BAUDDIV = (float)HAL_CM_Get_Freq(CM_UART)/(16.0F*(float)baudrate);
	
	// Integer part of Baud rate divisor
	UARTx->IBRD=MIN((uint32_t)BAUDDIV,UART_INT_DIVMAX);
	
	// Fractional part of Baud rate divisor
	uint32_t frac=(uint32_t)((BAUDDIV - (float)UARTx->IBRD) * 64.0F) + 0.5F;
	UARTx->FBRD = MIN(frac,UART_FRAC_DIVMAX);
	
	printf("FUARTCLK = %dHz\n",HAL_CM_Get_Freq(CM_UART));
	printf("BAUDDIV = %f\n",BAUDDIV);
	printf("Integer: %d\n",UARTx->IBRD);
	printf("Fractional: %d\n",UARTx->FBRD);
}

void HAL_UART_Write(UART_t *UARTx, uint8_t data)
{
	// To modify CR reg
	// 1. Disable UART
	// 2. Wait until end of TX or RX
	// 3. Flush FIFO: LCRH<4>=0
	// 4. Reprogram CR reg
	// 5. Enable UART
	
	// TX FIFO 32x8
	// RX FIFO 32x12
	
	//UARTx->DR<7:0>;
	// UARTx->FR<3> // BUSY
	// UARTx->FR<5> // TX FIFO FULL
	// UARTx->CR<8>: Transmit enable
	// UARTx->CR<0>: UART enable
	
	// Wait until RX FIFO is not full
	while ( bitcheck(UARTx->FR,5) );
	
	// TX Data
	UARTx->DR = data;
}

uint8_t HAL_UART_Read(UART_t *UARTx)
{
	// UARTx->CR<9>: Receive enable
	// UARTx->CR<0>: UART enable
	// UARTx->FR<4> // RX FIFO EMPTY
	
	// Wait until RX FIFO is not empty
    while ( bitcheck(UARTx->FR,4) );
    return UARTx->DR & 0x000000FFU;
}

void HAL_UART_MultiWrite(UART_t *UARTx, uint8_t *buf, size_t len)
{
	size_t i = 0;
	while(i < len)
	{
		HAL_UART_Write(UARTx,buf[i++]);
	}
}

void HAL_UART_MultiRead(UART_t *UARTx, uint8_t *buf, size_t len)
{
	size_t i = 0;
	while(i < len)
	{
		buf[i++]=HAL_UART_Read(UARTx);
	}
}
