#include "desktop.h"

static uint32_t *fb;
static uint32_t fb_width, fb_height;

static void put_pixel(uint32_t x, uint32_t y, uint32_t color) {
    if (!fb || x >= fb_width || y >= fb_height) return;
    fb[y * fb_width + x] = color;
}

static void rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
    for (uint32_t yy = y; yy < y + h && yy < fb_height; yy++)
        for (uint32_t xx = x; xx < x + w && xx < fb_width; xx++)
            put_pixel(xx, yy, color);
}

void desktop_init(uint32_t *framebuffer, uint32_t width, uint32_t height) {
    fb = framebuffer;
    fb_width = width;
    fb_height = height;
    desktop_clear(0x20242b);
}

void desktop_clear(uint32_t color) {
    if (!fb) return;
    for (uint32_t y = 0; y < fb_height; y++)
        for (uint32_t x = 0; x < fb_width; x++)
            fb[y * fb_width + x] = color;
}

void desktop_draw(void) {
    if (!fb) return;

    /* Minimal c-OS desktop: panel, launcher, and terminal/file placeholders. */
    rect(0, 0, fb_width, 32, 0x11151b);
    rect(0, fb_height - 48, fb_width, 48, 0x11151b);
    rect(12, fb_height - 38, 120, 28, 0x303844);
    rect(150, fb_height - 38, 120, 28, 0x303844);
    rect(288, fb_height - 38, 120, 28, 0x303844);
    rect(24, 48, 180, 110, 0x303844);
    rect(220, 48, 180, 110, 0x303844);
}
