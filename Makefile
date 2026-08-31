CC := i686-elf-gcc
AS := i686-elf-as
LD := i686-elf-ld
GRUB_MKRESCUE := grub-mkrescue

CFLAGS := -std=gnu11 -ffreestanding -O2 -Wall -Wextra -m32
LDFLAGS := -m elf_i386 -T linker.ld

BUILD := build
ISO_DIR := iso
KERNEL := $(ISO_DIR)/boot/kernel.elf
ISO := $(BUILD)/c-OS.iso

.PHONY: all kernel iso run clean

all: iso

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/boot.o: boot/boot.s | $(BUILD)
	$(AS) $< -o $@

$(BUILD)/kernel.o: src/kernel.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

kernel: $(KERNEL)

$(KERNEL): $(BUILD)/boot.o $(BUILD)/kernel.o linker.ld
	mkdir -p $(ISO_DIR)/boot
	$(LD) $(LDFLAGS) -o $@ $(BUILD)/boot.o $(BUILD)/kernel.o

iso: $(KERNEL) | $(BUILD)
	$(GRUB_MKRESCUE) -o $(ISO) $(ISO_DIR)

run: iso
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -rf $(BUILD) $(KERNEL)
