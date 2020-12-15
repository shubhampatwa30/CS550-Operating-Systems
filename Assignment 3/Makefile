CONFIG_MODULE_SIG=n
CONFIG_MODULE_SIG_ALL=n
CONFIG_MODULE_SIG_FORCE=n

ifeq ($(KERNELRELEASE),)
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

modules:
	make -C $(KERNELDIR) M=$(PWD) modules

modules_install:
	sudo make -C $(KERNELDIR) M=$(PWD) modules_install

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c *.mod modules.order Module.symvers .tmp_versions user

.PHONY: modules modules-install clean


else
	obj-m := pf_probe.o
endif
