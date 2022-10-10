#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <asm/io.h>
#include "source_1.h"

// Max characters user can send from USER space to this KERNEL module
#define LKM_MAX_USER_SIZE 1024

// Register GPIO address base
#define GPIO_BASE 0x3F200000


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
	const char msg[]={"Hello from LKM-GPIO module!"};
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
	printk("LKM-GPIO: Data buffer: %s\n", data_buffer);
	
	// Extract the information written by user
	if (sscanf(data_buffer, "%d,%d", &pin, &value) != 2)
	{
		
		printk("LKG-GPIO: Inproper data format submitted\n");
		return size;
	}
	
	
	// Assert information written by User
	if (pin > 21 || pin < 0)
	{
		printk("LKM-GPIO: Invalid pin number submitted\n");
		return size;
	}

	if (value != 0 && value != 1)
	{
		printk("LKM-GPIO: Invalid on/off value\n");
		return size;
	}

	printk("LKM-GPIO: You said pin %d, value %d\n", pin, value);
	
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
	printk("LKM-GPIO: Welcome to my GPIO driver!\n");
	
	// Map physical memory to virtual memory
	gpio_base_ptr = (int*)ioremap(GPIO_BASE, PAGE_SIZE);
	if (gpio_base_ptr == NULL)
	{
		printk("LKM-GPIO: Failed to map GPIO memory to driver\n");
		return -1;
	}
	
	printk("LKM-GPIO: Successfully mapped in GPIO memory\n");
	
	// create the kernel file in the /proc/ directory
	lkm_proc = proc_create("lkm-gpio", 0666, NULL, &lkm_proc_fops);
	if (lkm_proc == NULL)
	{
		return -1;
	}

	return 0;
}

// MODULE FUNCTION TO CLEAN
static void __exit gpio_driver_exit(void)
{
	printk("LKM-GPIO: Leaving my GPIO driver!\n");
	
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
MODULE_AUTHOR("Fernando Hermosillo Reynoso");
MODULE_DESCRIPTION("GPIO Kernel Driver Example");
MODULE_VERSION("1.0");
