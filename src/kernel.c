#include <stdint.h>

static volatile uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;
static const uint8_t VGA_COLOR = 0x07;

static void terminal_write(const char* text) {
    for (uint32_t i = 0; text[i] != '\0'; ++i) {
        VGA_MEMORY[i] = ((uint16_t)VGA_COLOR << 8) | (uint8_t)text[i];
    }
}

void kmain(void) {
    terminal_write("Welcome to c-OS!\n");
    terminal_write("Kernel initialized successfully.");

    for (;;) {
        __asm__ volatile ("hlt");
    }
}
