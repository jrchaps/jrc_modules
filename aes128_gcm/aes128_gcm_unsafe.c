#ifndef aes128_gcm_h
#define aes128_gcm_h

#include "aes128_gcm_unsafe.h"

u8 aes_substitution_box[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

u8 aes_round_constants[10] = {
    0x01, 0x02, 0x04, 0x08, 0x10,
    0x20, 0x40, 0x80, 0x1b, 0x36
};

void aes128_round_key(u8 key[16], size_t round_index) {
    u8 key_part[4];

    // RotWord
    key_part[0] = key[13];
    key_part[1] = key[14];
    key_part[2] = key[15];
    key_part[3] = key[12];

    // SubWord
    key_part[0] = aes_substitution_box[key_part[0]];
    key_part[1] = aes_substitution_box[key_part[1]];
    key_part[2] = aes_substitution_box[key_part[2]];
    key_part[3] = aes_substitution_box[key_part[3]];

    key_part[0] ^= aes_round_constants[round_index];

    for (size_t i = 0; i < 4; i += 1) {
        for (size_t j = 0; j < 4; j += 1) {
            key_part[j] ^= key[i * 4 + j];
            key[i * 4 + j] = key_part[j];
        }
    }
}

void aes128_get_round_keys(u8 round_keys[11][16], u8 initial_key[16]) {
    for (size_t i = 0; i < 16; i += 1) {
        round_keys[0][i] = initial_key[i];
    }

    for (size_t round_index = 0; round_index < 10; round_index += 1) {
        for (size_t i = 0; i < 16; i += 1) {
            round_keys[round_index + 1][i] = round_keys[round_index][i];
        }

        aes128_round_key(round_keys[round_index + 1], round_index);
    }
}

void aes128_substitute(u8 state[16]) {
    for (size_t i = 0; i < 16; i += 1) {
        state[i] = aes_substitution_box[state[i]];
    }
}

void aes128_shift_rows(u8 state[16]) {
    {
        u8 temp = state[1];
        state[1] = state[5];
        state[5] = state[9];
        state[9] = state[13];
        state[13] = temp;
    }
    {
        u8 temp = state[2];
        state[2] = state[10];
        state[10] = temp;
        temp = state[6];
        state[6] = state[14];
        state[14] = temp;
    }
    {
        u8 temp = state[15];
        state[15] = state[11];
        state[11] = state[7];
        state[7] = state[3];
        state[3] = temp;
    }
}

u8 aes_mul(u8 a, u8 b) {
    u8 product = 0;

    for (size_t i = 0; i < 8; i += 1) {
        if (b & 1) product ^= a;

        u8 highest_bit_set = (a & 0x80);
        a <<= 1;
        if (highest_bit_set) a ^= 0x1b;
        b >>= 1;
    }

    return product;
}

void aes128_mix_columns(u8 state[16]) {
    u8 matrix[16] = {
        2, 3, 1, 1,
        1, 2, 3, 1,
        1, 1, 2, 3,
        3, 1, 1, 2
    };

    u8 out[16] = { 0 };

    for (size_t i = 0; i < 4; i += 1) {
        for (size_t j = 0; j < 4; j += 1) {
            for (size_t k = 0; k < 4; k += 1) {
                out[i * 4 + j] ^= aes_mul(
                    state[i * 4 + k],
                    matrix[j * 4 + k]
                );
            }
        }
    }

    for (size_t i = 0; i < 16; i += 1) {
        state[i] = out[i];
    }
}

void aes128_encrypt(u8 plaintext[16], u8 round_keys[11][16]) {
    for (size_t round_index = 0 ;; round_index += 1) {
        // AddRoundKey
        for (size_t i = 0; i < 16; i += 1) {
            plaintext[i] ^= round_keys[round_index][i];
        }

        if (round_index == 9) {
            break;
        }

        // SubBytes
        aes128_substitute(plaintext);

        // ShiftRows
        aes128_shift_rows(plaintext);

        // MixColumns
        aes128_mix_columns(plaintext);
    }

    aes128_substitute(plaintext);
    aes128_shift_rows(plaintext);
    for (size_t i = 0; i < 16; i += 1) {
        plaintext[i] ^= round_keys[10][i];
    }
}

// Multiplies a by b in the GF(2^128). 
void gcm_mul(u8 a[16], u8 b[16]) {
    u8 product[16] = { 0 };

    u8 bit_mask = 128;
    u8 b_index = 0;

    for (u8 i = 0; i < 128; i += 1) {
        if (b[b_index] & bit_mask) {
            for (size_t i = 0; i < 16; i += 1) {
                product[i] ^= a[i];
            }
        }

        if (bit_mask & 0x01) {
            bit_mask = 128;
            b_index += 1;
        }
        else {
            bit_mask >>= 1;
        }

        u8 bit_128_is_set = (a[15] & 0x01);

        u8 prior_mask = 0;
        for (u8 i = 0; i < 16; i += 1) {
            u8 temp = a[i];
            a[i] >>= 1;
            a[i] |= prior_mask;
            prior_mask = (u8) ((temp & 0x01) << 7);
        }

        if (bit_128_is_set) {
            a[0] ^= 225; // 1110 0001 = R
        }
    }

    for (size_t i = 0; i < 16; i += 1) {
        a[i] = product[i];
    }
}

void aes128_gcm_xcrypt(
    slice xtext,
    u8 key[16],
    u8 nonce[12],
    slice auth,
    u8 auth_tag[16],
    u64 is_encrypting
) {
    u8 round_keys[11][16];
    aes128_get_round_keys(round_keys, key);

    u8 hash_key[16] = { 0 };
    aes128_encrypt(hash_key, round_keys);

    for (size_t i = 0; i < 16; i += 1) {
        auth_tag[i] = 0;
    }

    u32 auth_index = 0;
    u32 auth_tag_index = 0;

    while (auth_index < auth.length) {
        auth_tag[auth_tag_index] ^= auth.items[auth_index];
        auth_index += 1;
        auth_tag_index += 1;
        if (auth_tag_index == 16) {
            auth_tag_index = 0;
            gcm_mul(auth_tag, hash_key);
        }
    }

    if (auth_tag_index) {
        gcm_mul(auth_tag, hash_key);
    }

    u8 nonce_and_counter[16];
    u32 counter = 2;

    for (size_t i = 0; i < 12; i += 1) {
        nonce_and_counter[i] = nonce[i];
    }

    u32_to_be(nonce_and_counter + 12, counter);

    aes128_encrypt(nonce_and_counter, round_keys);

    u32 xtext_index = 0;
    u32 block_index = 0;

    while (xtext_index < xtext.length) {
        if (is_encrypting) {
            xtext.items[xtext_index] ^= nonce_and_counter[block_index];
            auth_tag[block_index] ^= xtext.items[xtext_index];
        }
        else {
            auth_tag[block_index] ^= xtext.items[xtext_index];
            xtext.items[xtext_index] ^= nonce_and_counter[block_index];
        }
        xtext_index += 1;
        block_index += 1;
        if (block_index == 16) {
            block_index = 0;

            gcm_mul(auth_tag, hash_key);

            counter += 1;

            for (size_t i = 0; i < 12; i += 1) {
                nonce_and_counter[i] = nonce[i];
            }

            u32_to_be(nonce_and_counter + 12, counter);

            aes128_encrypt(nonce_and_counter, round_keys);
        }
    }

    if (block_index) {
        gcm_mul(auth_tag, hash_key);
    }

    u8 length_a_b[16];
    u64_to_be(length_a_b, auth.length * 8);
    u64_to_be(length_a_b + 8, xtext.length * 8);

    for (size_t i = 0; i < 16; i += 1) {
        auth_tag[i] ^= length_a_b[i];
    }

    gcm_mul(auth_tag, hash_key);

    counter = 1;

    for (size_t i = 0; i < 12; i += 1) {
        nonce_and_counter[i] = nonce[i];
    }

    u32_to_be(nonce_and_counter + 12, counter);

    aes128_encrypt(nonce_and_counter, round_keys);

    for (size_t i = 0; i < 16; i += 1) {
        auth_tag[i] ^= nonce_and_counter[i];
    }
}

slice aes128_gcm_encrypt(
    slice plaintext,
    u8 key[16],
    u8 nonce[12],
    slice auth
) {
    u8 auth_tag[16];
    aes128_gcm_xcrypt(
        plaintext,
        key,
        nonce,
        auth,
        auth_tag,
        true
    );

    for (u8 i = 0; i < 16; i += 1) {
        plaintext.items[plaintext.length] = auth_tag[i];
        plaintext.length += 1;
    }

    return plaintext;
}

aes128_gcm_decrypt_output aes128_gcm_decrypt(
    slice ciphertext, 
    u8 key[16], 
    u8 nonce[12], 
    slice auth, 
    u8 auth_tag[16]
) {
    u8 real_auth_tag[16];
    aes128_gcm_xcrypt(
        ciphertext,
        key,
        nonce,
        auth,
        real_auth_tag,
        false
    );

    aes128_gcm_decrypt_output out;
    out.is_authentic = (u64) arrays_equal(real_auth_tag, auth_tag, 16);
    out.plaintext = ciphertext;

    return out;
}

#endif