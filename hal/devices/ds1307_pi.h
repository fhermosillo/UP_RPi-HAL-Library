#ifndef __DS1307_PI_H
#define __DS1307_PI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------*/
#include "bcm2xxx.h"
#include "bcm2xxx_hal_gpio.h"
#include "bcm2xxx_hal_i2c.h"

/* Exported types ----------------------------------------------------*/
typedef struct {
  uint8_t uni;
  uint8_t dec;
  uint8_t cen;
  uint8_t mil;
} bcd_t;

typedef struct {
  int year;
  int month;
  int day;
  int hour;
  int min;
  int sec;
} tm_t;

/* Exported constants ------------------------------------------------*/

/* Exported macros ---------------------------------------------------*/

/* Exported variables ------------------------------------------------*/

/* Exported functions ------------------------------------------------*/
// Auxiliar Functions
bcd_t dec2bcd(uint32_t x);
uint32_t bcd2dec(bcd_t bcd);

// DS1307 Functions
void DS1307_Setup(void);
void DS1307_Write(tm_t t);
tm_t DS1307_Read(void);

#ifdef __cplusplus
}
#endif


#endif /* __DS1307_PI_H */
