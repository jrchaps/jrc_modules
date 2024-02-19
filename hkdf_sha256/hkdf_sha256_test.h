#include "hkdf_sha256.c"

#include "../os/windows.c"

typedef struct test test;

struct test {
    slice extract_key_in;
    slice salt;
    slice info;
    slice key_in;
    u8* key_out;
    u8 key_out_length;
};

test tests[3];