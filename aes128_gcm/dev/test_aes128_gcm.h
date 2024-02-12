#include "../../os/windows.c"

#ifdef slice_bounds_checking
    #define assert(expression) if (!(expression)) { DebugBreak(); }
#endif

#include "../aes128_gcm_x64.c"

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