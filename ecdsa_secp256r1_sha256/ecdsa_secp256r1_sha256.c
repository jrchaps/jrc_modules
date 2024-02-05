#ifndef ecdsa_secp256_sha256_h
#define ecdsa_secp256_sha256_h

#include "ecdsa_secp256r1_sha256.h"

int default_CSPRNG(u8* out, u32 length) {
    crypto_random(out, length);
    return 1;
}

slice ecdsa_secp256r1_sha256(slice der_signature, u8 private_key[32], slice message) {
    u8 message_hash[32];
    sha256(message_hash, message);

    u8 signature[64];
    uECC_Curve curve = uECC_secp256r1();
    int success = uECC_sign(
        private_key,
        message_hash,
        32,
        signature,
        curve
    );
    assert(success);

    // https://bitcoin.stackexchange.com/questions/92680/what-are-the-der-signature-and-sec-format

    u8 length = 68;
    u8 r_length = 32;
    u8 s_length = 32;

    if (signature[0] > 0x7f) {
        length += 1;
        r_length += 1;
    }

    if (signature[32] > 0x7f) {
        length += 1;
        s_length += 1;
    }

    der_signature = copy_item(der_signature, 0x30);
    der_signature = append_item(der_signature, length);

    der_signature = append_item(der_signature, 0x02);
    der_signature = append_item(der_signature, r_length);
    if (r_length == 33) {
        der_signature = append_item(der_signature, 0);
    }
    der_signature = append_length(der_signature, signature, 32);

    der_signature = append_item(der_signature, 0x02);
    der_signature = append_item(der_signature, s_length);
    if (s_length == 33) {
        der_signature = append_item(der_signature, 0);
    }
    der_signature = append_length(der_signature, signature + 32, 32);

    return der_signature;
}

#endif