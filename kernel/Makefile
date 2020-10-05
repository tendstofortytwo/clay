CC=i686-elf-gcc
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra

AS=i686-elf-as
LFLAGS=-ffreestanding -O2 -nostdlib -lgcc

BUILDDIR=build
SYSROOT=$(BUILDDIR)/sysroot
ISO=$(BUILDDIR)/clay.iso

OBJECTS= \
$(BUILDDIR)/kernel.c.o \
$(BUILDDIR)/boot.s.o \
$(BUILDDIR)/vga/vga.c.o

BINARY=$(BUILDDIR)/kernel.bin

.PHONY: iso clean all

$(ISO): $(BINARY)
	mkdir -p $(SYSROOT)/boot/grub
	cp $(BINARY) $(SYSROOT)/boot/
	cp grub.cfg $(SYSROOT)/boot/grub/
	grub-mkrescue -o $(ISO) $(SYSROOT)

$(BINARY): $(OBJECTS) linker.ld
	mkdir -p $(@D)
	$(CC) \
		-T linker.ld \
		-o $(BINARY) \
		$(OBJECTS) \
		$(LFLAGS)

$(BUILDDIR)/%.c.o: %.c
	mkdir -p $(@D)
	$(CC) \
		-c $< \
		-o $@ \
		$(CFLAGS)

$(BUILDDIR)/%.s.o: %.s
	mkdir -p $(@D)
	$(AS) \
		$< \
		-o $@

boot.o: boot.s

clean:
	$(RM) -r $(BUILDDIR)

