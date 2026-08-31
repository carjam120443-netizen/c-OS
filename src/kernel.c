#include <stdint.h>
#include "fs.h"
#include "users.h"
#include "sudo.h"
#include "shell.h"
#include "keyboard.h"

static volatile uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;
static const uint8_t VGA_COLOR = 0x07;
static uint32_t terminal_row = 0, terminal_col = 0;
static char command_line[SHELL_LINE_MAX];
static uint32_t command_len = 0;

static void terminal_putchar(char c) {
    if (c == '\n') { terminal_col = 0; ++terminal_row; return; }
    if (c == '\b') {
        if (terminal_col) { --terminal_col; VGA_MEMORY[terminal_row * 80 + terminal_col] = ((uint16_t)VGA_COLOR << 8) | ' '; }
        return;
    }
    VGA_MEMORY[terminal_row * 80 + terminal_col] = ((uint16_t)VGA_COLOR << 8) | (uint8_t)c;
    if (++terminal_col >= 80) { terminal_col = 0; ++terminal_row; }
}

static void terminal_write(const char* text) {
    for (uint32_t i = 0; text[i]; ++i) terminal_putchar(text[i]);
}

static void terminal_prompt(void) {
    terminal_write("root@c-os:/# ");
}

static void terminal_process_key(char c) {
    if (c == '\n') {
        terminal_putchar('\n');
        command_line[command_len] = '\0';
        shell_run_command(command_line);
        command_len = 0;
        terminal_prompt();
        return;
    }
    if (c == '\b') {
        if (command_len) { --command_len; terminal_putchar('\b'); }
        return;
    }
    if (c >= 32 && c <= 126 && command_len + 1 < SHELL_LINE_MAX) {
        command_line[command_len++] = c;
        terminal_putchar(c);
    }
}

void kmain(void) {
    terminal_write("Welcome to c-OS!\n");
    terminal_write("Kernel initialized successfully.\n");
    users_init();
    keyboard_init();
    shell_init();
    terminal_write("[keyboard] PS/2 keyboard input initialized.\n");
    terminal_write("[shell] Interactive command dispatcher ready.\n\n");

    terminal_write("[fs] Mounting root filesystem at / ... ");
    fs_mount_root();
    if (fs_root()) {
        terminal_write("OK\n");
        terminal_write("[fs] /etc and /var/db/pkg are available.\n");
    } else terminal_write("FAILED\n");

    terminal_write("\nc-OS interactive terminal\n");
    terminal_prompt();

    for (;;) {
        keyboard_poll();
        while (keyboard_has_key()) terminal_process_key(keyboard_getchar());
        __asm__ volatile ("hlt");
    }
}
