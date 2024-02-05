#ifndef sha256_h
#define sha256_h

#include "sha256.h"

void sha256(u8 hash[32], slice message) {
    sha256_ctx ctx = sha256_begin();
    ctx = sha256_run(ctx, message);
    sha256_end(ctx, hash);
}

void sha256_string(u8 hash[32], char* message) {
    sha256(
        hash,
        string_to_slice(message)
    );
}

sha256_ctx sha256_begin() {
    sha256_ctx ctx = {
        .state = {
            0x6a09e667,
            0xbb67ae85,
            0x3c6ef372,
            0xa54ff53a,
            0x510e527f,
            0x9b05688c,
            0x1f83d9ab,
            0x5be0cd19,
        }
    };
    return ctx;
}

sha256_ctx sha256_run(sha256_ctx ctx, slice message) {
    ctx.message_length += message.length;

    for (size_t i = 0; i < message.length; i += 1) {
        ctx.block[ctx.block_length] = message.items[i];
        ctx.block_length += 1;
        if (ctx.block_length == 64) {
            ctx.block_length = 0;
            ctx = sha256_hash(ctx);
        }
    }
    return ctx;
}

void sha256_end(sha256_ctx ctx, u8 hash[32]) {

    ctx.block[ctx.block_length] = 0x80; // 1000 0000
    ctx.block_length += 1;

    if (ctx.block_length > 56) {
        for (; ctx.block_length < 64; ctx.block_length += 1) {
            ctx.block[ctx.block_length] = 0;
        }
        ctx.block_length = 0;
        ctx = sha256_hash(ctx);
    }

    for (; ctx.block_length < 56; ctx.block_length += 1) {
        ctx.block[ctx.block_length] = 0;
    }

    u64 bit_count = ctx.message_length * 8;
    // to big-endian
    ctx.block[56] = (u8) (bit_count >> 56);
    ctx.block[57] = (u8) (bit_count >> 48);
    ctx.block[58] = (u8) (bit_count >> 40);
    ctx.block[59] = (u8) (bit_count >> 32);
    ctx.block[60] = (u8) (bit_count >> 24);
    ctx.block[61] = (u8) (bit_count >> 16);
    ctx.block[62] = (u8) (bit_count >> 8);
    ctx.block[63] = (u8) (bit_count >> 0);

    ctx = sha256_hash(ctx);

    // to big-endian
    for (u8 i = 0; i < 8; i += 1) {
        hash[i * 4 + 0] = (u8) (ctx.state[i] >> 24);
        hash[i * 4 + 1] = (u8) (ctx.state[i] >> 16);
        hash[i * 4 + 2] = (u8) (ctx.state[i] >> 8);
        hash[i * 4 + 3] = (u8) (ctx.state[i] >> 0);
    }
}

sha256_ctx sha256_hash(sha256_ctx ctx) {
    u32 schedule[64];

    for (u8 j = 0; j < 16; j += 1) {
        schedule[j] = be_to_u32(&ctx.block[j * 4]);
    }

    for (u8 j = 16; j < 64; j += 1) {
        u32 s0 = (
            ror(schedule[j - 15], 7) ^
            ror(schedule[j - 15], 18) ^
            (schedule[j - 15] >> 3)
        );

        u32 s1 = (
            ror(schedule[j - 2], 17) ^
            ror(schedule[j - 2], 19) ^
            (schedule[j - 2] >> 10)
        );

        schedule[j] = (
            s0 +
            s1 +
            schedule[j - 16] + 
            schedule[j - 7]
        );
    }

    u32 a = ctx.state[0];
    u32 b = ctx.state[1];
    u32 c = ctx.state[2];
    u32 d = ctx.state[3];
    u32 e = ctx.state[4];
    u32 f = ctx.state[5];
    u32 g = ctx.state[6];
    u32 h = ctx.state[7];

    for (u8 j = 0; j < 64; j += 1) {
        u32 s0 = (
            ror(a, 2) ^
            ror(a, 13) ^
            ror(a, 22)
        );

        u32 s1 = (
            ror(e, 6) ^
            ror(e, 11) ^
            ror(e, 25)
        );

        u32 maj = (a & b) ^ (a & c) ^ (b & c);
        u32 ch = (e & f) ^ ((~e) & g); 

        u32 temp1 = (
            s1 +
            ch +
            h +
            round_constants[j] + 
            schedule[j]
        );

        u32 temp2 = s0 + maj;

        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }

    ctx.state[0] += a;
    ctx.state[1] += b;
    ctx.state[2] += c;
    ctx.state[3] += d;
    ctx.state[4] += e;
    ctx.state[5] += f;
    ctx.state[6] += g;
    ctx.state[7] += h;

    return ctx;
}

u32 round_constants[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

#endif