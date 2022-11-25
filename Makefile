obj-m = proc_filesys.o 
all:
	cd process_gen && $(MAKE)
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	cd process_gen && $(MAKE) clean
