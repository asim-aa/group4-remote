//Yangjian Q1 Q2
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
            str[i] -= 32;  
            count++;
        }
    }
    return count;
}

//end

// Nirel Indurkar Q5
int32_t codepoint_at(char str[], int32_t byte_index) {
    // TODO: Implement this function
    unsigned char *ptr = (unsigned char *)&str[byte_index];
    
    if ((ptr[0] & 0xC0) == 0x80) {
        return -1; // Continuation byte
    }


    if ((ptr[0] & 0x80) == 0x00) {
        return ptr[0];
    } else if ((ptr[0] & 0xE0) == 0xC0) {
        return ((ptr[0] & 0x1F) << 6) |
               (ptr[1] & 0x3F);
    } else if ((ptr[0] & 0xF0) == 0xE0) {
        return ((ptr[0] & 0x0F) << 12) |
               ((ptr[1] & 0x3F) << 6) |
               (ptr[2] & 0x3F);
    } else if ((ptr[0] & 0xF8) == 0xF0) {
        return ((ptr[0] & 0x07) << 18) |
               ((ptr[1] & 0x3F) << 12) |
               ((ptr[2] & 0x3F) << 6) |
               (ptr[3] & 0x3F);
    }
}

int main() {
    char input[2048];
    int32_t byte_index;
    
    // Read lines of input until EOF
    while (scanf("%[^\n]%*c", input) == 1) {
        // Get the byte index after the space
        char* space = strrchr(input, ' ');
        if (space != NULL) {
            *space = '\0';  // Split string at space
            byte_index = atoi(space + 1);
            
            // Call function and print result
            printf("%d\n", codepoint_at(input, byte_index));
        }
    }
    
    return 0;
}

// Nirel Indurkar Q6
uint8_t codepoint_size(char string[]) {
    if(string[0] == '\0') {
        return 0;
    }
    unsigned char first = (unsigned char)string[0];
    if ((first & 0x80) == 0x00) {
        return 1;
    } else if ((first & 0xE0) == 0xC0) {
        return 2;
    } else if ((first & 0xF0) == 0xE0) {
        return 3;
    } else if ((first & 0xF8) == 0xF0) {
        return 4;
    } else {
        return -1;
    }

}
int main(int argc, char** argv) {
        char buffer[100];
        uint8_t result;
        while(1) {
                char* maybe_eof = fgets(buffer, 99, stdin);
                if(maybe_eof == NULL) { break; }
                result = codepoint_size(buffer);
                printf("%d\n", result);
        }
}

//END OF CODE

// Asim Ahmed Q7 Q8 Q9
static int decode_utf8(const char *s, uint32_t *out_cp) {
    const unsigned char *u = (const unsigned char*)s;
    if      (u[0] < 0x80) {
        *out_cp = u[0]; return 1;
    } else if ((u[0] & 0xE0) == 0xC0) {
        *out_cp = ((u[0]&0x1F)<<6) | (u[1]&0x3F);
        return 2;
    } else if ((u[0] & 0xF0) == 0xE0) {
        *out_cp = ((u[0]&0x0F)<<12)
                | ((u[1]&0x3F)<<6)
                |  (u[2]&0x3F);
        return 3;
    } else if ((u[0] & 0xF8) == 0xF0) {
        *out_cp = ((u[0]&0x07)<<18)
                | ((u[1]&0x3F)<<12)
                | ((u[2]&0x3F)<<6)
                |  (u[3]&0x3F);
        return 4;
    }
    *out_cp = 0;
    return 1;
}

// —— UTF-8 printer helper ——
// Emits the UTF-8 bytes of cp via putchar.
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

// —— 1. Print all animal emojis ——
// Scans s, prints any codepoint in U+1F400–1F43F or 1F980–1F9AE.
void print_animal_emojis(const char *s) {
    size_t i = 0;
    uint32_t cp;
    printf("Animal emojis: ");
    while (s[i]) {
        int len = decode_utf8(s + i, &cp);
        if ((cp >= 0x1F400 && cp <= 0x1F43F) ||
            (cp >= 0x1F980 && cp <= 0x1F9AE)) {
            print_utf8(cp);
            putchar(' ');
        }
        i += len;
    }
    putchar('\n');
}

// —— 2. Print “codepoint at index 3 + 1” ——
// Finds the 4th codepoint (index 3), adds 1, and prints it.
void print_next_cp_at3(const char *s) {
    size_t i = 0, idx = 0;
    uint32_t cp;
    while (s[i]) {
        int len = decode_utf8(s + i, &cp);
        if (idx == 3) {
            uint32_t next = cp + 1;
            printf("Codepoint at idx 3 + 1 = U+%04X: ", next);
            print_utf8(next);
            putchar('\n');
            return;
        }
        i += len;
        idx++;
    }
    printf("No 4th codepoint to increment.\n");
}

// —— 3. Print substring of codepoints [start, end) ——
// Walks codepoints, identifies byte range, then prints that slice.
void utf8_substring(const char str[], int start, int end) {
    int cp_index = 0, byte_index = 0;
    int begin_byte = -1, end_byte = -1;
    int str_len = strlen(str);

    // Walk until we hit end
    while (str[byte_index] && cp_index < end) {
        if (cp_index == start) begin_byte = byte_index;

        uint32_t cp;
        int sz = decode_utf8(str + byte_index, &cp);
        byte_index += sz;
        cp_index++;
    }
    if (cp_index == start && begin_byte < 0) begin_byte = byte_index;
    if (cp_index >= end && end_byte < 0) end_byte = byte_index;
    if (begin_byte < 0) begin_byte = str_len;
    if (end_byte < 0)   end_byte   = str_len;

    int out_len = end_byte - begin_byte;
    if (out_len > 0) {
        fwrite(str + begin_byte, 1, out_len, stdout);
    }
    putchar('\n');
}

// END OF CODE



int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: utf8analyzer \"<UTF-8 encoded string>\"\n");
        return 1;
    }

    // implement the UTF-8 analyzer here
}

