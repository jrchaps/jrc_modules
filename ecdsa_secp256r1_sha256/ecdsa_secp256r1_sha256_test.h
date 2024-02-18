#include "../os/windows.c"

#if defined slice_bounds_checking
    #define assert(expression) if (!(expression)) { trigger_breakpoint(); }
#else
    #define assert(...)
#endif

#include "ecdsa_secp256r1_sha256.c"