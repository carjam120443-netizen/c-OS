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
$(BUILD)/kernel.o: src/kernel.c src/fs.h src/users.h src/sudo.h src/shell.h src/keyboard.h | $(BUILD)
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
$(BUILD)/fat_dir.o: src/fat_dir.c src/fat_dir.h src/fat_disk.h | $(BUILD)
	$(CC) $(CFLAGS) -c src/fat_dir.c -o $@
$(BUILD)/fat_write.o: src/fat_write.c src/fat_write.h | $(BUILD)
	$(CC) $(CFLAGS) -c src/fat_write.c -o $@
$(BUILD)/fat12.o: src/fat12.c src/fat12.h | $(BUILD)
	$(CC) $(CFLAGS) -c src/fat12.c -o $@
$(BUILD)/ide.o: src/ide.c src/ide.h | $(BUILD)
	$(CC) $(CFLAGS) -c src/ide.c -o $@
$(BUILD)/desktop.o: src/desktop.c src/desktop.h | $(BUILD)
	$(CC) $(CFLAGS) -c src/desktop.c -o $@
$(BUILD)/users.o: src/users.c src/users.h | $(BUILD)
	$(CC) $(CFLAGS) -c src/users.c -o $@
$(BUILD)/sudo.o: src/sudo.c src/sudo.h src/users.h | $(BUILD)
	$(CC) $(CFLAGS) -c src/sudo.c -o $@
$(BUILD)/shell.o: src/shell.c src/shell.h src/users.h src/sudo.h | $(BUILD)
	$(CC) $(CFLAGS) -c src/shell.c -o $@
$(BUILD)/keyboard.o: src/keyboard.c src/keyboard.h | $(BUILD)
	$(CC) $(CFLAGS) -c src/keyboard.c -o $@

kernel: $(KERNEL)

$(KERNEL): $(BUILD)/boot.o $(BUILD)/kernel.o $(BUILD)/fs.o $(BUILD)/fat.o $(BUILD)/block.o $(BUILD)/fat_disk.o $(BUILD)/fat_image.o $(BUILD)/fat_dir.o $(BUILD)/fat_write.o $(BUILD)/fat12.o $(BUILD)/ide.o $(BUILD)/desktop.o $(BUILD)/users.o $(BUILD)/sudo.o $(BUILD)/shell.o $(BUILD)/keyboard.o linker.ld
	mkdir -p $(ISO_DIR)/boot
	$(LD) $(LDFLAGS) -o $@ $(filter-out linker.ld,$^)

iso: $(KERNEL) | $(BUILD)
	$(GRUB_MKRESCUE) -o $(ISO) $(ISO_DIR)

vbox: iso
	@echo "c-OS ISO ready for VirtualBox: $(ISO)"

run: iso
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -rf $(BUILD) $(KERNEL)
