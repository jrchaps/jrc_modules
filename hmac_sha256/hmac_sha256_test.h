#include "../os/windows.c"
#include "hmac_sha256.c"

typedef struct test test;

struct test {
    u8 hash[32];
    slice key;
    slice message;
};

test tests[3];