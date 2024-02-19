#include "hmac_sha256.c"

#include "../os/windows.c"

typedef struct test test;

struct test {
    u8 hash[32];
    slice key;
    slice message;
};

test tests[3];