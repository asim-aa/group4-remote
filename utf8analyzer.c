#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: utf8analyzer \"<UTF-8 encoded string>\"\n");
        return 1;
    }

//function 1
int checkIfASCII(char s[]) {
        int i = 0;
        while (s[i] != '\0') {
            if ((unsigned char)s[i] > 127) {
                return 0; //fail
            }
            i++;
        }
        return 1;
    }

//function 2

int capitalize_ascii(char str[]) {
        int count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] -= 32;  
            count++;
        }
    }
    return count;
}

