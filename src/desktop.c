#include "desktop.h"

static uint32_t *fb;
static uint32_t fb_width, fb_height;

static void put_pixel(uint32_t x, uint32_t y, uint32_t color) {
    if (!fb || x >= fb_width || y >= fb_height) return;
    fb[y * fb_width + x] = color;
}

static void rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
    for (uint32_t yy = y; yy < y + h && yy < fb_height; yy++)
        for (uint32_t xx = x; xx < x + w && xx < fb_width; xx++) put_pixel(xx, yy, color);
}

static void icon(uint32_t x, uint32_t y, uint32_t color) {
    rect(x, y, 48, 48, 0x303844);
    rect(x + 8, y + 8, 32, 32, color);
}

void desktop_init(uint32_t *framebuffer, uint32_t width, uint32_t height) {
    fb = framebuffer; fb_width = width; fb_height = height;
    desktop_clear(0x20242b); desktop_draw();
}

void desktop_clear(uint32_t color) {
    if (!fb) return;
    for (uint32_t y = 0; y < fb_height; y++)
        for (uint32_t x = 0; x < fb_width; x++) fb[y * fb_width + x] = color;
}

void desktop_draw(void) {
    if (!fb) return;
    rect(0, 0, fb_width, 32, 0x11151b);
    rect(0, fb_height - 48, fb_width, 48, 0x11151b);

    /* Real desktop shortcuts: Terminal, Files, Settings, and Package Manager. */
    icon(24, 48, 0x4aa3df);
    icon(104, 48, 0x77b255);
    icon(184, 48, 0xb0b0b0);
    icon(264, 48, 0xd6a84f);

    /* Bottom application shortcuts. */
    rect(12, fb_height - 38, 100, 28, 0x303844);
    rect(124, fb_height - 38, 100, 28, 0x303844);
    rect(236, fb_height - 38, 100, 28, 0x303844);
    rect(348, fb_height - 38, 100, 28, 0x303844);
}
