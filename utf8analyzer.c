// utf8analyzer.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

// Yangjian Q1 Q2
// Q1: Return 1 if all bytes in `string` are ASCII (≤127), else 0.
// Q2: Capitalize [a–z] in `str` in‐place and return count of letters changed.
uint8_t is_ascii(char string[]) {
    for (int i = 0; string[i] != '\0'; i++) {
        if ((unsigned char)string[i] > 127) {
            return 0;
        }
    }
    return 1;
}

int32_t capitalize_ascii(char str[]) {
    int32_t count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] -= 32;  // ASCII offset
            count++;
        }
    }
    return count;
}
// End Yangjian Q1 Q2


// Lucas Nguyen Q3 Q4
// Q3: Convert a binary string in `str` (e.g. "1011") to decimal and print it.
// Q4: Return number of UTF-8 codepoints in `str`, or –1 on error.
void bin_to_dec(char str[]) {
    unsigned int result = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        result <<= 1;
        if (str[i] == '1') result |= 1;
    }
    printf("%u\n", result);
}

int32_t utf8_strlen(char str[]) {
    int32_t count = 0;
    unsigned char *p = (unsigned char*)str;
    while (*p) {
        if      ((*p & 0x80) == 0x00) p += 1;
        else if ((*p & 0xE0) == 0xC0) p += 2;
        else if ((*p & 0xF0) == 0xE0) p += 3;
        else if ((*p & 0xF8) == 0xF0) p += 4;
        else return -1;
        count++;
    }
    return count;
}
// End Lucas Nguyen Q3 Q4


// Nirel Indurkar Q5 Q6
// Q5: Return the Unicode codepoint beginning at byte_index, or –1 on continuation/error.
// Q6: Return the byte‐length of the UTF-8 codepoint at str[0], or 0 if empty.
int32_t codepoint_at(char str[], int32_t byte_index) {
    unsigned char *p = (unsigned char*)str + byte_index;
    if ((p[0] & 0xC0) == 0x80) return -1;  // continuation byte
    if ((p[0] & 0x80) == 0x00)       return  p[0];
    else if ((p[0] & 0xE0) == 0xC0)  return ((p[0]&0x1F)<<6)  | (p[1]&0x3F);
    else if ((p[0] & 0xF0) == 0xE0)  return ((p[0]&0x0F)<<12) | ((p[1]&0x3F)<<6) | (p[2]&0x3F);
    else if ((p[0] & 0xF8) == 0xF0)  return ((p[0]&0x07)<<18) | ((p[1]&0x3F)<<12) | ((p[2]&0x3F)<<6) | (p[3]&0x3F);
    return -1;
}

uint8_t codepoint_size(char string[]) {
    if (!string[0]) return 0;
    unsigned char c = (unsigned char)string[0];
    if      ((c & 0x80) == 0x00) return 1;
    else if ((c & 0xE0) == 0xC0) return 2;
    else if ((c & 0xF0) == 0xE0) return 3;
    else if ((c & 0xF8) == 0xF0) return 4;
    return 0;
}
// End Nirel Indurkar Q5 Q6


// Asim Ahmed Q7 Q8 Q9
// Q7 helper: Decode one UTF-8 codepoint from s into *out_cp, return byte‐length.
static int decode_utf8(const char *s, uint32_t *out_cp) {
    const unsigned char *u = (const unsigned char*)s;
    if      (u[0] < 0x80) { *out_cp = u[0]; return 1; }
    else if ((u[0]&0xE0)==0xC0) { *out_cp = ((u[0]&0x1F)<<6)|(u[1]&0x3F); return 2; }
    else if ((u[0]&0xF0)==0xE0) { *out_cp = ((u[0]&0x0F)<<12)|((u[1]&0x3F)<<6)|(u[2]&0x3F); return 3; }
    else if ((u[0]&0xF8)==0xF0) { *out_cp = ((u[0]&0x07)<<18)|((u[1]&0x3F)<<12)|((u[2]&0x3F)<<6)|(u[3]&0x3F); return 4; }
    *out_cp = 0; return 1;
}

// Q7 helper: Print one codepoint as UTF-8 bytes.
static void print_utf8(uint32_t cp) {
    if      (cp <= 0x7F) putchar(cp);
    else if (cp <= 0x7FF) {
        putchar(0xC0|(cp>>6)); putchar(0x80|(cp&0x3F));
    } else if (cp <= 0xFFFF) {
        putchar(0xE0|(cp>>12));
        putchar(0x80|((cp>>6)&0x3F));
        putchar(0x80|(cp&0x3F));
    } else {
        putchar(0xF0|(cp>>18));
        putchar(0x80|((cp>>12)&0x3F));
        putchar(0x80|((cp>>6)&0x3F));
        putchar(0x80|(cp&0x3F));
    }
}

// Q8: Print all animal emojis in s (ranges U+1F400–1F43F, U+1F980–1F9AE).
void print_animal_emojis(const char *s) {
    size_t i = 0; uint32_t cp;
    printf("Animal emojis: ");
    while (s[i]) {
        int len = decode_utf8(s + i, &cp);
        if ((cp>=0x1F400&&cp<=0x1F43F)||(cp>=0x1F980&&cp<=0x1F9AE)) {
            print_utf8(cp); putchar(' ');
        }
        i += len;
    }
    putchar('\n');
}

// Q9a: Find the 4th codepoint (idx 3), add 1, and print it.
void print_next_cp_at3(const char *s) {
    size_t i=0, idx=0; uint32_t cp;
    while (s[i]) {
        int len = decode_utf8(s + i, &cp);
        if (idx == 3) {
            uint32_t nxt = cp + 1;
            printf("Codepoint at idx 3 + 1 = U+%04X: ", nxt);
            print_utf8(nxt);
            putchar('\n');
            return;
        }
        i += len; idx++;
    }
    printf("No 4th codepoint to increment.\n");
}

// Q9b: Print substring of codepoints [start,end).
void utf8_substring(const char str[], int start, int end) {
    int cp_i=0, byte_i=0, b0=-1, b1=-1, sl=strlen(str);
    while (str[byte_i] && cp_i<end) {
        if (cp_i==start) b0 = byte_i;
        uint32_t cp; int sz = decode_utf8(str+byte_i, &cp);
        byte_i += sz; cp_i++;
    }
    if (b0<0) b0=sl;
    if (cp_i>=end && b1<0) b1=byte_i;
    if (b1<0) b1=sl;
    int out_len = b1 - b0;
    if (out_len>0) fwrite(str+b0,1,out_len,stdout);
    putchar('\n');
}
// End Asim Ahmed Q7 Q8 Q9
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s \"<UTF-8 encoded string>\"\n", argv[0]);
        return 1;
    }
    char *input = argv[1];

    // Q1: ASCII?
    printf("q1:%u\n", is_ascii(input));

    // Q2
    char *tmp = strdup(input);
    int32_t caps = capitalize_ascii(tmp);
    printf("q2:%d_%s\n", caps, tmp);
    free(tmp);

    // Q3
    printf("q3:");
    bin_to_dec(input);

    // Q4
    printf("q4:%d\n", utf8_strlen(input));

    // Q5
    printf("q5:%d\n", codepoint_at(input, 0));

    // Q6
    printf("q6:%u\n", codepoint_size(input));

    // Q7
    printf("q7:");
    print_animal_emojis(input);

    // Q8
    printf("q8:");
    print_next_cp_at3(input);

    // Q9
    printf("q9:");
    utf8_substring(input, 0, 6);

    return 0;
}
