#ifndef basic_h
#define basic_h

#include "basic.h"

u32 ror(u32 in, u32 count) {
    return in >> count | in << (32 - count);
}

u32 rol(u32 in, u32 count) {
    return in << count | in >> (32 - count);
}

void u16_to_be(u8* out, u16 in) {
    out[0] = (u8) (in >> 8);
    out[1] = (u8) (in >> 0);
}

void u64_to_be(u8* out, u64 in) {
    out[0] = (u8) (in >> 56);
    out[1] = (u8) (in >> 48);
    out[2] = (u8) (in >> 40);
    out[3] = (u8) (in >> 32);
    out[4] = (u8) (in >> 24);
    out[5] = (u8) (in >> 16);
    out[6] = (u8) (in >> 8);
    out[7] = (u8) (in >> 0);
}

u16 be_to_u16(u8* in) {
    return (u16) (
        (u16) in[0] << 8 |
        (u16) in[1] << 0 
    );
}

u32 be_to_u32(u8* in) {
    return (
        (u32) in[0] << 24 |
        (u32) in[1] << 16 |
        (u32) in[2] << 8  |
        (u32) in[3] << 0 
    );
}

u32 le_to_32(u8* in) {
    return (
        (u32) in[0] << 0  |
        (u32) in[1] << 8  |
        (u32) in[2] << 16 |
        (u32) in[3] << 24
    );
}

int arrays_equal(u8* a, u8* b, size_t length) {
    size_t i = 0;
    while (i < length) {
        if (a[i] != b[i]) return false;
        i += 1;
    }
    return true;
}

#endif