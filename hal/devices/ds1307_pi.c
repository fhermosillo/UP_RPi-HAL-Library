#include "ds1307_pi.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>

static I2C_t *I2C0;

tm_t getTime(void);


void DS1307_Setup(void)
{
	// Setup I2C GPIO
	
	// Setup I2C
	
	// Setup DS1307
	tm_t t = getTime(); // Read current time
	DS1307_Write(t);
}

void DS1307_Write(tm_t t)
{
	bcd_t bcd;
	uint8_t data;
	
	// Write 0x80 on register 0x00
	HAL_I2C_WriteReg(I2C0, 0x00, 0x80);
	
	// Write Minute register
	bcd = dec2bcd(t.min);
	data = bcd.uni | (bcd.dec << 4);
	HAL_I2C_WriteReg(I2C0, 0x01, data);
	
	// Write Hour register
	bcd = dec2bcd(t.hour);
	
	// Write Day register
	bcd = dec2bcd(t.day);
	
	// Write Month register
	bcd = dec2bcd(t.month);
	
	// Write Year register
	bcd = dec2bcd(t.year);
	
	// Write Seconds register
	bcd = dec2bcd(t.seconds);
}

tm_t DS1307_Read(void)
{
	tm_t t;
	uint8_t data;
	
	// Read Seconds register
	HAL_I2C_ReadReg(I2C0, 0x00, &data);
	t.sec = bcd2dec(data); // Convert to decimal
	
	// Read Minute register
	
	t.min = bcd2dec(data);
	
	// Read Hour register
	
	t.min = bcd2dec(data);
	
	// Read Day register
	
	t.min = bcd2dec(data);
	
	// Read Month register
	
	t.min = bcd2dec(data);
	
	// Read Year register
	
	t.min = bcd2dec(data);
	
	// Return time
	return t;
}



bcd_t dec2bcd(uint32_t x) {
  bcd_t bcd;

  bcd.uni = x % 10;
  bcd.dec = (x / 10) % 10;
  bcd.cen = (x / 100) % 10;
  bcd.mil = (x / 1000) % 10;

  return bcd;
}

uint32_t bcd2dec(bcd_t bcd) {
  uint32_t x;
  
  x = bcd.uni;
  x += bcd.dec*10;
  x += bcd.cen*100;
  x += bcd.mil*1000;
  
  return x;
}

tm_t getTime(void) {
  time_t t = time(NULL);
  struct tm tloc = *localtime(&t);

  tm_t tm;
  tm.year = tloc.tm_year + 1900;
  tm.month = tloc.tm_mon + 1;
  tm.day = tloc.tm_mday;
  tm.hour = tloc.tm_hour;
  tm.min = tloc.tm_min;
  tm.sec = tloc.tm_sec;

  return tm;
}