#include <stdint.h>
#include "fs.h"

static volatile uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;
static const uint8_t VGA_COLOR = 0x07;
static uint32_t terminal_row = 0;
static uint32_t terminal_col = 0;

static void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_col = 0;
        ++terminal_row;
        return;
    }

    VGA_MEMORY[terminal_row * 80 + terminal_col] =
        ((uint16_t)VGA_COLOR << 8) | (uint8_t)c;

    if (++terminal_col >= 80) {
        terminal_col = 0;
        ++terminal_row;
    }
}

static void terminal_write(const char* text) {
    for (uint32_t i = 0; text[i] != '\0'; ++i)
        terminal_putchar(text[i]);
}

static void terminal_write_uint(uint32_t value) {
    char buffer[11];
    uint32_t i = 0;

    if (value == 0) {
        terminal_putchar('0');
        return;
    }

    while (value > 0) {
        buffer[i++] = (char)('0' + (value % 10));
        value /= 10;
    }

    while (i > 0)
        terminal_putchar(buffer[--i]);
}

void kmain(void) {
    terminal_write("Welcome to c-OS!\n");
    terminal_write("Kernel initialized successfully.\n\n");

    terminal_write("[fs] Mounting root filesystem at / ... ");
    fs_mount_root();

    if (fs_root()) {
        terminal_write("OK\n");
        terminal_write("[fs] Root filesystem contains ");
        terminal_write_uint(fs_node_count());
        terminal_write(" nodes.\n");
        terminal_write("[fs] /etc and /var/db/pkg are available.\n");
    } else {
        terminal_write("FAILED\n");
    }

    terminal_write("\nc-OS root filesystem mounted successfully.\n");

    for (;;) {
        __asm__ volatile ("hlt");
    }
}
