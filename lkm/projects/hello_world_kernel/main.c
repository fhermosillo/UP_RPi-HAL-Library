#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

uint32_t x = 0;

/* This is module initialization entry point */
static int __init my_kernel_module_init(void)
{
	printk(KERN_INFO "Hello world from LKM!");
	return 0;
}

/* This is module clean-up entry point */
static void __exit my_kernel_module_exit(void)
{
	printk(KERN_INFO "God bye LKM\n");
}

/* This is registration of the above entry points with kernel */
module_init(my_kernel_module_init);
module_exit(my_kernel_module_exit);

/* This is descriptive information about the module */
MODULE_LICENSE("GPL"); // This module adhers to GPL licencing
MODULE_AUTHOR("Fernando Hermosillo Reynoso");
MODULE_DESCRIPTION("The hello world of a Linux kernel module");
MODULE_VERSION("1.0");
