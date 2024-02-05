#include "../sha256/sha256.c"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include "tinycrypt/lib/source/ecc.c"
#include "tinycrypt/lib/source/ecc_dsa.c"
#pragma clang diagnostic pop

void crypto_random(u8*, u32);