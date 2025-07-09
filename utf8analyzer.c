#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Decode one UTF-8 codepoint from s into *out_cp.
// Returns the number of bytes consumed (1–4).
static int decode_utf8(const char *s, uint32_t *out_cp) {
    const unsigned char *u = (const unsigned char*)s;
    if      (u[0] < 0x80) {
        *out_cp = u[0]; return 1;
    } else if ((u[0] & 0xE0) == 0xC0) {
        *out_cp = ((u[0] & 0x1F) << 6)
                |  (u[1] & 0x3F);
        return 2;
    } else if ((u[0] & 0xF0) == 0xE0) {
        *out_cp = ((u[0] & 0x0F) << 12)
                | ((u[1] & 0x3F) << 6)
                |  (u[2] & 0x3F);
        return 3;
    } else if ((u[0] & 0xF8) == 0xF0) {
        *out_cp = ((u[0] & 0x07) << 18)
                | ((u[1] & 0x3F) << 12)
                | ((u[2] & 0x3F) << 6)
                |  (u[3] & 0x3F);
        return 4;
    }
    *out_cp = 0;
    return 1;
}

// Encode a Unicode codepoint into UTF-8 bytes in dest[].
// Returns the number of bytes written (1–4).
static int encode_utf8(uint32_t num, char dest[4]) {
    if (num <= 0x7F) {
        dest[0] = (char)num;
        return 1;
    } else if (num <= 0x7FF) {
        dest[0] = (char)(0xC0 | (num >> 6));
        dest[1] = (char)(0x80 | (num & 0x3F));
        return 2;
    } else if (num <= 0xFFFF) {
        dest[0] = (char)(0xE0 | (num >> 12));
        dest[1] = (char)(0x80 | ((num >> 6) & 0x3F));
        dest[2] = (char)(0x80 | (num & 0x3F));
        return 3;
    } else {
        dest[0] = (char)(0xF0 | (num >> 18));
        dest[1] = (char)(0x80 | ((num >> 12) & 0x3F));
        dest[2] = (char)(0x80 | ((num >> 6) & 0x3F));
        dest[3] = (char)(0x80 | (num & 0x3F));
        return 4;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s \"<UTF-8 encoded string>\"\n", argv[0]);
        return 1;
    }

    const char *s = argv[1];
    size_t  i = 0, idx = 0, animal_count = 0;
    uint32_t cp, cp_at3 = 0;
    // static buffer for animals; adjust size if you expect more
    uint32_t animals[128];

    // Single pass: decode each codepoint, collect animals, remember idx==3
    while (s[i]) {
        int len = decode_utf8(s + i, &cp);

        if (idx == 3) {
            cp_at3 = cp;
        }

        // Animal emoji ranges
        if ((cp >= 0x1F400 && cp <= 0x1F43F) ||
            (cp >= 0x1F980 && cp <= 0x1F9AE)) {
            if (animal_count < sizeof(animals)/sizeof(*animals))
                animals[animal_count++] = cp;
        }

        idx++;
        i += len;
    }

    // Print all found animal emojis
    printf("Found %zu animal emoji(s): ", animal_count);
    for (size_t j = 0; j < animal_count; j++) {
        char buf[4];
        int blen = encode_utf8(animals[j], buf);
        fwrite(buf, 1, blen, stdout);
        putchar(' ');
    }
    putchar('\n');

    // Print codepoint at index 3 + 1
    if (idx > 3) {
        uint32_t next_cp = cp_at3 + 1;
        printf("Codepoint at idx 3 + 1 = U+%04X: ", next_cp);
        char buf[4];
        int blen = encode_utf8(next_cp, buf);
        fwrite(buf, 1, blen, stdout);
        putchar('\n');
    } else {
        printf("No 4th codepoint to increment.\n");
    }

    return 0;
}
