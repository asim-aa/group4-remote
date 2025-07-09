#include <stdio.h>
#include <stdint.h>


static int decode_utf8(const char *s, uint32_t *out_cp) {
    const unsigned char *u = (const unsigned char*)s;
    if      (u[0] < 0x80) {
        *out_cp = u[0]; return 1;
    } else if ((u[0] & 0xE0) == 0xC0) {
        *out_cp = ((u[0]&0x1F)<<6)   | (u[1]&0x3F);
        return 2;
    } else if ((u[0] & 0xF0) == 0xE0) {
        *out_cp = ((u[0]&0x0F)<<12)  | ((u[1]&0x3F)<<6)
                |  (u[2]&0x3F);
        return 3;
    } else if ((u[0] & 0xF8) == 0xF0) {
        *out_cp = ((u[0]&0x07)<<18)  | ((u[1]&0x3F)<<12)
                | ((u[2]&0x3F)<<6)   |  (u[3]&0x3F);
        return 4;
    }
    *out_cp = 0;
    return 1;
}


static void print_utf8(uint32_t cp) {
    if (cp <= 0x7F) {
        putchar(cp);
    } else if (cp <= 0x7FF) {
        putchar(0xC0 |  (cp >> 6));
        putchar(0x80 |  (cp & 0x3F));
    } else if (cp <= 0xFFFF) {
        putchar(0xE0 |  (cp >> 12));
        putchar(0x80 | ((cp >> 6) & 0x3F));
        putchar(0x80 |  (cp & 0x3F));
    } else {
        putchar(0xF0 |  (cp >> 18));
        putchar(0x80 | ((cp >> 12) & 0x3F));
        putchar(0x80 | ((cp >> 6)  & 0x3F));
        putchar(0x80 |  (cp & 0x3F));
    }
}
