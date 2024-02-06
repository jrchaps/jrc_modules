#ifndef aes128_gcm_intrinsics_h
#define aes128_gcm_intrinsics_h

#include "aes128_gcm_x86-64.h"

// This should compile to single bswap instructions with optimization.
u32 swap_endian_u32(u32 a) {
    return (
        a >> 24 & 0x000000ff |
        a >> 8  & 0x0000ff00 |
        a << 8  & 0x00ff0000 |
        a << 24 & 0xff000000
    );
}

u64 swap_endian_u64(u64 a) {
    return (
        a >> 56 & 0x00000000000000ff |
        a >> 40 & 0x000000000000ff00 |
        a >> 24 & 0x0000000000ff0000 |
        a >> 8  & 0x00000000ff000000 |
        a << 8  & 0x000000ff00000000 |
        a << 24 & 0x0000ff0000000000 |
        a << 40 & 0x00ff000000000000 |
        a << 56 & 0xff00000000000000
    );
}

slice_u8_u32 aes128_gcm_encrypt(
    slice_u8_u32 plaintext, 
    t16_u8 key, 
    t12_u8 nonce, 
    slice_u8_u32 auth
) {

    t16_u8 auth_tag = aes128_gcm_xcrypt(
        plaintext,
        key,
        nonce,
        auth,
        true
    );

    _mm_storeu_si128((__m128i*) (plaintext.items + plaintext.length), auth_tag.t1);
    plaintext.length += 16;

    return plaintext;
}

aes128_gcm_decrypt_result aes128_gcm_decrypt(
    slice_u8_u32 ciphertext, 
    t16_u8 key, 
    t12_u8 nonce, 
    slice_u8_u32 auth, 
    t16_u8 auth_tag
) {

    t16_u8 real_auth_tag = aes128_gcm_xcrypt(
        ciphertext,
        key,
        nonce,
        auth,
        false
    );

    u64 is_authentic;
    __m128i neq = _mm_xor_si128(real_auth_tag.t1, auth_tag.t1);
    if(_mm_test_all_zeros(neq, neq)) {
        is_authentic = true;
    }
    else {
        is_authentic = false;
    }

    return (aes128_gcm_decrypt_result) { is_authentic, ciphertext };
}

t16_u8 aes128_gcm_xcrypt(
    slice_u8_u32 xtext,
    t16_u8 key,
    t12_u8 nonce,
    slice_u8_u32 auth,
    u64 is_encrypting
) {

    aes128_round_keys keys = aes128_get_round_keys(key);

    t16_u8 hash_key = aes128_encrypt((t16_u8) { 0 }, keys);

    t16_u8 auth_tag;
    auth_tag.t1 = _mm_setzero_si128();

    u32 block_count = auth.length / 16;
    u32 remainder = auth.length % 16;
    
    for (u32 i = 0; i < block_count; i += 1) {
        __m128i auth_block = _mm_loadu_si128((__m128i*) auth.items);
        auth_tag.t1 = _mm_xor_si128(auth_tag.t1, auth_block);
        auth_tag.t1 = gcm_mul(auth_tag.t1, hash_key.t1);
        auth.items += 16;
    }

    if (remainder) {
        for (u32 i = 0; i < remainder; i += 1) {
            auth_tag.items[i] ^= auth.items[i];
        }
        auth_tag.t1 = gcm_mul(auth_tag.t1, hash_key.t1);
    }

    t16_u8 nonce_and_counter; 
    nonce_and_counter.t1 = _mm_loadu_si128((__m128i*) nonce.items);
    u32 counter = 2;
    nonce_and_counter.t1 = _mm_insert_epi32(nonce_and_counter.t1, swap_endian_u32(counter), 3);
    t16_u8 countertext = aes128_encrypt(nonce_and_counter, keys);

    block_count = xtext.length / 16;
    remainder = xtext.length % 16;

    for (u32 i = 0; i < block_count; i += 1) {

        __m128i xtext_block = _mm_loadu_si128((__m128i*) xtext.items);

        if (is_encrypting) {
            xtext_block = _mm_xor_si128(xtext_block, countertext.t1);
            auth_tag.t1 = _mm_xor_si128(auth_tag.t1, xtext_block);
        }
        else {
            auth_tag.t1 = _mm_xor_si128(auth_tag.t1, xtext_block);
            xtext_block = _mm_xor_si128(xtext_block, countertext.t1);
        }

        _mm_storeu_si128((__m128i*) xtext.items, xtext_block);

        auth_tag.t1 = gcm_mul(auth_tag.t1, hash_key.t1);

        counter += 1;
        nonce_and_counter.t1 = _mm_insert_epi32(nonce_and_counter.t1, swap_endian_u32(counter), 3);
        countertext = aes128_encrypt(nonce_and_counter, keys);

        xtext.items += 16;
    }

    if (remainder) {
        for (u32 i = 0; i < remainder; i += 1) {
            if (is_encrypting) {
                xtext.items[i] ^= countertext.items[i];
                auth_tag.items[i] ^= xtext.items[i];
            }
            else {
                auth_tag.items[i] ^= xtext.items[i];
                xtext.items[i] ^= countertext.items[i];
            }
        } 
        auth_tag.t1 = gcm_mul(auth_tag.t1, hash_key.t1);
    }

    u64 length_a = swap_endian_u64(auth.length * 8);
    u64 length_b = swap_endian_u64(xtext.length * 8);

    __m128i length_a_b = _mm_cvtsi64_si128((long long) length_a);
    length_a_b = _mm_insert_epi64(length_a_b, length_b, 1);
    auth_tag.t1 = _mm_xor_si128(auth_tag.t1, length_a_b);

    auth_tag.t1 = gcm_mul(auth_tag.t1, hash_key.t1);

    nonce_and_counter.t1 = _mm_insert_epi32(nonce_and_counter.t1, swap_endian_u32(1), 3);
    nonce_and_counter = aes128_encrypt(nonce_and_counter, keys);
    
    auth_tag.t1 = _mm_xor_si128(auth_tag.t1, nonce_and_counter.t1);

    return auth_tag;
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

t16_u8 aes128_encrypt(t16_u8 state, aes128_round_keys keys) {
    state.t1 = _mm_xor_si128(state.t1, keys.keys[0].t1);

    for (u8_fast i = 1; i < 10; i += 1) {
        state.t1 = _mm_aesenc_si128(state.t1, keys.keys[i].t1);
    }

    state.t1 = _mm_aesenclast_si128(state.t1, keys.keys[10].t1);

    return state;
}

aes128_round_keys aes128_get_round_keys(t16_u8 key) {
    aes128_round_keys keys;

    keys.keys[0] = key;

    __m128i temp = _mm_aeskeygenassist_si128(key.t1, 0x01);
    key.t1 = aes128_round_key(key.t1, temp);
    keys.keys[1] = key;

    temp = _mm_aeskeygenassist_si128(key.t1, 0x02);
    key.t1 = aes128_round_key(key.t1, temp);
    keys.keys[2] = key;

    temp = _mm_aeskeygenassist_si128(key.t1, 0x04);
    key.t1 = aes128_round_key(key.t1, temp);
    keys.keys[3] = key;

    temp = _mm_aeskeygenassist_si128(key.t1, 0x08);
    key.t1 = aes128_round_key(key.t1, temp);
    keys.keys[4] = key;

    temp = _mm_aeskeygenassist_si128(key.t1, 0x10);
    key.t1 = aes128_round_key(key.t1, temp);
    keys.keys[5] = key;

    temp = _mm_aeskeygenassist_si128(key.t1, 0x20);
    key.t1 = aes128_round_key(key.t1, temp);
    keys.keys[6] = key;

    temp = _mm_aeskeygenassist_si128(key.t1, 0x40);
    key.t1 = aes128_round_key(key.t1, temp);
    keys.keys[7] = key;

    temp = _mm_aeskeygenassist_si128(key.t1, 0x80);
    key.t1 = aes128_round_key(key.t1, temp);
    keys.keys[8] = key;

    temp = _mm_aeskeygenassist_si128(key.t1, 0x1b);
    key.t1 = aes128_round_key(key.t1, temp);
    keys.keys[9] = key;

    temp = _mm_aeskeygenassist_si128(key.t1, 0x36);
    key.t1 = aes128_round_key(key.t1, temp);
    keys.keys[10] = key;

    return keys;
}

__m128i aes128_round_key(__m128i temp1, __m128i temp2) {
    __m128i temp3;
    temp2 = _mm_shuffle_epi32(temp2, 0xff);
    temp3 = _mm_slli_si128(temp1, 0x4);
    temp1 = _mm_xor_si128(temp1, temp3);
    temp3 = _mm_slli_si128(temp3, 0x4);
    temp1 = _mm_xor_si128(temp1, temp3);
    temp3 = _mm_slli_si128(temp3, 0x4);
    temp1 = _mm_xor_si128(temp1, temp3);
    temp1 = _mm_xor_si128(temp1, temp2);
    return temp1;
}

#endif