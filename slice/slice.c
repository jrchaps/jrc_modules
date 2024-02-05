#ifndef slice_h
#define slice_h

#include "slice.h"

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