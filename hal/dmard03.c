#include "dmard03.h"

/* Private constants ------------------------------------------------*/
// I2C Interface
#define DMARD03_ADDRESS		0x1C
#define SDA_PIN	GPIO_PIN_2
#define SCL_PIN	GPIO_PIN_3
// SPI Interface
#define MOSI_PIN	GPIO_PIN_10
#define MISO_PIN	GPIO_PIN_9
#define SCLK_PIN	GPIO_PIN_11
#define CS0_PIN		GPIO_PIN_8
#define CS1_PIN		GPIO_PIN_7
//

/* Private types -----------------------------------------------------*/
/* Private macros ----------------------------------------------------*/
#define DMARD03_MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#define DMARD03_MIN(X,Y) ((X) < (Y) ? (X) : (Y))

/* Private variables -------------------------------------------------*/
//static SPI_t *SPI0 = NULL;
SoftSPI_t SSPI;
static I2C_t *I2C1 = NULL;
static int (*DMARD03_WriteReg)(uint8_t,uint8_t) = NULL;
static int (*DMARD03_ReadReg8)(uint8_t,uint8_t*) = NULL;
static int (*DMARD03_ReadReg16)(uint8_t,uint16_t*) = NULL;


/* Prototype Private Functions ---------------------------------------*/
static int DMARD03_I2C_WriteReg(uint8_t reg, uint8_t data);
static int DMARD03_I2C_ReadReg8(uint8_t reg, uint8_t *data);
static int DMARD03_I2C_ReadReg16(uint8_t reg,uint16_t *data);
static int DMARD03_SPI_WriteReg(uint8_t reg, uint8_t data);
static int DMARD03_SPI_ReadReg8(uint8_t reg,uint8_t *data);
static int DMARD03_SPI_ReadReg16(uint8_t reg,uint16_t *data);
//static uint32_t DMARD03_SPI_Format(eDMARD03SPIRW rw, uint8_t reg, uint8_t data);


/* Exported Function Reference ---------------------------------------*/
void DMARD03_Begin(eDMARD03Interface interface, eDMARD03Sampling fs, eDMARD03FiltOrd ord)
{
	if(interface == DMARD03_I2C)
	{
		/* Setup I2C */
		// Setup GPIO
		HAL_GPIO_Set_Mode(MISO_PIN, GPIO_OUTPUT);
		HAL_GPIO_Set_Mode(CS0_PIN, GPIO_OUTPUT);
		HAL_GPIO_Set_Mode(SDA_PIN, GPIO_FUNCTION_0);
		HAL_GPIO_Set_Mode(SCL_PIN, GPIO_FUNCTION_0);
		HAL_GPIO_Write(MISO_PIN, GPIO_LOW);
		HAL_GPIO_Write(CS0_PIN, GPIO_LOW);
		
		// I2C Driver setup
		I2C1 = HAL_I2C_Init(I2C1_DRIVER);
		HAL_I2C_Set_Baud(I2C1, 100000);
		HAL_I2C_Set_Address(I2C1, DMARD03_ADDRESS);
		
		if( HAL_I2C_Is_Connected(I2C1, DMARD03_ADDRESS) )
		{
			printf("DMARD03 is connected!\n");
		} else
		{
			printf("I2C: DMARD is not connected\n");
			exit(-1);
		}
		
		DMARD03_WriteReg = &DMARD03_I2C_WriteReg;
		DMARD03_ReadReg8 = &DMARD03_I2C_ReadReg8;
		DMARD03_ReadReg16 = &DMARD03_I2C_ReadReg16;
	} else
	{
		printf("SPI Interface\n");
		/* Setup SPI */
		/*// GPIO setup
		HAL_GPIO_Set_Mode(MOSI_PIN, GPIO_FUNCTION_0); // MOSI
		HAL_GPIO_Set_Mode(MISO_PIN, GPIO_FUNCTION_0);// MISO
		HAL_GPIO_Set_Mode(SCLK_PIN, GPIO_FUNCTION_0);// SCLK
		HAL_GPIO_Set_Mode(CS0_PIN, GPIO_FUNCTION_0);// CS0
		HAL_GPIO_Set_Mode(CS1_PIN, GPIO_FUNCTION_0);// CS1
		
		// SPI Driver setup
		SPI0 = HAL_SPI_Init(SPI0_DRIVER);
		HAL_SPI_Set_Mode(SPI0,SPI_MODE_0);
		HAL_SPI_Set_Baud(SPI0, MHZ(1));
		HAL_SPI_Set_Chip(SPI0, SPI_CHIP_0, SPI_CHIP_POL_LOW);
		*/
		
		// GPIO
		HAL_GPIO_Set_Mode(CS0_PIN,GPIO_OUTPUT);
		HAL_GPIO_Write(CS0_PIN,GPIO_LOW);
		
		// Software SPI driver setup
		SoftSPI(&SSPI, MOSI_PIN, MISO_PIN, SCLK_PIN);	// Pinout
		SoftSPI_SetBitOrder(&SSPI, SOFTSPI_MSBFIRST);	// MSB
		SoftSPI_SetNumBits(&SSPI, 22);					// 22-bits frames
		SoftSPI_SetMode(&SSPI, SOFTSPI_MODE3);			// Mode 3
		SoftSPI_SetClock(&SSPI,20);						// F = 1/20us
		SoftSPI_Begin(&SSPI);							// GPIO
		
		// Set I/O function
		DMARD03_WriteReg = &DMARD03_SPI_WriteReg;
		DMARD03_ReadReg8 = &DMARD03_SPI_ReadReg8;
		DMARD03_ReadReg16 = &DMARD03_SPI_ReadReg16;
	}
	
	// Configure Moving Average Filter Order & Sampling Frequency
	/*uint8_t data;
	(*DMARD03_ReadReg8)(DMARD03_CON0,&data);
	data &= 0xF8;
	
	data |= (uint8_t)ord | ((uint8_t)fs << 2);
	(*DMARD03_WriteReg)(DMARD03_CON0, data);*/
}

void DMARD03_End(void)
{
	
}

size_t DMARD03_Write(uint8_t reg, uint8_t *data, size_t n)
{
	// Assert input arguments
	if(data == NULL || n < 1 || reg > DMARD03_INT1)
	{
		return -1;
	}
	
	if(reg + n - 1 > DMARD03_INT1)
	{
		n = n - DMARD03_INT1;
	}
	
	// Read
	size_t i;
	for(i = 0; i < n; i++)
	{
		(*DMARD03_WriteReg)(reg+i,data[i]);
	}
	
	return i;
}

size_t DMARD03_Read(uint8_t reg, uint8_t *data, size_t nbytes)
{
	// Assert input arguments
	if((data == NULL) || (nbytes < 1) || (reg > DMARD03_INT1) || (reg + nbytes - 1 > DMARD03_INT1))
	{
		return -1;
	}
	
	// Read
	size_t i;
	for(i = 0; i < nbytes; i++)
	{
		(*DMARD03_ReadReg8)(reg+i,&data[i]);
	}
	
	return i;
}

float DMARD03_ReadXAccel(void)
{
	uint8_t data[2];
	DMARD03_Read(DMARD03_XACCE0,data,2);
	
	// X data comes in 11-bit format
	uint16_t tmp = (uint16_t)(data[1]&0x07) | ((uint16_t)data[0] << 3);
	
	// Y data comes in 2's complement
	int16_t x = (bitcheck(tmp,10) ? tmp | 0xFC00 : tmp);
	
	// Convert to m/s^2
	return 9.8F * ((float)x / 256.0F);
}

float DMARD03_ReadYAccel(void)
{
	uint8_t data[2];
	DMARD03_Read(DMARD03_YACCE0,data,2);
	
	// X data comes in 11-bit format
	uint16_t tmp = (uint16_t)(data[1]&0x07) | ((uint16_t)data[0] << 3);
	
	// Y data comes in 2's complement
	int16_t y = (bitcheck(tmp,10) ? tmp | 0xFC00 : tmp);
	
	// Convert to m/s^2
	return 9.8F * ((float)y / 256.0F);
}

float DMARD03_ReadZAccel(void)
{
	uint8_t data[2];
	DMARD03_Read(DMARD03_ZACCE0,data,2);
	
	// X data comes in 11-bit format
	uint16_t tmp = (uint16_t)(data[1]&0x07) | ((uint16_t)data[0] << 3);
	
	// Y data comes in 2's complement
	int16_t z = (bitcheck(tmp,10) ? tmp | 0xFC00 : tmp);
	
	// Convert to m/s^2
	return 9.8F * ((float)z / 256.0F);
}

void DMARD03_ReadAccel(Point3_t *p)
{
	p->x = DMARD03_ReadXAccel();
	p->y = DMARD03_ReadYAccel();
	p->z = DMARD03_ReadZAccel();
}


/* Private function reference ----------------------------------------*/
static int DMARD03_I2C_WriteReg(uint8_t reg, uint8_t data)
{
	eI2CError err = HAL_I2C_WriteReg(I2C1, reg, data);
	
	return (int)err;
}

static int DMARD03_I2C_ReadReg8(uint8_t reg,uint8_t *data)
{
	eI2CError err = HAL_I2C_ReadReg(I2C1,reg,data);
	
	return (int)err;
}

static int DMARD03_I2C_ReadReg16(uint8_t reg, uint16_t *data)
{
	uint8_t *pData = (uint8_t *)data;
	
	eI2CError err0 = HAL_I2C_ReadReg(I2C1,reg,&pData[0]);
	eI2CError err1 = HAL_I2C_ReadReg(I2C1,reg,&pData[1]);
	
	
	return (int)err0 | (int)err1;
}

static int DMARD03_SPI_WriteReg(uint8_t reg, uint8_t data)
{
	SoftSPI_ChipSelect(CS0_PIN,1,GPIO_HIGH);
	SoftSPI_Transfer(&SSPI, DMARD03_SPI_DataFormat(DMARD03_SPI_WRITE,reg,data));
	SoftSPI_ChipSelect(CS0_PIN,1,GPIO_LOW);
	
	return 0;
}

static int DMARD03_SPI_ReadReg8(uint8_t reg, uint8_t *data)
{
	SoftSPI_ChipSelect(CS0_PIN,1,GPIO_HIGH);
	uint32_t rxdata=SoftSPI_Transfer(&SSPI, DMARD03_SPI_DataFormat(DMARD03_SPI_READ,reg,0x00));
	SoftSPI_ChipSelect(CS0_PIN,1,GPIO_LOW);
	*data = (uint8_t)(rxdata & 0xFF);
	
	return 0;
}

static int DMARD03_SPI_ReadReg16(uint8_t reg, uint16_t *data)
{
	SoftSPI_ChipSelect(CS0_PIN,1,GPIO_HIGH);
	uint32_t rxdata0=SoftSPI_Transfer(&SSPI, DMARD03_SPI_DataFormat(DMARD03_SPI_READ,reg,0x00));
	uint32_t rxdata1=SoftSPI_Transfer(&SSPI, DMARD03_SPI_DataFormat(DMARD03_SPI_READ,reg+1,0x00));
	SoftSPI_ChipSelect(CS0_PIN,1,GPIO_LOW);
	*data = (uint16_t)(rxdata1) | ((uint16_t)rxdata0 << 8);
	
	return 0;
}


uint32_t DMARD03_SPI_DataFormat(eDMARD03SPIRW rw, uint8_t reg, uint8_t data)
{
	// First, the R/nW bit
	uint32_t dout = (uint32_t)rw << 21;
	
	// Then, register address (address is 13-bit)
	dout |= (uint32_t)reg << 8;
	
	// Finally, the data register, if WR then add data
	if(rw == DMARD03_SPI_WRITE)
	{
		dout |= (uint32_t)data;
	}
	
	return dout;
}
