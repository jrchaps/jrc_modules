#include "../os/windows.c"

#if defined slice_bounds_checking
    #define assert(expression) if (!(expression)) { trigger_breakpoint(); }
#else
    #define assert(...)
#endif

#include "sha256.c"

typedef struct test test;

struct test {
    char* message;
    u8 hash[32];
};

test tests[3];