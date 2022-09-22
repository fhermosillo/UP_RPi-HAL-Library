#include "bcm2xxx_hal_i2c.h"


#define I2C0_BASE		0x00205000U
#define I2C1_BASE		0x00804000U
//#define I2C2_BASE		0x00805000U
#define I2C3_BASE		0x00805600U
#define I2C4_BASE		0x00805800U
#define I2C5_BASE		0x00805A00U
#define I2C6_BASE		0x00805C00U

#define I2C_CONTROL		0x00000000U
#define I2C_STATUS		0x00000004U
#define I2C_DLEN		0x00000008U
#define I2C_ADDR		0x0000000CU
#define I2C_FIFO		0x00000010U
#define I2C_DIV			0x00000014U
#define I2C_DEL			0x00000018U
#define I2C_CLKT		0x0000001CU

#define I2C_C_READ		BIT(0)
#define I2C_C_ST		BIT(7)
#define I2C_C_I2CEN		BIT(15)
#define I2C_C_CLEAR_0	BIT(4)
#define I2C_C_CLEAR_1	BIT(5)

#define I2C_S_TA	BIT(0)
#define I2C_S_DONE	BIT(1)
#define I2C_S_RXD	BIT(5)
#define I2C_S_ERR	BIT(8)
#define I2C_S_CLKT	BIT(9)


#define I2C0_SDA_GPIO0  0	
#define I2C0_SCL_GPIO0  1
#define I2C1_SDA_GPIO0  2
#define I2C1_SCL_GPIO0  3
#define I2C3_SDA_GPIO0  2
#define I2C3_SCL_GPIO0  3
#define I2C3_SDA_GPIO1  4
#define I2C3_SCL_GPIO1  5
#define I2C4_SDA_GPIO0  6
#define I2C4_SCL_GPIO0  7
#define I2C4_SDA_GPIO1  8
#define I2C4_SCL_GPIO1  9
#define I2C5_SDA_GPIO0  10
#define I2C5_SCL_GPIO0  11
#define I2C5_SDA_GPIO1  12
#define I2C5_SCL_GPIO1  13
#define I2C6_SDA_GPIO0  0
#define I2C6_SCL_GPIO0  1
#define I2C6_SDA_GPIO1  22
#define I2C6_SCL_GPIO1  23

static I2C_t *I2C0 = NULL;
static I2C_t *I2C1 = NULL;
static I2C_t *I2C3 = NULL;
static I2C_t *I2C4 = NULL;
static I2C_t *I2C5 = NULL;
static I2C_t *I2C6 = NULL;


I2C_t *HAL_I2C_Init(eI2CDrive i2cx_driver)
{
	// Set driver
	I2C_t *I2Cx = NULL;
	volatile uint32_t *perif_base = HAL_get_peri_base();
	switch(i2cx_driver)
	{
		case I2C0_DRIVE:
			I2C0 = (I2C_t *)(perif_base + I2C0_BASE/4);
			I2Cx = I2C0;
		break;
		
		case I2C1_DRIVE:
			I2C1 = (I2C_t *)(perif_base + I2C1_BASE/4);
			I2Cx = I2C1;

		break;
		
		case I2C3_DRIVE:
			I2C3 = (I2C_t *)(perif_base + I2C3_BASE/4);
			I2Cx = I2C3;
		break;
		
		case I2C4_DRIVE:
			I2C4 = (I2C_t *)(perif_base + I2C4_BASE/4);
			I2Cx = I2C4;
		break;
		
		case I2C5_DRIVE:
			I2C5 = (I2C_t *)(perif_base + I2C5_BASE/4);
			I2Cx = I2C5;
		break;
		
		case I2C6_DRIVE:
			I2C6 = (I2C_t *)(perif_base + I2C6_BASE/4);
			I2Cx = I2C6;
		break;
		
		default:
			return I2Cx;
		break;
	}
	
	
	
	bitclr(I2Cx->C,15);	// Disable I2Cx
	bitset(I2Cx->C,10);	// Disable RX interrupts
	bitset(I2Cx->C,9);	// Disable TX interrupts
	bitset(I2Cx->C,8);	// Disable DONE interrupts
	bitset(I2Cx->C,5);	// Clear FIFO
	bitset(I2Cx->C,15);	// Enable I2Cx
	
	return I2Cx;
}

void HAL_I2C_Set_Speed(I2C_t *i2cx, I2CSpeedMode mode)
{
	i2cx->DIV = (uint32_t)mode;
	uint32_t cdiv = i2cx->DIV & 0x0000FFFFU;
	uint32_t fedl = MAX(cdiv/16,1);
	uint32_t redl = MAX(cdiv/4,1);
	
	i2cx->DEL = fedl << 16 | redl;
}

void HAL_I2C_Set_Baud(I2C_t *i2cx, uint32_t baudios)
{
	// baudios = CORE_CLOCK/cdiv
	uint32_t cdiv = HAL_get_core_freq()/baudios;
	
	if(cdiv < 65535)
	{
		i2cx->DIV = cdiv;
		
		uint32_t fedl = MAX(cdiv/16,1);
		uint32_t redl = MAX(cdiv/4,1);
		
		i2cx->DEL = fedl << 16 | redl;
	}
}

void HAL_I2C_Set_Address(I2C_t *I2Cx, uint32_t addr)
{
	// Writte A7...A0
	I2Cx->A = addr;
}

eI2CError HAL_I2C_Write(I2C_t *i2cx, uint8_t *buf, size_t len)
{
	// Clear status
	bitset(i2cx->S,1); // DONE
	bitset(i2cx->S,8); // ERR
	bitset(i2cx->S,9); // CLKT	
	
	// Write 1 to DLEN
	i2cx->DLEN = len;
	
	// Write other data to be transmitted to the FIFO
	size_t n = 0;
	while(len--)
	{
		i2cx->FIFO = buf[n++];
	}
	
	// Set C.READ = 0 AND C.ST = 1
	bitclr(i2cx->C,0);
	bitset(i2cx->C,7);
	
	// Poll S.DONE until S.DONE=1
	while((i2cx->S & (I2C_S_DONE | I2C_S_ERR | I2C_S_CLKT)) == 0);
	
	// Check error flags
	eI2CError err = I2C_ERROR_NONE;
	if( bitcheck(i2cx->S,8) )
		err = I2C_ERROR_ACK;
	else if( bitcheck(i2cx->S,9) )
		err = I2C_ERROR_TIMEOUT;
	
	return err;
}

eI2CError HAL_I2C_Read (I2C_t *i2cx, uint8_t *buf, size_t len)
{
	// Clear status
	bitset(i2cx->S,1); // DONE
	bitset(i2cx->S,8); // ERR
	bitset(i2cx->S,9); // CLKT
	
	// Write number of bytes to read in DLEN
	i2cx->DLEN = len;
	
	// Set C.READ = 1 AND C.ST = 1
	bitset(i2cx->C,1); 	// READ
	bitset(i2cx->C,7);	// ST
	
	// Poll S.DONE until S.DONE=1
	while((i2cx->S & (I2C_S_DONE | I2C_S_ERR | I2C_S_CLKT)) == 0);
	
	// Read data
	size_t n = 0;
	while(len--)
	{
		buf[n++]=i2cx->FIFO;
	}
	
	// Check error flags
	eI2CError err = I2C_ERROR_NONE;
	if( bitcheck(i2cx->S,8) )
		err = I2C_ERROR_ACK;
	else if( bitcheck(i2cx->S,9) )
		err = I2C_ERROR_TIMEOUT;
	
	return err;
}


eI2CError HAL_I2C_WriteReg(I2C_t *i2cx, uint8_t regaddr, uint8_t data)
{
	eI2CError err = I2C_ERROR_NONE;
	
	// Disable I2C
	i2cx->C = 0;
	
	// Write Register Address ------------------------------------------
	// Set byte transfer
	i2cx->DLEN = 2;
	
	// Reset FIFO
	i2cx->C |= I2C_C_CLEAR_0 | I2C_C_I2CEN;
	
	// Push 8-bit register address
	i2cx->FIFO = (uint32_t)regaddr;
	i2cx->FIFO = (uint32_t)data;
	
	// Reset DONE flag
	bitset(i2cx->S,1);
	
	// Start Write Transfer
	i2cx->C |= I2C_C_ST | I2C_C_I2CEN;
	
	// Spin until the DONE flag (in status register) is set
	while(((i2cx->S >> 1)&0x01)!=1);
	
	// Get errors
	if( bitcheck(i2cx->S,8) )
		err = I2C_ERROR_ACK;
	else if( bitcheck(i2cx->S,9) )
		err = I2C_ERROR_TIMEOUT;
	
	return err;
}



eI2CError HAL_I2C_ReadReg(I2C_t *i2cx, uint8_t regaddr, uint8_t *data)
{	 
	eI2CError err = I2C_ERROR_NONE;
	
	// Disable I2C
	i2cx->C = 0;
	
	// Write Register Address ------------------------------------------
	// Set byte transfer
	i2cx->DLEN = 1;
	
	// Reset FIFO
	i2cx->C |= I2C_C_CLEAR_0 | I2C_C_I2CEN;
	
	// Push 8-bit register address
	i2cx->FIFO = (uint32_t)regaddr;
	
	// Reset DONE flag
	bitset(i2cx->S,1);
	
	// Start Write Transfer
	i2cx->C |= I2C_C_ST | I2C_C_I2CEN;
	
	// Spin until the DONE flag (in status register) is set
	while(((i2cx->S>>1)& 0X01) != 1);
	
	// Read Register Value  --------------------------------------------
	// Reset FIFO
	i2cx->C |= I2C_C_CLEAR_1 | I2C_C_CLEAR_0;
	
	// Reset DONE flag
	bitset(i2cx->S,1);
	
	// Start read transfer
	i2cx->C |= I2C_C_READ | I2C_C_ST;
	
	// Spin until the DONE flag (in status register) is set
	while(((i2cx->S>>1)&0x01) != 1);
	
	*data = (uint8_t)(i2cx->FIFO & 0xFF);
	
	// Get errors
	if( bitcheck(i2cx->S,8) )
		err = I2C_ERROR_ACK;
	else if( bitcheck(i2cx->S,9) )
		err = I2C_ERROR_TIMEOUT;
	
	return err;
}

int HAL_I2C_Is_Connected(I2C_t *i2cx, uint32_t addr)
{
	HAL_I2C_Set_Address(i2cx, addr);
	
	uint8_t buf = 0;
	eI2CError err = HAL_I2C_ReadReg(i2cx, 0x00, &buf);
	
	return err == I2C_ERROR_NONE ? 1 : 0;
}
