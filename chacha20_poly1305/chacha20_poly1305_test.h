#include "../os/windows.c"

#if defined slice_bounds_checking
    #define assert(expression) if (!(expression)) { trigger_breakpoint(); }
#else
    #define assert(...)
#endif

#include "chacha20_poly1305.c"

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