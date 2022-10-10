MOD = main
KPATH :=/lib/modules/$(shell uname -r)/build
PWD :=$(shell pwd)
obj-m = $(MOD).o

all:
	$(MAKE) -C $(KPATH) M=$(PWD) modules

clean:
	$(MAKE) -C $(KPATH) M=$(PWD) clean

install: all
	sudo insmod $(MOD).ko
	echo "INSTALLING DONE"

uninstall:
	sudo rmmod $(MOD).ko
	echo "REMOVING DONE"

log: 
	dmesg
