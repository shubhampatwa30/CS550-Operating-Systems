obj-m:=process_list.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	gcc user_space.c -o user_space
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
	find . -type f -executable -delete
