#ifndef __BCM2XXX_H
#define __BCM2XXX_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

/* -------------------------------------------------------------------- */
/* 							Public Defines 								*/
/* -------------------------------------------------------------------- */
#define RPI_OK			1
#define RPI_ERROR		0

#define RPI3_MODEL_B	4
#define RPI4_MODEL_B	3

#define RPI_CORE_FREQ_HZ	250000000

/* -------------------------------------------------------------------- */
/* 							Public Macros 								*/
/* -------------------------------------------------------------------- */
#define bitset(X,n)	(X) |= (1 << (n))
#define bitclr(X,n)	(X) &= ~(1 << (n))
#define bitcheck(X,n)  ((X) & (1<<(n)))

#define bitmask(n)	(1 << (n))

#define BIT(n)		(1 << (n))

#define MAX(X,Y) (X) > (Y) ? (X) : (Y)
#define MIN(X,Y) (X) < (Y) ? (X) : (Y)

#define MHZ(x)	(x)*1000000
#define KHZ(x)	(x)*1000

/* -------------------------------------------------------------------- */
/* 							External Variables 							*/
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/* 						Prototype functions 							*/
/* -------------------------------------------------------------------- */
/*!
 * @name: HAL_read_peri_addr
 * @brief: Read the peripheral address from a specific rpi model
 *
 * @input model: RPI model (RPI4B, RPI3B)
 *
 * @output addr: pointer to uint32_t to store the peripheral
 *			address.
 * @output sz: Total memory (uint32_t) of peripheral registers
 *
 * @return: none
 */
void HAL_read_peri_addr(uint32_t model, uint32_t *addr, size_t *sz);

/*!
 * @name: HAL_memory_map
 * @brief: Physical to virtual peripheral memory mapping.
 *
 * @return: none
 */
void HAL_memory_map(uint32_t model);

/*!
 * @name: HAL_memory_unmap
 * @brief: Free peripheral memory mapping.
 *
 * @return: none
 */
void HAL_memory_unmap(void);

/*!
 * @name: HAL_is_open
 * @brief: Is peripheral's memory open
 *
 * @return: Weather the peripheral's memory has been successfully mapped (RPI_OK, RPI_ERROR)
 */
int HAL_is_peri_open(void);

/*!
 * @name: HAL_get_peri_base
 * @brief: Get peripheral base address pointer
 *
 * @return: peripheral base address
 */
volatile uint32_t *HAL_get_peri_base(void);

/*!
 * @name: HAL_get_core_freq
 * @brief: Get core clock frequency in Herz
 *
 * @return: Core clock frequency
 */
uint32_t HAL_get_core_freq(void);


void HAL_Switch_RealTime(void);


#endif /* __BCM2XXX_H */
