#include "source_1.h"

void print_message(const char *kernel_name, const char *msg)
{
	printk("%s: %s\n", kernel_name, msg);
}
