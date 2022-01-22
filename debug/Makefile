obj-m := my_char_device.o
KERNEL_DIR := /usr/src/linux-headers-$(shell uname -r)
PWD := $(shell pwd)
default:
	make -C ${KERNEL_DIR} M=${PWD} modules
clean:
	rm -f *.o *.ko *.mod.c *.mod *.mod.o modules.* Module.*

