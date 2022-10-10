MOD = lkm_gpio
KPATH :=/lib/modules/$(shell uname -r)/build
PWD :=$(shell pwd)
obj-m+= $(MOD).o
$(MOD)-objs+= main.o source_1.o

all:
	$(MAKE) -C $(KPATH) M=$(PWD) modules

clean:
	$(MAKE) -C $(KPATH) M=$(PWD) clean

install:
	sudo insmod $(MOD).ko

uninstall:
	sudo rmmod $(MOD).ko

log: 
	dmesg
