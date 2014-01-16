obj-m = modcountlist.o
modcountlist-objs = modlist.o modcount.o mainmod.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
