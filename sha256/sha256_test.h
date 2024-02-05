#include "../os/windows.c"
#include "sha256.c"

typedef struct test test;

struct test {
    char* message;
    u8 hash[32];
};

test tests[3];