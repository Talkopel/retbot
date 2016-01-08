obj-m := retbot.o
retbot-objs := ./src/retbot.o ./src/collector.o  ./src/recorder.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean