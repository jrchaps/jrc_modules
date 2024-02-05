#ifndef string_h
#define string_h

#include "string.h"

void string_copy(char* out, char* in) {
    while (*in != '\0') {
        *out = *in;
        out += 1;
        in += 1;
    }
    *out = '\0';
}

void string_append(char* out, char* in) {
    while (*out != '\0') {
        out += 1;
    } 
    string_copy(out, in);
}

size_t string_length(char* in) {
    size_t length = 0;
    while (in[length] != '\0') {
        // todo: assert
        length += 1;
    }
    return length;
}

#endif