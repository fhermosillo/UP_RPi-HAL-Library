#include "bcm2xxx.h"

void *peri_base_ptr = MAP_FAILED;
size_t peri_size = 0;

static uint32_t core_freq = RPI_CORE_FREQ_HZ;

void HAL_read_peri_addr(uint32_t model, uint32_t *addr, size_t *sz)
{
	// Open file in binary read-only mode
	const char adrFilename[] = "/proc/device-tree/soc/ranges";
	FILE *fd = fopen(adrFilename,"rb");
	
	// Read 16 bytes from the file
	uint8_t buff[16];
	size_t bytes_per_item = 1;
	size_t num_items = 16;
	fread(buff, bytes_per_item, num_items, fd);
	
	// Extract peripheral base address
	if(model == RPI4_MODEL_B)
	{
		*addr = buff[8] << 24 |
			buff[9] << 16 |
			buff[10] << 8 |
			buff[11];
			
		*sz = buff[12] << 24 |
			buff[13] << 16 |
			buff[14] << 8 |
			buff[15];
	}
	else if(model == RPI3_MODEL_B)
	{
		*addr = buff[4] << 24 |
			buff[5] << 16 |
			buff[6] << 8 |
			buff[7];
		
		*sz = buff[8] << 24 |
			buff[9] << 16 |
			buff[10] << 8 |
			buff[11];
	}
	else
	{
		*addr = 0;
		*sz = 0;
	}
}

void HAL_memory_map(uint32_t model)
{
	uint32_t base_addr;
	int fd;
	
	peri_size = 0;
	
	// Get peripheral size
	HAL_read_peri_addr(model, &base_addr, &peri_size);
	if(base_addr == 0 || peri_size == 0)
	{
		printf("ERROR: Can't read peripheral's address\n");
		exit(-1);
	}
	printf("Base Address: 0x%X\n", base_addr);
	printf("Peripheral memory size: %d\n", peri_size);
	
	// Open memory mapping file
	fd = open("/dev/mem", O_RDWR | O_SYNC);
	if(fd < 0)
	{
		printf("ERROR: Can't open \"/dev/mem\" file\n");
		exit(-1);
	}
	
	// Memory mapping of the /dev/mem file
	peri_base_ptr = mmap(NULL, peri_size,
						PROT_READ | PROT_WRITE,
						MAP_SHARED,
						fd,
						base_addr);
	
	if(peri_base_ptr == MAP_FAILED)
	{
		printf("ERROR: Unable to map peripheral memory\n");
		exit(-1);
	}
	
	
	/*if(model == RPI4_MODEL_B) 
		core_freq = RPI4_CORE_FREQ_HZ;
	*/

	// Close file, but content will be kept in memory
	close(fd);
}

void HAL_memory_unmap(void)
{
	if(peri_base_ptr != MAP_FAILED)
	{
		munmap(peri_base_ptr, peri_size);
	}
	
	peri_size = 0;
	peri_base_ptr = MAP_FAILED;
}

volatile uint32_t * HAL_get_peri_base(void)
{
	return (volatile uint32_t *)peri_base_ptr;
}

int HAL_is_peri_open(void)
{
	return peri_base_ptr != MAP_FAILED;
}

uint32_t HAL_get_core_freq(void)
{
	return core_freq;
}
