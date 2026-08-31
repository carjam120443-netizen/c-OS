CC := i686-elf-gcc
AS := i686-elf-as
LD := i686-elf-ld
GRUB_MKRESCUE := grub-mkrescue

CFLAGS := -std=gnu11 -ffreestanding -O2 -Wall -Wextra -m32 -Isrc
LDFLAGS := -m elf_i386 -T linker.ld

BUILD := build
ISO_DIR := iso
KERNEL := $(ISO_DIR)/boot/kernel.elf
ISO := $(BUILD)/c-OS.iso

.PHONY: all kernel iso vbox run clean

all: iso

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/boot.o: boot/boot.s | $(BUILD)
	$(AS) $< -o $@

$(BUILD)/kernel.o: src/kernel.c src/fs.h | $(BUILD)
	$(CC) $(CFLAGS) -c src/kernel.c -o $@

$(BUILD)/fs.o: src/fs.c src/fs.h | $(BUILD)
	$(CC) $(CFLAGS) -c src/fs.c -o $@

kernel: $(KERNEL)

$(KERNEL): $(BUILD)/boot.o $(BUILD)/kernel.o $(BUILD)/fs.o linker.ld
	mkdir -p $(ISO_DIR)/boot
	$(LD) $(LDFLAGS) -o $@ $(BUILD)/boot.o $(BUILD)/kernel.o $(BUILD)/fs.o

iso: $(KERNEL) | $(BUILD)
	$(GRUB_MKRESCUE) -o $(ISO) $(ISO_DIR)

vbox: iso
	@echo "c-OS ISO ready for VirtualBox: $(ISO)"

run: iso
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -rf $(BUILD) $(KERNEL)
