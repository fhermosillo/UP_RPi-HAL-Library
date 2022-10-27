#ifndef __BCM2XXX_DELAY_H
#define __BCM2XXX_DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------*/
#include <time.h>
#include <errno.h>


/* Exported types ----------------------------------------------------*/

/* Exported constants ------------------------------------------------*/

/* Exported macros ---------------------------------------------------*/

/* Exported variables ------------------------------------------------*/

/* Exported functions ------------------------------------------------*/
int delay_ms(long ms);
int delay_us(long us);

#ifdef __cplusplus
}
#endif


#endif /* __BCM2XXX_DELAY_H */
