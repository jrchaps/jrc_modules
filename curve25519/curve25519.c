#ifndef curve25519_h
#define curve25519_h

#include "curve25519.h"

void curve25519_scale(u8 prod[32], u8 a[32], u8 b[32]) {
    crypto_scalarmult(prod, a, b);   
}

void curve25519_scale_base(u8 prod[32], u8 n[32]) {
    crypto_scalarmult_base(prod, n);   
}

#endif