SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)
INCLUDE = -I ./include/
ASMS = a
LINK = linker
CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles

all: clean kernel8.img run

%.o: %.S
	aarch64-linux-gnu-gcc $(CFLAGS) $(INCLUDE) -c $(ASMS).S -o $(ASMS).o -g

%.o: %.c
	aarch64-linux-gnu-gcc $(CFLAGS) $(INCLUDE) -c $< -o $@ -g

kernel8.img: $(ASMS).o $(OBJS)
	aarch64-linux-gnu-ld -nostdlib -nostartfiles $(ASMS).o $(OBJS) -T $(LINK).ld -o kernel8.elf
	aarch64-linux-gnu-objcopy -O binary kernel8.elf kernel8.img

clean:
	rm kernel8.elf kernel8.img src/*.o *.o>/dev/null 2>/dev/null || true

run:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img --display none -serial null -serial stdio -s -initrd initramfs.cpio -dtb bcm2710-rpi-3-b-plus.dtb
	# qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial null -serial stdio -s -initrd initramfs.cpio -dtb bcm2710-rpi-3-b-plus.dtb

flash:
	sudo dd if=kernel8.img of=/dev/sdb

screen:
	sudo screen /dev/ttyUSB0 115200

ptty:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -s -display none -serial null -serial pty -initrd initramfs.cpio -dtb bcm2710-rpi-3-b-plus.dtb

boot:
	python3 uart_boot.py
screenb:
	sudo screen /dev/pts/4 115200
cpio:
	cd rootfs
	find . | cpio -o -H newc > ../initramfs.cpio
	cd ..