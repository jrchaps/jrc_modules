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

void u32_to_be(u8* out, u32 in) {
    out[0] = (u8) (in >> 24);
    out[1] = (u8) (in >> 16);
    out[2] = (u8) (in >> 8);
    out[3] = (u8) (in >> 0);
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

u64 be_to_u64(u8* in) {
    return (
        (u64) in[0] << 56 |
        (u64) in[1] << 48 |
        (u64) in[2] << 40 |
        (u64) in[3] << 32 |
        (u64) in[0] << 24 |
        (u64) in[1] << 16 |
        (u64) in[2] << 8  |
        (u64) in[3] << 0 
    );
}

void u16_to_le(u8* out, u16 in) {
    out[0] = (u8) (in >> 0);
    out[1] = (u8) (in >> 8);
}

void u32_to_le(u8* out, u32 in) {
    out[0] = (u8) (in >> 0);
    out[1] = (u8) (in >> 8);
    out[2] = (u8) (in >> 16);
    out[3] = (u8) (in >> 24);
}

void u64_to_le(u8* out, u64 in) {
    out[0] = (u8) (in >> 0);
    out[1] = (u8) (in >> 8);
    out[2] = (u8) (in >> 16);
    out[3] = (u8) (in >> 24);
    out[4] = (u8) (in >> 32);
    out[5] = (u8) (in >> 40);
    out[6] = (u8) (in >> 48);
    out[7] = (u8) (in >> 56);
}

u16 le_to_u16(u8* in) {
    return (
        (u16) in[0] << 0  |
        (u16) in[1] << 8  |
    );
}

u32 le_to_u32(u8* in) {
    return (
        (u32) in[0] << 0  |
        (u32) in[1] << 8  |
        (u32) in[2] << 16 |
        (u32) in[3] << 24
    );
}

u64 le_to_u64(u8* in) {
    return (
        (u64) in[0] << 0  |
        (u64) in[1] << 8  |
        (u64) in[2] << 16 |
        (u64) in[3] << 24 |
        (u64) in[3] << 32 |
        (u64) in[3] << 40 |
        (u64) in[3] << 48 |
        (u64) in[3] << 56
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
        length += 1;
    }
    return length;
}

slice slice_cut(slice in, size_t start_index, size_t end_index) {
    assert(end_index >= start_index);
    assert(in.capacity >= end_index);
    in.items += start_index;
    in.length = end_index - start_index;
    #ifdef slice_bounds_checking
        in.capacity -= start_index;
    #endif
    return in;
}

slice slice_clear(slice in) {
    slice out = slice_cut(in, 0, 0);
    return out;
}

slice slice_up(slice in, size_t length) {
    slice out = slice_cut(in, 0, in.length + length);
    return out; 
}

slice slice_bump(slice in, size_t length) {
    slice out = slice_cut(in, length, length);
    return out;
}

slice slice_end(slice in) {
    slice out = slice_bump(in, in.length);
    return out;
}

slice next(slice* in, size_t length) {
    if (length > in->length) {
        length = in->length;
    }

    slice out = slice_make(length, length, in->items);

    *in = slice_cut(
        *in,
        length,
        in->length
    );

    return out;
}

int slices_equal(slice a, slice b) {
    if (a.length != b.length) return false;
    size_t i = 0;
    while (i < a.length) {
        if (a.items[i] != b.items[i]) return false;
        i += 1;
    }
    return true;
}

slice slice_around(slice* in, slice separator) {
    slice out = slice_clear(*in);

    while (true) {
        if (separator.length > in->length) {
            out = slice_up(out, in->length);
            *in = slice_end(*in);
            return out;
        }

        slice token = slice_cut(*in, 0, separator.length);

        if (slices_equal(token, separator)) {
            *in = slice_cut(*in, separator.length, in->length);
            return out;
        }

        out = slice_up(out, 1);
        *in = slice_cut(*in, 1, in->length);
    }
}

slice reverse(slice out) {
    for (size_t i = 0; i < out.length / 2; i += 1) {
        u8 temp = out.items[i];
        out.items[i] = out.items[out.length - 1 - i];
        out.items[out.length - 1 - i] = temp;
    }
    return out;
}

slice append_item(slice out, u8 in) {
    assert(out.length < out.capacity); 
    out.items[out.length] = in;
    out.length += 1;
    return out;
}

slice copy_item(slice out, u8 in) {
    out.length = 0;
    out = append_item(out, in);
    return out;
}

slice append_length(slice out, u8* in, size_t length) {
    for (size_t i = 0; i < length; i += 1) {
        out = append_item(out, in[i]);
    }
    return out;
}

slice copy(slice out, slice in) {
    out.length = 0;
    while (out.length < in.length) {
        out = append_item(out, in.items[out.length]);
    } 
    return out;
}

slice append(slice out, slice in) {
    for (size_t i = 0; i < in.length; i += 1) {
        out = append_item(out, in.items[i]);
    }
    return out;
}

slice append_ascii_decimal(slice out, u64 in) {
    slice decimal = slice_bump(out, out.length);
    u8 ascii_decimals[] = { 48, 49, 50, 51, 52, 53, 54, 55, 56, 57 };
    while (true) {
        decimal = append_item(decimal, ascii_decimals[in % 10]);
        in /= 10;
        if (!in) {
            break;
        }
    }
    decimal = reverse(decimal);
    out = slice_up(out, decimal.length);
    return out;
}

slice string_to_slice(char* in) {
    slice result = slice_make(
        string_length(in),
        string_length(in),
        (u8*) in
    );
    return result;
}

#endif