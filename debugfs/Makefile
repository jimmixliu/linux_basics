ifneq ($(KERNELRELEASE), )
    obj-m := debug_fs.o
else
    KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build
    PWD := $(shell pwd)

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	make -C $(KERNEL_DIR) M=$(PWD) modules clean
	rm -rf modules.order
obj-m += debug_fs.o
endif
