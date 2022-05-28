ifneq (, $(shell which grub-mkrescue 2>/dev/null))
	GRUB_MKRESCUE=grub-mkrescue
else
	ifneq (, $(shell which grub2-mkrescue 2>/dev/null))
	GRUB_MKRESCUE=grub2-mkrescue
	else
	$(error "cannot find grub-mkrescue in PATH")
	endif
endif

CC=i686-elf-gcc
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra -mgeneral-regs-only -mno-red-zone

AS=i686-elf-as
LFLAGS=-ffreestanding -O2 -nostdlib -lgcc

BUILDDIR=build
SYSROOT=$(BUILDDIR)/sysroot
ISO=$(BUILDDIR)/clay.iso

OBJECTS= \
	$(BUILDDIR)/kernel.c.o \
	$(BUILDDIR)/boot.s.o \
	$(BUILDDIR)/vga/vga.c.o \
	$(BUILDDIR)/boot/gdt.c.o \
	$(BUILDDIR)/boot/gdt.s.o \
	$(BUILDDIR)/boot/idt.c.o \
	$(BUILDDIR)/boot/idt.s.o \
	$(BUILDDIR)/boot/pic.c.o \

BINARY=$(BUILDDIR)/kernel.bin

.PHONY: iso clean

iso: $(OBJECTS) $(BINARY) $(ISO)

$(ISO): $(BINARY)
	mkdir -p $(SYSROOT)/boot/grub
	cp $(BINARY) $(SYSROOT)/boot/
	cp grub.cfg $(SYSROOT)/boot/grub/
	$(GRUB_MKRESCUE) -o $(ISO) $(SYSROOT)

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

clean:
	$(RM) -r $(BUILDDIR)

