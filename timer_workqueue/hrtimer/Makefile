ifneq ($(KERNELRELEASE),)
	obj-m := hrtimer.o
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) modules clean
	rm -rf modules.order

endif
