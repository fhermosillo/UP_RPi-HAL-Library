#include "softspi.h"
#include <time.h>
#include <sys/time.h>

/* Private types -----------------------------------------------------*/

/* Private constants -------------------------------------------------*/

/* Private macros ----------------------------------------------------*/

/* Private variables -------------------------------------------------*/

/* Prototype Private Functions ---------------------------------------*/
static uint32_t SoftSPI_FormatMSB(uint32_t data, uint8_t numBits);
static void SoftSPI_DelayMicros(unsigned int us);
/* Exported Function Reference ---------------------------------------*/
void SoftSPI(SoftSPI_t *dev, uint8_t mosi, uint8_t miso, uint8_t sck)
{
    dev->mosi = mosi;
    dev->miso = miso;
    dev->scl = sck;
    
    dev->delay = -1;
    dev->cpha = GPIO_LOW;
    dev->cpol = GPIO_LOW;
    dev->bitOrder = SOFTSPI_MSBFIRST;
    dev->numBits = 8;
}

void SoftSPI_SetNumBits(SoftSPI_t *dev, uint8_t numbits)
{
	if(numbits > 0 && numbits <= 32)
	{
		dev->numBits = numbits;
	}
}

void SoftSPI_Begin(SoftSPI_t *dev)
{
    HAL_GPIO_Set_Mode(dev->mosi, GPIO_OUTPUT);
    HAL_GPIO_Set_Mode(dev->miso, GPIO_INPUT);
    HAL_GPIO_Set_Mode(dev->scl, GPIO_OUTPUT);
}

void SoftSPI_End(SoftSPI_t *dev)
{
    HAL_GPIO_Set_Mode(dev->mosi, GPIO_INPUT);
    HAL_GPIO_Set_Mode(dev->miso, GPIO_INPUT);
    HAL_GPIO_Set_Mode(dev->scl, GPIO_INPUT);
}


void SoftSPI_SetMode(SoftSPI_t *dev, eSoftSPIMode mode)
{
	switch(mode)
	{
		case SOFTSPI_MODE0:
			dev->cpol = GPIO_LOW;
			dev->cpha = GPIO_LOW;
		break;
		
		case SOFTSPI_MODE1:
			dev->cpol = GPIO_LOW;
			dev->cpha = GPIO_HIGH;
		break;
		
		case SOFTSPI_MODE2:
			dev->cpol = GPIO_HIGH;
			dev->cpha = GPIO_LOW;
		break;
		
		default:
			dev->cpol = GPIO_HIGH;
			dev->cpha = GPIO_HIGH;
		break;
	}
}

void SoftSPI_SetBitOrder(SoftSPI_t *dev, eSoftSPIBitOrder order)
{
	dev->bitOrder = order;
}


void SoftSPI_SetClock(SoftSPI_t *dev, uint32_t us)
{
	dev->delay = us;
}



uint32_t SoftSPI_Transfer(SoftSPI_t *dev, uint32_t data)
{
	//uint8_t buf[32];
	// Revert data if MSB is selected
	if (dev->bitOrder == SOFTSPI_MSBFIRST)
	{
		data = SoftSPI_FormatMSB(data, dev->numBits);
	}
	
	eGPIOState sck = (dev->cpol) ? GPIO_HIGH : GPIO_LOW;
	uint32_t rxdata = 0;
	for (uint8_t bit = 0; bit < dev->numBits; bit++)
	{
		//printf("Bit: %d\n",bit);
		if (dev->cpha) {
			sck ^= 1;
			HAL_GPIO_Write(dev->scl, sck);
			SoftSPI_DelayMicros(dev->delay>>1);
		}
		
		//printf("\tWR.SCLK = %d\n",HAL_GPIO_Read(dev->scl));
		
		/* ... Write bit */
		//buf[bit] = ((data & (1<<bit)) ? 1 : 0);
		HAL_GPIO_Write(dev->mosi, ((data & (1<<bit)) ? GPIO_HIGH : GPIO_LOW));
		SoftSPI_DelayMicros(dev->delay>>1);
		
		sck ^= 1;
		HAL_GPIO_Write(dev->scl, sck);
		
		//printf("\tRD.SCLK = %d\n",HAL_GPIO_Read(dev->scl));
		/* Read bit */
		{
			eGPIOState bval = HAL_GPIO_Read(dev->miso);

			if (dev->bitOrder == SOFTSPI_MSBFIRST) {
				rxdata <<= 1;
				rxdata |= (uint32_t)bval;
			} else {
				rxdata >>= 1;
				rxdata |= (uint32_t)bval << (dev->numBits-1);
			}
		}
		
		SoftSPI_DelayMicros(dev->delay>>1);
		
		if (!dev->cpha) {
			sck ^= 1;
			HAL_GPIO_Write(dev->scl, sck);
		}
		
		//printf("\tSCLK = %d\n",HAL_GPIO_Read(dev->scl));
	}
	
	/*printf("TX: %db",dev->numBits);
	for(int i = 0; i < dev->numBits; i++)
	{
		printf("%d",buf[i]);
	}
	printf("\n");*/
	return rxdata;
}


void SoftSPI_ChipSelect(uint8_t cs,unsigned int ts, eGPIOState pol)
{
	SoftSPI_DelayMicros(ts); // Wait for CS setup/hold time
	HAL_GPIO_Write(cs,pol);
}

/* Private function reference ----------------------------------------*/
static uint32_t SoftSPI_FormatMSB(uint32_t data, uint8_t numBits)
{
	uint32_t tmp = 0;
	for(int i = 0; i < numBits/2; i++)
	{
		tmp |= (data & (1 << (numBits - i - 1))) >> (numBits - 2*i - 1);
	}
	
	for(int i = 0; i < (numBits/2 + numBits%2); i++)
	{
		tmp |= (data & (1 << i)) << (numBits - 2*i - 1);
	}
	
	return tmp;
}

static void SoftSPI_DelayMicros(unsigned int us)
{
	struct timeval tNow, tLong, tEnd ;

	gettimeofday (&tNow, NULL) ;
	tLong.tv_sec  = us / 1000000 ;
	tLong.tv_usec = us % 1000000 ;
	timeradd (&tNow, &tLong, &tEnd) ;

	while (timercmp (&tNow, &tEnd, <))
	{
		gettimeofday (&tNow, NULL) ;
	}
}
