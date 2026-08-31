#include "keyboard.h"

#define KBD_STATUS 0x64
#define KBD_DATA   0x60
#define BUFFER_SIZE 128

static char buffer[BUFFER_SIZE];
static uint32_t head, tail;
static uint8_t shift;

static inline uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static const char normal[128] = {
    0,27,'1','2','3','4','5','6','7','8','9','0','-','=',8,9,
    'q','w','e','r','t','y','u','i','o','p','[',']','\n',0,'a','s',
    'd','f','g','h','j','k','l',';','\'', '`',0,'\\','z','x','c','v',
    'b','n','m',',','.','/',0,'*',0,' ',0
};
static const char shifted[128] = {
    0,27,'!','@','#','$','%','^','&','*','(',')','_','+',8,9,
    'Q','W','E','R','T','Y','U','I','O','P','{','}','\n',0,'A','S',
    'D','F','G','H','J','K','L',':','"','~',0,'|','Z','X','C','V',
    'B','N','M','<','>','?',0,'*',0,' ',0
};

static void push(char c) {
    uint32_t next = (head + 1) % BUFFER_SIZE;
    if (next != tail) { buffer[head] = c; head = next; }
}

void keyboard_init(void) { head = tail = 0; shift = 0; }

void keyboard_poll(void) {
    while (inb(KBD_STATUS) & 1) {
        uint8_t sc = inb(KBD_DATA);
        if (sc == 0x2A || sc == 0x36) { shift = 1; continue; }
        if (sc == 0xAA || sc == 0xB6) { shift = 0; continue; }
        if (sc & 0x80) continue;
        if (sc < 128) {
            char c = shift ? shifted[sc] : normal[sc];
            if (c) push(c);
        }
    }
}

int keyboard_has_key(void) { return head != tail; }

char keyboard_getchar(void) {
    if (head == tail) return 0;
    char c = buffer[tail];
    tail = (tail + 1) % BUFFER_SIZE;
    return c;
}
