#include "desktop.h"

static uint32_t *fb;
static uint32_t fb_width, fb_height, fb_pitch_pixels;

static void put_pixel(uint32_t x, uint32_t y, uint32_t color) {
    if (!fb || x >= fb_width || y >= fb_height) return;
    fb[y * fb_pitch_pixels + x] = color;
}

static void rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
    for (uint32_t yy = y; yy < y + h && yy < fb_height; yy++)
        for (uint32_t xx = x; xx < x + w && xx < fb_width; xx++)
            put_pixel(xx, yy, color);
}

static void icon(uint32_t x, uint32_t y, uint32_t color) {
    rect(x, y, 56, 56, 0x303844);
    rect(x + 8, y + 8, 40, 40, color);
}

void desktop_init(uint32_t *framebuffer, uint32_t width, uint32_t height) {
    desktop_init_framebuffer(framebuffer, width, height, width * 4, 32);
}

void desktop_init_framebuffer(uint32_t *framebuffer, uint32_t width, uint32_t height,
                              uint32_t pitch_bytes, uint32_t bpp) {
    fb = framebuffer;
    fb_width = width;
    fb_height = height;
    fb_pitch_pixels = pitch_bytes / 4;
    if (bpp != 32 || !fb_pitch_pixels) { fb = 0; return; }
    desktop_clear(0x20242b);
    desktop_draw();
}

void desktop_clear(uint32_t color) {
    if (!fb) return;
    for (uint32_t y = 0; y < fb_height; y++)
        for (uint32_t x = 0; x < fb_width; x++)
            fb[y * fb_pitch_pixels + x] = color;
}

void desktop_draw(void) {
    if (!fb) return;
    rect(0, 0, fb_width, 34, 0x11151b);
    if (fb_height > 48) rect(0, fb_height - 48, fb_width, 48, 0x11151b);

    /* Native c-OS desktop shortcuts inspired by lightweight BSD/X11 layouts. */
    icon(24, 56, 0x4aa3df);   /* Terminal */
    icon(104, 56, 0x77b255);  /* Files */
    icon(184, 56, 0xb0b0b0);  /* Settings */
    icon(264, 56, 0xd6a84f);   /* Package manager */

    rect(12, fb_height - 38, 100, 28, 0x303844);
    rect(124, fb_height - 38, 100, 28, 0x303844);
    rect(236, fb_height - 38, 100, 28, 0x303844);
    rect(348, fb_height - 38, 100, 28, 0x303844);
}

int desktop_available(void) { return fb != 0; }
