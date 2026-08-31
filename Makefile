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

$(BUILD)/fat.o: src/fat.c src/fat.h | $(BUILD)
	$(CC) $(CFLAGS) -c src/fat.c -o $@

$(BUILD)/block.o: src/block.c src/block.h | $(BUILD)
	$(CC) $(CFLAGS) -c src/block.c -o $@

$(BUILD)/fat_disk.o: src/fat_disk.c src/fat_disk.h src/block.h | $(BUILD)
	$(CC) $(CFLAGS) -c src/fat_disk.c -o $@

$(BUILD)/fat_image.o: src/fat_image.c src/block.h | $(BUILD)
	$(CC) $(CFLAGS) -c src/fat_image.c -o $@

kernel: $(KERNEL)

$(KERNEL): $(BUILD)/boot.o $(BUILD)/kernel.o $(BUILD)/fs.o $(BUILD)/fat.o $(BUILD)/block.o $(BUILD)/fat_disk.o $(BUILD)/fat_image.o linker.ld
	mkdir -p $(ISO_DIR)/boot
	$(LD) $(LDFLAGS) -o $@ $(BUILD)/boot.o $(BUILD)/kernel.o $(BUILD)/fs.o $(BUILD)/fat.o $(BUILD)/block.o $(BUILD)/fat_disk.o $(BUILD)/fat_image.o

iso: $(KERNEL) | $(BUILD)
	$(GRUB_MKRESCUE) -o $(ISO) $(ISO_DIR)

vbox: iso
	@echo "c-OS ISO ready for VirtualBox: $(ISO)"

run: iso
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -rf $(BUILD) $(KERNEL)
