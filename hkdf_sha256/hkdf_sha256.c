#ifndef hkdf_sha256_h
#define hkdf_sha256_h

#include "hkdf_sha256.h"

void hkdf_extract_sha256(u8 new_key[32], slice key, slice salt) {
    hmac_sha256(new_key, salt, key);
}

void hkdf_expand_sha256(u8* new_key, u8 new_key_length, slice key, slice info) {

    u8 block_count = new_key_length / 32;
    u8 remainder = new_key_length % 32;
    new_key_length = 0;
    slice block = slice_alloc(32);

    for (u8 i = 0; i < block_count; i += 1) {
        hmac_sha256_ctx ctx = hmac_sha256_begin(key);
        ctx = hmac_sha256_run(ctx, block);
        ctx = hmac_sha256_run(ctx, info);
        ctx = hmac_sha256_run(ctx, (slice) slice_literal(i + 1));
        hmac_sha256_end(ctx, block.items);
        block.length = 32;
        for (u8 j = 0; j < 32; j += 1) {
            new_key[new_key_length] = block.items[j];
            new_key_length += 1;
        }
    }

    if (remainder) {
        hmac_sha256_ctx ctx = hmac_sha256_begin(key);
        ctx = hmac_sha256_run(ctx, block);
        ctx = hmac_sha256_run(ctx, info);
        ctx = hmac_sha256_run(ctx, (slice) slice_literal(block_count + 1));
        hmac_sha256_end(ctx, block.items);
        for (u8 j = 0; j < remainder; j += 1) {
            new_key[new_key_length] = block.items[j];
            new_key_length += 1;
        }
    }
}

#endif