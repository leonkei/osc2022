SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
ASMS = a
LINK = linker
CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles

all: clean kernel8.img run

%.o: %.S
	aarch64-linux-gnu-gcc $(CFLAGS) -c $(ASMS).S -o $(ASMS).o

%.o: %.c
	aarch64-linux-gnu-gcc $(CFLAGS) -c $< -o $@

kernel8.img: $(ASMS).o $(OBJS)
	aarch64-linux-gnu-ld -nostdlib -nostartfiles $(ASMS).o $(OBJS) -T $(LINK).ld -o kernel8.elf
	aarch64-linux-gnu-objcopy -O binary kernel8.elf kernel8.img

clean:
	rm kernel8.elf kernel8.img *.o >/dev/null 2>/dev/null || true

run:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img --display none -serial null -serial stdio