#ifndef aes128_gcm_x64_h
#define aes128_gcm_x64_h

#include "aes128_gcm_x64.h"

#define aes128_get_round_key(out, key, round_constant) {           \
    __m128i temp = _mm_aeskeygenassist_si128(key, round_constant); \
    __m128i temp2;                                                 \
    temp = _mm_shuffle_epi32(temp, 0xff);                          \
    temp2 = _mm_slli_si128(key, 0x4);                              \
    key = _mm_xor_si128(key, temp2);                               \
    temp2 = _mm_slli_si128(temp2, 0x4);                            \
    key = _mm_xor_si128(key, temp2);                               \
    temp2 = _mm_slli_si128(temp2, 0x4);                            \
    key = _mm_xor_si128(key, temp2);                               \
    key = _mm_xor_si128(key, temp);                                \
    out = key;                                                     \
}

void aes128_get_round_keys(__m128i out[11], u8 initial_key[16]) {
    __m128i key = _mm_loadu_si128((void*) initial_key);
    out[0] = key;
    aes128_get_round_key(out[1], key, 0x01);
    aes128_get_round_key(out[2], key, 0x02);
    aes128_get_round_key(out[3], key, 0x04);
    aes128_get_round_key(out[4], key, 0x08);
    aes128_get_round_key(out[5], key, 0x10);
    aes128_get_round_key(out[6], key, 0x20);
    aes128_get_round_key(out[7], key, 0x40);
    aes128_get_round_key(out[8], key, 0x80);
    aes128_get_round_key(out[9], key, 0x1b);
    aes128_get_round_key(out[10], key, 0x36);
}

__m128i aes128_encrypt(__m128i state, __m128i round_keys[11]) {
    state = _mm_xor_si128(state, round_keys[0]);

    for (size_t i = 1; i < 10; i += 1) {
        state = _mm_aesenc_si128(state, round_keys[i]);
    }

    state = _mm_aesenclast_si128(state, round_keys[10]);

    return state;
}

__m128i gcm_mul(__m128i a, __m128i b) {

    __m128i mask = _mm_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);

    a = _mm_shuffle_epi8(a, mask);
    b = _mm_shuffle_epi8(b, mask);

    __m128i T0, T1, T2, T3, T4, T5;

    T0 = _mm_clmulepi64_si128(a, b, 0x00);
    T1 = _mm_clmulepi64_si128(a, b, 0x01);
    T2 = _mm_clmulepi64_si128(a, b, 0x10);
    T3 = _mm_clmulepi64_si128(a, b, 0x11);

    T1 = _mm_xor_si128(T1, T2);
    T2 = _mm_slli_si128(T1, 8);
    T1 = _mm_srli_si128(T1, 8);
    T0 = _mm_xor_si128(T0, T2);
    T3 = _mm_xor_si128(T3, T1);

    T4 = _mm_srli_epi32(T0, 31);
    T0 = _mm_slli_epi32(T0, 1);

    T5 = _mm_srli_epi32(T3, 31);
    T3 = _mm_slli_epi32(T3, 1);

    T2 = _mm_srli_si128(T4, 12);
    T5 = _mm_slli_si128(T5, 4);
    T4 = _mm_slli_si128(T4, 4);
    T0 = _mm_or_si128(T0, T4);
    T3 = _mm_or_si128(T3, T5);
    T3 = _mm_or_si128(T3, T2);

    T4 = _mm_slli_epi32(T0, 31);
    T5 = _mm_slli_epi32(T0, 30);
    T2 = _mm_slli_epi32(T0, 25);

    T4 = _mm_xor_si128(T4, T5);
    T4 = _mm_xor_si128(T4, T2);
    T5 = _mm_srli_si128(T4, 4);
    T3 = _mm_xor_si128(T3, T5);
    T4 = _mm_slli_si128(T4, 12);
    T0 = _mm_xor_si128(T0, T4);
    T3 = _mm_xor_si128(T3, T0);

    T4 = _mm_srli_epi32(T0, 1);
    T1 = _mm_srli_epi32(T0, 2);
    T2 = _mm_srli_epi32(T0, 7);
    T3 = _mm_xor_si128(T3, T1);
    T3 = _mm_xor_si128(T3, T2);
    T3 = _mm_xor_si128(T3, T4);

    T3 = _mm_shuffle_epi8(T3, mask);

    return T3;
}

__m128i aes128_gcm_xcrypt(
    slice xtext,
    u8 key[16],
    u8 nonce[12],
    slice auth,
    u64 is_encrypting
) {
    __m128i round_keys[11];
    aes128_get_round_keys(round_keys, key);

    __m128i zeros = _mm_setzero_si128();
    __m128i hash_key = aes128_encrypt(zeros, round_keys);

    __m128i auth_tag = _mm_setzero_si128();

    u32 block_count = (u32) auth.length / 16;
    u32 remainder = (u32) auth.length % 16;
    
    for (u32 i = 0; i < block_count; i += 1) {
        __m128i auth_block = _mm_loadu_si128((void*) auth.items);
        auth.items += 16;
        auth_tag = _mm_xor_si128(auth_tag, auth_block);
        auth_tag = gcm_mul(auth_tag, hash_key);
    }

    if (remainder) {
        u8 partial_block[16] = { 0 };
        for (size_t i = 0; i < remainder; i += 1) {
            partial_block[i] = auth.items[i];
        }
        auth_tag = _mm_xor_si128(
            auth_tag, 
            _mm_loadu_si128((void*) partial_block)
        );

        auth_tag = gcm_mul(auth_tag, hash_key);
    }

    u32 counter = 2;
    __m128i nonce_and_counter = _mm_setr_epi8(
        (char) nonce[0],
        (char) nonce[1],
        (char) nonce[2],
        (char) nonce[3],
        (char) nonce[4],
        (char) nonce[5],
        (char) nonce[6],
        (char) nonce[7],
        (char) nonce[8],
        (char) nonce[9],
        (char) nonce[10],
        (char) nonce[11],
        0,
        0,
        0,
        2
    );

    __m128i key_stream = aes128_encrypt(nonce_and_counter, round_keys);

    block_count = (u32) xtext.length / 16;
    remainder = (u32) xtext.length % 16;

    for (u32 i = 0; i < block_count; i += 1) {

        __m128i xtext_block = _mm_loadu_si128((void*) xtext.items);
        if (is_encrypting) {
            xtext_block = _mm_xor_si128(xtext_block, key_stream);
            auth_tag = _mm_xor_si128(auth_tag, xtext_block);
        }
        else {
            auth_tag = _mm_xor_si128(auth_tag, xtext_block);
            xtext_block = _mm_xor_si128(xtext_block, key_stream);
        }
        _mm_storeu_si128((void*) xtext.items, xtext_block);
        xtext.items += 16;

        counter += 1;
        nonce_and_counter = _mm_insert_epi32(
            nonce_and_counter, 
            endian_swap_u32(counter), 
            3
        );

        key_stream = aes128_encrypt(nonce_and_counter, round_keys);

        auth_tag = gcm_mul(auth_tag, hash_key);
    }

    if (remainder) {
        u8 key_stream_items[16];
        _mm_storeu_si128((void*) key_stream_items, key_stream);
        u8 partial_block[16] = { 0 };

        for (u32 i = 0; i < remainder; i += 1) {
            if (is_encrypting) {
                xtext.items[i] ^= key_stream_items[i];
                partial_block[i] = xtext.items[i];
            }
            else {
                partial_block[i] = xtext.items[i];
                xtext.items[i] ^= key_stream_items[i];
            }
        }

        auth_tag = _mm_xor_si128(
            auth_tag, 
            _mm_loadu_si128((void*) partial_block)
        );

        auth_tag = gcm_mul(auth_tag, hash_key);
    }

    __m128i length_a_b = _mm_setr_epi64(
        (__m64) endian_swap_u64(auth.length * 8),
        (__m64) endian_swap_u64(xtext.length * 8)
    );

    auth_tag = _mm_xor_si128(auth_tag, length_a_b);
    auth_tag = gcm_mul(auth_tag, hash_key);

    nonce_and_counter = _mm_insert_epi32(
        nonce_and_counter, 
        endian_swap_u32(1), 
        3
    );

    key_stream = aes128_encrypt(nonce_and_counter, round_keys);
    
    auth_tag = _mm_xor_si128(auth_tag, key_stream);

    return auth_tag;
}

slice aes128_gcm_encrypt(
    slice plaintext,
    u8 key[16],
    u8 nonce[12],
    slice auth
) {
    __m128i auth_tag = aes128_gcm_xcrypt(
        plaintext,
        key,
        nonce,
        auth,
        true
    );

    _mm_storeu_si128((void*) (plaintext.items + plaintext.length), auth_tag);
    plaintext.length += 16;

    return plaintext;
}

aes128_gcm_decrypt_output aes128_gcm_decrypt(
    slice ciphertext, 
    u8 key[16],
    u8 nonce[12],
    slice auth,
    u8 auth_tag[16]
) {
    aes128_gcm_decrypt_output out;

    __m128i real_auth_tag = aes128_gcm_xcrypt(
        ciphertext,
        key,
        nonce,
        auth,
        false 
    );

    out.plaintext = ciphertext;

    __m128i neq = _mm_xor_si128(real_auth_tag, _mm_loadu_si128((void*) auth_tag));
    out.is_authentic = (u64) _mm_test_all_zeros(neq, neq);

    return out;
}

#endif