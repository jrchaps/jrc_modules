#include "../sha256/sha256.c"

typedef struct hmac_sha256_ctx hmac_sha256_ctx;

struct hmac_sha256_ctx {
    sha256_ctx hash_ctx;
    u8 outer_pad[64];
};