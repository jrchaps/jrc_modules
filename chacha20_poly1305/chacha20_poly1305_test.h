#include "chacha20_poly1305.c"

#include "../os/windows.c"

typedef struct test test;

struct test {
    slice plaintext;
    slice ciphertext;
    u8 key[32];
    u8 nonce[12];
    slice auth;
    u8 auth_tag[16];
};

test tests[1];