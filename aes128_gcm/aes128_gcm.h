#include "../basic/basic.c"

typedef struct aes128_gcm_decrypt_output aes128_gcm_decrypt_output;

struct aes128_gcm_decrypt_output {
    u64 is_authentic;
    slice plaintext;
};