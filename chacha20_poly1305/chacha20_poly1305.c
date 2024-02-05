#ifndef chacha20_poly1305_h
#define chacha20_poly1305_h

#include "chacha20_poly1305.h"

slice chacha20_poly1305_encrypt(slice plaintext, u8 key[32], u8 nonce[12], slice auth) {
    chacha20_cipher(plaintext, key, nonce, 1); 
    // plaintext is now ciphertext

    u8 auth_tag[16];
    chacha20_poly1305_auth_tag(
        auth_tag, 
        plaintext, 
        auth, 
        key, 
        nonce
    );

    for (size_t i = 0; i < 16; i += 1) {
        plaintext = append_item(plaintext, auth_tag[i]);
    }

    return plaintext;
}

int chacha20_poly1305_decrypt(slice ciphertext, u8 key[32], u8 nonce[12], slice auth, u8 auth_tag[16]) {
    u8 real_auth_tag[16];
    chacha20_poly1305_auth_tag(
        real_auth_tag, 
        ciphertext, 
        auth, 
        key, 
        nonce
    );

    u32 d = 0;
    for (size_t i = 0; i < 16; i += 1) {
        d |= real_auth_tag[i] ^ auth_tag[i];
    }

    chacha20_cipher(ciphertext, key, nonce, 1);

    return (1 & ((d - 1) >> 8)) - 1;
}

void chacha20_poly1305_auth_tag(
    u8 auth_tag[16], 
    slice ciphertext, 
    slice auth, 
    u8 key[32], 
    u8 nonce[12]
) {
    u8 poly1305_key[64];
    chacha20_block(poly1305_key, key, nonce, 0);

    poly1305_ctx ctx = poly1305_begin(poly1305_key);
    ctx = poly1305_run(ctx, auth);
    slice padding = slice_alloc(15);

    if (auth.length % 16) {
        padding.length = 16 - auth.length % 16;
    }
    else {
        padding.length = 0;
    }

    ctx = poly1305_run(ctx, padding);
    ctx = poly1305_run(ctx, ciphertext);

    if (ciphertext.length % 16) {
        padding.length = 16 - ciphertext.length % 16;
    }
    else {
        padding.length = 0;
    }

    ctx = poly1305_run(ctx, padding);

    padding.items[0] = (u8) (auth.length >> 0);
    padding.items[1] = (u8) (auth.length >> 8);
    padding.items[2] = (u8) (auth.length >> 16);
    padding.items[3] = (u8) (auth.length >> 24);
    padding.items[4] = (u8) (auth.length >> 32);
    padding.items[5] = (u8) (auth.length >> 40);
    padding.items[6] = (u8) (auth.length >> 48);
    padding.items[7] = (u8) (auth.length >> 56);
    padding.length = 8;

    ctx = poly1305_run(ctx, padding);

    padding.items[0] = (u8) (ciphertext.length >> 0);
    padding.items[1] = (u8) (ciphertext.length >> 8);
    padding.items[2] = (u8) (ciphertext.length >> 16);
    padding.items[3] = (u8) (ciphertext.length >> 24);
    padding.items[4] = (u8) (ciphertext.length >> 32);
    padding.items[5] = (u8) (ciphertext.length >> 40);
    padding.items[6] = (u8) (ciphertext.length >> 48);
    padding.items[7] = (u8) (ciphertext.length >> 56);
    padding.length = 8;

    ctx = poly1305_run(ctx, padding);
    poly1305_end(ctx, auth_tag, poly1305_key);
}

void chacha20_cipher(slice plaintext, u8 key[32], u8 nonce[12], u32 counter) {
    u32 block_count = (u32) plaintext.length / 64; // todo: safe cast
    size_t remainder = plaintext.length % 64;

    u8 key_stream[64];
    for (u32 i = 0; i < block_count; i += 1 ) {
        chacha20_block(key_stream, key, nonce, counter + i);
        for (size_t j = 0; j < 64; j += 1) {
            plaintext.items[j] ^= key_stream[j];
        }
        plaintext.items += 64;
    } 

    if (remainder) {
        chacha20_block(key_stream, key, nonce, counter + block_count);
        for (size_t i = 0; i < remainder; i += 1) {
            plaintext.items[i] ^= key_stream[i];
        }
    }
}

void chacha20_block(u8 key_stream[64], u8 key[32], u8 nonce[12], u32 counter) {
    u32 state[16];

    state[0] = 0x61707865;
    state[1] = 0x3320646e;
    state[2] = 0x79622d32;
    state[3] = 0x6b206574;

    state[4] = le_to_32(&key[0]);
    state[5] = le_to_32(&key[4]);
    state[6] = le_to_32(&key[8]);
    state[7] = le_to_32(&key[12]);
    state[8] = le_to_32(&key[16]);
    state[9] = le_to_32(&key[20]);
    state[10] = le_to_32(&key[24]);
    state[11] = le_to_32(&key[28]);

    state[12] = counter; // to little_endian?

    state[13] = le_to_32(&nonce[0]);
    state[14] = le_to_32(&nonce[4]);
    state[15] = le_to_32(&nonce[8]);

    u32 state_old[16];

    for (u8 i = 0; i < 16; i += 1) {
        state_old[i] = state[i];
    }

    for (u8 i = 0; i < 10; i += 1) {
        chacha_qr(&state[0], &state[4], &state[8], &state[12]);
        chacha_qr(&state[1], &state[5], &state[9], &state[13]);
        chacha_qr(&state[2], &state[6], &state[10], &state[14]);
        chacha_qr(&state[3], &state[7], &state[11], &state[15]);
        chacha_qr(&state[0], &state[5], &state[10], &state[15]);
        chacha_qr(&state[1], &state[6], &state[11], &state[12]);
        chacha_qr(&state[2], &state[7], &state[8], &state[13]);
        chacha_qr(&state[3], &state[4], &state[9], &state[14]);
    }

    for (u8 i = 0; i < 16; i += 1) {
        state[i] += state_old[i];
    }

    // to little-endian
    for (u8 i = 0; i < 16; i += 1) {
        key_stream[i * 4 + 0] = (u8) (state[i] >> 0);
        key_stream[i * 4 + 1] = (u8) (state[i] >> 8);
        key_stream[i * 4 + 2] = (u8) (state[i] >> 16);
        key_stream[i * 4 + 3] = (u8) (state[i] >> 24);
    }
}

void chacha_qr(u32* a, u32* b, u32* c, u32* d) {

    *a += *b;
    *d ^= *a;
    *d = rol(*d, 16);

    *c += *d;
    *b ^= *c;
    *b = rol(*b, 12);

    *a += *b;
    *d ^= *a;
    *d = rol(*d, 8);

    *c += *d;
    *b ^= *c;
    *b = rol(*b, 7);

}

// This is modeled after the crypto_onetimeauth function from tweetnacl.
// It's modified in a way that allows message processing in parts.
// I don't really understand what's going on with the modular arithmetic.
void poly1305(u8 out[16], slice message, u8 key[32]) {
    poly1305_ctx ctx = poly1305_begin(key);
    ctx = poly1305_run(ctx, message);
    poly1305_end(ctx, out, key);
}

poly1305_ctx poly1305_begin(u8 key[32]) {
    poly1305_ctx ctx;

    ctx.block_length = 0;

    for (size_t i = 0; i < 16; i += 1) {
        ctx.r[i] = key[i];
    }
    ctx.r[3] &= 0x0f;
    ctx.r[4] &= 0xfc;
    ctx.r[7] &= 0x0f;
    ctx.r[8] &= 0xfc;
    ctx.r[11] &= 0x0f;
    ctx.r[12] &= 0xfc;
    ctx.r[15] &= 0x0f;
    ctx.r[16] = 0;

    for (size_t i = 0; i < 17; i += 1) {
        ctx.acc[i] = 0;
    }

    return ctx;
}

poly1305_ctx poly1305_run(poly1305_ctx ctx, slice message) {
    for (size_t i = 0; i < message.length; i += 1) {
        ctx.block[ctx.block_length] = message.items[i];
        ctx.block_length += 1;
        if (ctx.block_length == 16) {
            ctx.block[16] = 1;
            ctx.block_length = 0;
            poly1305_process(ctx.acc, ctx.block, ctx.r);
        }
    }

    return ctx;
}

void poly1305_end(poly1305_ctx ctx, u8 out[16], u8 key[32]) {
    if (ctx.block_length) {
        ctx.block[ctx.block_length] = 1;
        ctx.block_length += 1;
        for (; ctx.block_length < 17; ctx.block_length += 1) {
            ctx.block[ctx.block_length] = 0;
        }
        poly1305_process(ctx.acc, ctx.block, ctx.r);
    }

    u32 acc_old[17];
    for (size_t i = 0; i < 17; i += 1) {
        acc_old[i] = ctx.acc[i];
    } 

    u32 p[17] = {
        5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 252
    };

    add_poly1305(ctx.acc, p);

    u32 s = -(ctx.acc[16] >> 7);
    for (size_t i = 0; i < 17; i += 1) {
        ctx.acc[i] ^= s & (acc_old[i] ^ ctx.acc[i]);
    } 

    for (size_t i = 0; i < 16; i += 1) {
        ctx.block[i] = key[i + 16];
    } 
    ctx.block[16] = 0;

    add_poly1305(ctx.acc, ctx.block);

    for (size_t i = 0; i < 16; i += 1) {
        out[i] = (u8) ctx.acc[i];
    } 
}

void poly1305_process(u32 acc[17], u32 block[17], u32 r[17]) {
    add_poly1305(acc, block);

    u32 prod[17];
    for (size_t i = 0; i < 17; i += 1) {
        prod[i] = 0;
        for (size_t j = 0; j < 17; j += 1) {
            prod[i] += acc[j] * ((j <= i) ? r[i - j] : 320 * r[i + 17 - j]);
        } 
    } 

    for (size_t i = 0; i < 17; i += 1) {
        acc[i] = prod[i];
    } 

    u32 u = 0;
    for (size_t i = 0; i < 16; i += 1) {
        u += acc[i];
        acc[i] = u & 255;
        u >>= 8;
    } 

    u += acc[16];
    acc[16] = u & 3;
    u = 5 * (u >> 2);

    for (size_t i = 0; i < 16; i += 1) {
        u += acc[i];
        acc[i] = u & 255;
        u >>= 8;
    } 

    u += acc[16];
    acc[16] = u;
}

void add_poly1305(u32 a[17], u32 b[17]) {
    u32 u = 0;
    for (size_t j = 0; j < 17; j += 1) {
        u += a[j] + b[j];
        a[j] = u & 255;
        u >>= 8;
    }
}

#endif