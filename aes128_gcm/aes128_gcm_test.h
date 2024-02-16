#include "../os/windows.c"

#if defined slice_bounds_checking
    #define assert(expression) if (!(expression)) { trigger_breakpoint(); }
#else
    #define assert(...)
#endif

#if defined aes128_gcm_x64 
    #include "aes128_gcm_x64.c"
#elif defined aes128_gcm_unsafe
    #include "aes128_gcm_unsafe.c"
#endif

typedef struct aes128_bag aes128_bag;
typedef struct aes128_gcm_bag aes128_gcm_bag;

struct aes128_bag {
    u8 plaintext[16];
    u8 ciphertext[16];
    u8 key[16];
};

struct aes128_gcm_bag {
    slice plaintext;
    slice ciphertext;
    u8 key[16];
    u8 nonce[12];
    slice auth;
    u8 auth_tag[16];
};

#define aes128_bags_length 5
aes128_bag aes128_bags[aes128_bags_length];

#define aes128_gcm_bags_length 5
aes128_gcm_bag aes128_gcm_bags[aes128_gcm_bags_length];