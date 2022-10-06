#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <asm/io.h>

#define LKM_NAME "lkm-gpio"
#define LKM_NAME_PRINT "LKM-GPIO"

// Max characters user can send from USER space to this KERNEL module
#define LKM_MAX_USER_SIZE 1024

// Peripheral base address
#define PERIPHERAL_BASE (0xFE000000U)
#define GPIO_BASE (PERIPHERAL_BASE + 0x00200000)


// Structure to interact with /proc/ directory
static struct proc_dir_entry *lkm_proc = NULL;

// Data buffer to interact with USER
static char data_buffer[LKM_MAX_USER_SIZE+1] = {0};

// GPIO registers
static unsigned int *gpio_base_ptr = NULL;


static void gpio_pin_on(unsigned int pin)
{
	unsigned int fsel_index = pin/10;
	unsigned int fsel_bitpos = pin%10;
	
	// Register access (low level)
	unsigned int* GPFSELx = gpio_base_ptr + fsel_index;
	unsigned int* GPSETx = (unsigned int*)((char*)gpio_base_ptr + 0x1c);
	
	// Change FSEL to output
	*GPFSELx &= ~(7 << (fsel_bitpos*3));
	*GPFSELx |= (1 << (fsel_bitpos*3));
	
	// Turn on pin bit
	*GPSETx |= (1 << pin);
}

static void gpio_pin_off(unsigned int pin)
{
	unsigned int *gpio_off_register = (unsigned int*)((char*)gpio_base_ptr + 0x28);
	*gpio_off_register |= (1<<pin);
}


// MODULE FUNCTION TO WRITE FROM KERNEL TO USER
ssize_t lkm_gpio_read(struct file *file, char __user *user, size_t size, loff_t *off)
{
	const char msg[]={"Hello from my module!"};
	return copy_to_user(user,msg,sizeof(msg)) ? 0 : sizeof(msg);
}

// MODULE FUNCTION TO WRITE FROM USER TO KERNEL
ssize_t lkm_gpio_write(struct file *file, const char __user *user, size_t size, loff_t *off)
{
	unsigned int pin = UINT_MAX;
	unsigned int value = UINT_MAX;
	
	// Clean buffer data (set to 0x00 every element)
	memset(data_buffer, 0x0, sizeof(data_buffer));

	if (size > LKM_MAX_USER_SIZE)
	{
		size = LKM_MAX_USER_SIZE;
	}

	// Copy what user transmite to kernel
	if (copy_from_user(data_buffer, user, size))
		return 0;
	
	// Print this information in kernel space
	printk("%s: Data buffer: %s\n", LKM_NAME_PRINT, data_buffer);
	
	// Extract the information written by user
	if (sscanf(data_buffer, "%d,%d", &pin, &value) != 2)
	{
		printk("%s: Inproper data format submitted\n", LKM_NAME_PRINT);
		return size;
	}
	
	
	// Assert information written by User
	if (pin > 21 || pin < 0)
	{
		printk("%s: Invalid pin number submitted\n", LKM_NAME_PRINT);
		return size;
	}

	if (value != 0 && value != 1)
	{
		printk("%s: Invalid on/off value\n", LKM_NAME_PRINT);
		return size;
	}

	printk("%s: GPIO%d=%d\n", LKM_NAME_PRINT, pin, value);
	
	// Execute the hardware changes
	if (value == 1)
	{
		gpio_pin_on(pin);
	} else if (value == 0)
	{
		gpio_pin_off(pin);
	}

	return size;
}

// Register the above functions
static const struct proc_ops lkm_proc_fops = 
{
	.proc_read = lkm_gpio_read,
	.proc_write = lkm_gpio_write,
};


// MODULE FUNCTION TO INITIALIZE
static int __init gpio_driver_init(void)
{
	printk("%s: Welcome to my driver!\n",LKM_NAME_PRINT);
	
	// Map physical memory to virtual memory
	gpio_base_ptr = (unsigned int*)ioremap(GPIO_BASE, PAGE_SIZE);
	if (gpio_base_ptr == NULL)
	{
		printk("%s: Failed to map GPIO memory to driver\n",LKM_NAME_PRINT);
		return -1;
	}
	
	printk("%s: Successfully mapped in GPIO memory\n",LKM_NAME_PRINT);
	
	// create the kernel file in the /proc/ directory
	// Make sure to change the 
	lkm_proc = proc_create(LKM_NAME, 0666, NULL, &lkm_proc_fops);
	if (lkm_proc == NULL)
	{
		return -1;
	}

	return 0;
}

// MODULE FUNCTION TO CLEAN
static void __exit gpio_driver_exit(void)
{
	printk("%s: Leaving my GPIO driver!\n",LKM_NAME_PRINT);
	
	// Unmap memory
	iounmap(gpio_base_ptr);
	
	// Remove file in /proc/ directory
	proc_remove(lkm_proc);
}

// REGISTER THE ABOVE MODULE FUNCTIONS
module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

// KERNEL MODULE INFORMATION
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Universidad Panamericana");
MODULE_DESCRIPTION("GPIO Kernel Module");
MODULE_VERSION("1.0");
