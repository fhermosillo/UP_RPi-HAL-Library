#include "bcm2xxx_hal_spi.h"

#include <stdlib.h>



#define SPI0_BASE		0x00204000U

//#define SPI1_BASE		0x00804000U

//#define SPI2_BASE		0x00805000U

#define SPI3_BASE		0x00204600U

#define SPI4_BASE		0x00204800U

#define SPI5_BASE		0x00204A00U

#define SPI6_BASE		0x00204C00U



#define SPI_CS		0x00000000U

#define SPI_FIFO	0x00000004U

#define SPI_CLK		0x00000008U

#define SPI_DLEN	0x0000000CU

#define SPI_LTHO	0x00000010U

#define SPI_DC		0x00000014U



#define SPI_CS_CLEAR_MASK	0x00000030U

#define SPI_CS_CHIP_MASK	0x00000003U

#define SPI_CS_MODE_MASK	0x0000000CU



#define SPI_CS_LEN_LONG	0x02000000

#define SPI_CS_DMA_LEN	0x01000000

#define SPI_CS_CSPOL2	0x00800000

#define SPI_CS_CSPOL1	0x00400000

#define SPI_CS_CSPOL0	0x00200000

#define SPI_CS_RXF		0x00100000

#define SPI_CS_RXR		0x00080000

#define SPI_CS_TXD		0x00040000

#define SPI_CS_RXD		0x00020000

#define SPI_CS_DONE		0x00010000

#define SPI_CS_LEN		0x00002000

#define SPI_CS_REN		0x00001000

#define SPI_CS_ADCS		0x00000800

#define SPI_CS_INTR		0x00000400

#define SPI_CS_INTD		0x00000200

#define SPI_CS_DMAEN	0x00000100

#define SPI_CS_TA		0x00000080

#define SPI_CS_CSPOL	0x00000040

#define SPI_CS_CLEAR_RX	0x00000020

#define SPI_CS_CLEAR_TX	0x00000010

#define SPI_CS_CPOL		0x00000008

#define SPI_CS_CPHA		0x00000004

#define SPI_CS_CS1		0x00000002

#define SPI_CS_CS0		0x00000001



#define SPI_MAXDIV 65535







#ifndef DIV_ROUND_UP

#define DIV_ROUND_UP(NUM,DEN)  (((NUM) + (DEN) - 1) / (DEN))

#endif



SPI_t *HAL_SPI_Init(eSPIDriver SPIx_driver)

{

	// Set driver

	SPI_t *SPIx = (SPI_t *)malloc(sizeof(SPI_t));

	if(HAL_get_peri_base() == MAP_FAILED || SPIx == NULL)

	{

		return NULL;

	}

	

	switch(SPIx_driver)

	{

		case SPI0_DRIVER:

			SPIx = (SPI_t *)(HAL_get_peri_base() + SPI0_BASE/4);

		break;

		

		case SPI3_DRIVER:

			SPIx = (SPI_t *)(HAL_get_peri_base() + SPI3_BASE/4);

		break;

		

		case SPI4_DRIVER:

			SPIx = (SPI_t *)(HAL_get_peri_base() + SPI4_BASE/4);

		break;

		

		case SPI5_DRIVER:

			SPIx = (SPI_t *)(HAL_get_peri_base() + SPI5_BASE/4);

		break;

		

		case SPI6_DRIVER:

			SPIx = (SPI_t *)(HAL_get_peri_base() + SPI6_BASE/4);

		break;

		

		default:

			return NULL;

		break;

	}

	

	// Reset registers

	uint32_t cs = 0x00000000U;

	// Disable SPI interrupts and transfer

	//cs &= ~(SPI_CS_INTR | SPI_CS_INTD | SPI_CS_DMAEN | SPI_CS_TA);

	

	// Clear DONE bit

	cs |= SPI_CS_DONE;

	

	// Reset RX/TX FIFOS

	cs |= SPI_CS_CLEAR_RX | SPI_CS_CLEAR_TX;

	

	SPIx->CS = cs;

	SPIx->CLK = 0x00000000U;

	SPIx->DLEN = 0x00000000U;

	SPIx->LTOH = 0x00000001U;

	SPIx->DC = 0x30201020U;

	

	return SPIx;

}





void HAL_SPI_DeInit(SPI_t * SPIx)

{

	if(SPIx != NULL)

	{

		//free(SPIx);

	}

}



void HAL_SPI_Set_Mode(SPI_t *SPIx, eSPIMode mode)

{

	// Clear mode bits (CPOL,CPH)

	SPIx->CS &= ~SPI_CS_MODE_MASK;

	

	// Setup Mode

	SPIx->CS |= (uint32_t)mode << 2;

}



void HAL_SPI_Set_Chip(SPI_t *SPIx, eSPIChip chip, eSPIChipPol chip_pol)

{

	// Setup chip select polarity

	bitclr(SPIx->CS,6);

	SPIx->CS |= (uint32_t)chip_pol << 6;

	

	// Setup chip select

	SPIx->CS &= ~SPI_CS_CHIP_MASK;

	SPIx->CS |= (uint32_t)chip;

}



void HAL_SPI_Set_ClockDiv(SPI_t*SPIx, eSPIDivider cdiv)

{

	SPIx->CLK = (uint32_t)cdiv;

}



void HAL_SPI_Set_Baud(SPI_t *SPIx, uint32_t baudios)

{

	// baudios = CORE_CLOCK/cdiv

	// CDIV must be a multiple of two

	uint32_t cdiv = DIV_ROUND_UP(HAL_get_core_freq(),baudios);

	cdiv += cdiv%2;

	

	if(cdiv < SPI_MAXDIV)

	{

		SPIx->CLK = cdiv;

	}

}





uint8_t HAL_SPI_Transfer(SPI_t *SPIx, uint8_t data)

{

	// Clear TX and RX FIFO

	SPIx->CS |= SPI_CS_CLEAR_MASK; // CLEAR

	

	// Start transfer

	bitset(SPIx->CS,7);

	

	// Write to FIFO

	while(!(SPIx->CS & SPI_CS_TXD));

	SPIx->FIFO = data;

	

	// Poll DONE until it goes to 1

    while(!bitcheck(SPIx->CS,16))

    {

	}

    

	// Read from FIFO

	while(!(SPIx->CS & SPI_CS_RXD))

	{

	}

	

	uint8_t byte = SPIx->FIFO;



    // Set TA = 0

	bitclr(SPIx->CS,7);

	

	return byte;

}



void HAL_SPI_Transfern(SPI_t *SPIx, uint8_t *txbuf, uint8_t *rxbuf, size_t num_bytes)

{

	// Clear TX and RX FIFO

	SPIx->CS |= SPI_CS_CLEAR_MASK; // CLEAR

	

	// Start transfer

	bitset(SPIx->CS,7);

	

	// Write to FIFO

	size_t txbytes = 0, rxbytes = 0;

	while(txbytes < num_bytes|| rxbytes < num_bytes)

	{

		// Write if FIFO is not full

		if((SPIx->CS & SPI_CS_TXD) && txbytes < num_bytes)

		{

			SPIx->FIFO = (txbuf ? txbuf[txbytes] : 0);

			txbytes++;

		}

		

		// Read if FIFO is not empty

		if((SPIx->CS & SPI_CS_RXD) && rxbytes < num_bytes)

		{

			uint8_t byte = SPIx->FIFO;

			if(rxbuf)

				rxbuf[rxbytes] = byte;

			rxbytes++;

			

		}

		

	}



	

	// Poll DONE until it goes to 1

    while(!bitcheck(SPIx->CS,16));



    // Set TA = 0

	bitclr(SPIx->CS,7);

}

