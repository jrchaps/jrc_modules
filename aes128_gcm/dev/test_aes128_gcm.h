#include "Windows.h"

#ifdef slices_with_bounds_checking
    #define assert(expression, message) if (!(expression)) DebugBreak();
#endif
#ifdef no_intrinsics
    #include "../aes128_gcm.c"
#else
    #include "../aes128_gcm_x86-64.c"
#endif

typedef struct aes128_bag aes128_bag;
typedef struct aes128_gcm_bag aes128_gcm_bag;

struct aes128_bag {
    t16_u8 plaintext;
    t16_u8 ciphertext;
    t16_u8 key;
};

struct aes128_gcm_bag {
    slice_u8_u32 plaintext;
    slice_u8_u32 ciphertext;
    t16_u8 key;
    t12_u8 nonce;
    slice_u8_u32 auth;
    t16_u8 auth_tag;
};

#define aes128_bags_length 5
aes128_bag aes128_bags[aes128_bags_length];

#define aes128_gcm_bags_length 5
aes128_gcm_bag aes128_gcm_bags[aes128_gcm_bags_length];