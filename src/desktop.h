#ifndef COS_DESKTOP_H
#define COS_DESKTOP_H
#include <stdint.h>

#define DESKTOP_WIDTH  640
#define DESKTOP_HEIGHT 480

void desktop_init(uint32_t *framebuffer, uint32_t width, uint32_t height);
void desktop_init_framebuffer(uint32_t *framebuffer, uint32_t width, uint32_t height,
                              uint32_t pitch_bytes, uint32_t bpp);
void desktop_clear(uint32_t color);
void desktop_draw(void);
int desktop_available(void);

#endif
