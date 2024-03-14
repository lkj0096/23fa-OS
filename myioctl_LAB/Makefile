obj-m += myioctl_driver.o

all: driver app

driver:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

app:
	gcc -o myioctl_app myioctl_app.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm myioctl_app