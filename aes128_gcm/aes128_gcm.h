#include "../types.h"
#include "../defines.h"
#include "../tuples/tuples.c"
#include "../slices/slices.c"

typedef struct aes128_gcm_decrypt_result aes128_gcm_decrypt_result;
typedef struct aes128_round_keys aes128_round_keys;

slice_u8_u32 aes128_gcm_encrypt(slice_u8_u32, t16_u8, t12_u8, slice_u8_u32);
aes128_gcm_decrypt_result aes128_gcm_decrypt(slice_u8_u32, t16_u8, t12_u8, slice_u8_u32, t16_u8);
t16_u8 aes128_gcm_xcrypt(slice_u8_u32, t16_u8, t12_u8, slice_u8_u32, u64);
t16_u8 gcm_mul(t16_u8, t16_u8);

t16_u8 aes128_encrypt(t16_u8, aes128_round_keys);
aes128_round_keys aes128_get_round_keys(t16_u8);
t16_u8 aes128_mix_columns(t16_u8);
t16_u8 aes128_round_key(t16_u8, u8_fast);
t16_u8 aes128_substitute(t16_u8);
t16_u8 aes128_shift_rows(t16_u8);
u8 aes_mul(u8, u8);

t4_u8 aes_round_constants[10];
u8 aes_substitution_box[256];

struct aes128_gcm_decrypt_result {
    u64 is_authentic;
    slice_u8_u32 plaintext;
};

struct aes128_round_keys {
    t16_u8 keys[11];
};