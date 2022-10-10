#ifndef __SOURCE_1_H
#define __SOURCE_1_H

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <asm/io.h>


void print_message(const char *kernel_name, const char *msg);

#endif /* __SOURCE_1_H */
