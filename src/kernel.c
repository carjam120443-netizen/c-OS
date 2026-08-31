#include <stdint.h>
#include "fs.h"
#include "users.h"
#include "sudo.h"
#include "shell.h"
#include "keyboard.h"
#include "desktop.h"

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

static void terminal_prompt(void) { terminal_write("root@c-os:/# "); }

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

static int framebuffer_from_multiboot(uint32_t magic, uint32_t mbi,
                                      uint32_t **addr, uint32_t *width,
                                      uint32_t *height, uint32_t *pitch,
                                      uint32_t *bpp) {
    /* Multiboot 0.6.x: framebuffer fields start at offset 88 when bit 12 is set. */
    if (magic != 0x2BADB002 || !mbi) return 0;
    volatile uint32_t *p = (volatile uint32_t*)mbi;
    if (!(p[0] & (1u << 12))) return 0;
    *addr = (uint32_t*)(uintptr_t)p[22];
    *pitch = p[23];
    *width = p[24];
    *height = p[25];
    *bpp = ((volatile uint8_t*)mbi)[102];
    return *addr && *width && *height && *pitch && *bpp == 32;
}

void kmain(uint32_t magic, uint32_t mbi) {
    users_init();
    keyboard_init();
    shell_init();

    uint32_t *framebuffer = 0, width = 0, height = 0, pitch = 0, bpp = 0;
    if (framebuffer_from_multiboot(magic, mbi, &framebuffer, &width, &height, &pitch, &bpp)) {
        desktop_init_framebuffer(framebuffer, width, height, pitch, bpp);
        /* The graphical desktop is the default c-OS session. */
        for (;;) {
            keyboard_poll();
            while (keyboard_has_key()) {
                char c = keyboard_getchar();
                /* Lightweight desktop controls: T redraws the native terminal
                   surface in the text console on systems without a GUI app yet. */
                if (c == 'r' || c == 'R') desktop_draw();
            }
        }
    }

    terminal_write("Welcome to c-OS!\n");
    terminal_write("Kernel initialized successfully.\n");
    terminal_write("[keyboard] PS/2 keyboard input initialized.\n");
    terminal_write("[shell] Interactive command dispatcher ready.\n\n");
    terminal_write("[fs] Mounting root filesystem at / ... ");
    fs_mount_root();
    if (fs_root()) terminal_write("OK\n"); else terminal_write("FAILED\n");
    terminal_write("\nc-OS interactive terminal\n");
    terminal_prompt();

    for (;;) {
        keyboard_poll();
        while (keyboard_has_key()) terminal_process_key(keyboard_getchar());
    }
}
