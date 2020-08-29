CC=i686-elf-gcc
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra

AS=i686-elf-as
LFLAGS=-ffreestanding -O2 -nostdlib -lgcc

OBJECTS=kernel.o boot.o vga.o

BINARY=kernel.bin

ISO=clay.iso
ISODIR=iso

iso: binary
	mkdir -p $(ISODIR)/boot/grub
	cp $(BINARY) $(ISODIR)/boot/
	cp grub.cfg $(ISODIR)/boot/grub/
	grub-mkrescue -o $(ISO) $(ISODIR)

binary: $(OBJECTS) linker.ld
	$(CC) \
		-T linker.ld \
		-o $(BINARY) \
		$(OBJECTS) \
		$(LFLAGS)

kernel.o: kernel.c
	$(CC) \
		-c kernel.c \
		-o kernel.o \
		$(CFLAGS)

vga.o: vga/vga.c
	$(CC) \
		-c vga/vga.c \
		-o vga.o \
		$(CFLAGS)

boot.o: boot.s
	$(AS) \
		boot.s \
		-o boot.o

clean:
	$(RM) $(OBJECTS) $(BINARY) $(ISO)
	$(RM) -r $(ISODIR)
