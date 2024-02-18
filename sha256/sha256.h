#include "../basic/basic.c"

#define sha256_hash_length 32

typedef struct sha256_ctx sha256_ctx;

u32 round_constants[64];

void sha256(u8[32], slice);
void sha256_string(u8[32], char*);
sha256_ctx sha256_begin(void);
sha256_ctx sha256_run(sha256_ctx, slice);
void sha256_end(sha256_ctx, u8[32]);
sha256_ctx sha256_hash(sha256_ctx);

struct sha256_ctx {
    size_t message_length;
    u32 state[8];
    u8 block_length;
    u8 block[64];
};