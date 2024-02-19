#include "sha256.c"

#include "../os/windows.c"

typedef struct test test;

struct test {
    char* message;
    u8 hash[32];
};

test tests[3];