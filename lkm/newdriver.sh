#!/bin/bash

echo "RaspberryPI Version?"
read version
echo "Project name?"
read projname


# check if project folder exists
if [ -d "$(pwd)/projects" ] 
then
    echo ""
else
    mkdir $(pwd)/projects
fi

# Check if project already exists
if [ -d "$(pwd)/projects/$projname" ] 
then
    echo "Error: Directory $(pwd)/projects/$projname already exists." 
else
    echo "Creating new LKM project at $(pwd)/projects/$projname..."
    # Create project directory
    mkdir $(pwd)/projects/$projname
    # copy the contents of lkm folder
    if version==3
    then
	cp -R $(pwd)/main_rpi3.c $(pwd)/projects/$projname/main.c
    else
	cp -R $(pwd)/main_rpi4.c $(pwd)/projects/$projname/main.c
    fi
    
    # Change permissions
    sudo chmod 777 $(pwd)/projects/$projname
    
    # Create makefile
    printf 'MOD=$projname\n' > $(pwd)/projects/$projname/Makefile
    printf 'MOD = main\n' >> $(pwd)/projects/$projname/Makefile
    printf 'KPATH :=/lib/modules/$(shell uname -r)/build\n' >> $(pwd)/projects/$projname/Makefile
    printf 'PWD :=$(shell pwd)\n' >> $(pwd)/projects/$projname/Makefile
    printf 'obj-m += $(MOD).o\n' >> $(pwd)/projects/$projname/Makefile
    printf '$(MOD)-objs += main.o # you can add more source files by appending in this line the name of the source file .o, e.g., source_1.o' >> $(pwd)/projects/$projname/Makefile
    printf '\n' >> $(pwd)/projects/$projname/Makefile
    printf 'all:\n' >> $(pwd)/projects/$projname/Makefile
    printf '	$(MAKE) -C $(KPATH) M=$(PWD) modules\n' >> $(pwd)/projects/$projname/Makefile
    printf '\n' >> $(pwd)/projects/$projname/Makefile
    printf 'clean:\n' >> $(pwd)/projects/$projname/Makefile
    printf '	$(MAKE) -C $(KPATH) M=$(PWD) clean\n' >> $(pwd)/projects/$projname/Makefile
    printf '\n' >> $(pwd)/projects/$projname/Makefile
    printf 'install: all\n' >> $(pwd)/projects/$projname/Makefile
    printf '	sudo insmod $(MOD).ko\n' >> $(pwd)/projects/$projname/Makefile
    printf '\n' >> $(pwd)/projects/$projname/Makefile
    printf 'uninstall:\n' >> $(pwd)/projects/$projname/Makefile
    printf '	sudo rmmod $(MOD).ko\n' >> $(pwd)/projects/$projname/Makefile
    printf '\n' >> $(pwd)/projects/$projname/Makefile
    printf 'log:\n' >> $(pwd)/projects/$projname/Makefile
    printf '	tail -f /var/log/messages\n' >> $(pwd)/projects/$projname/Makefile
    
    # Open geany for edit projectname
    sudo geany $(pwd)/projects/$projname/main.c &
    
fi
