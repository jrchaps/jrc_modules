#include "ecdsa_secp256r1_sha256_test.h"

void start() {
    u8 private_key[32] = {
        0x19, 0x87, 0x11, 0x45, 0x30, 0x8A, 0x61, 0x0B, 
        0xF6, 0xC2, 0x60, 0x73, 0x89, 0xA0, 0xA1, 0x51, 
        0x83, 0x27, 0x61, 0x35, 0x4D, 0xD7, 0x4C, 0x69, 
        0xD2, 0xA8, 0xD3, 0x6D, 0xE9, 0x1B, 0x44, 0xFC
    };

    // 19871145308A610BF6C2607389A0A151832761354DD74C69D2A8D36DE91B44FC
    // 0410AB3577098A8115D00F7CA8EF7852F013BC0E45A75B7FFB81AB2FAC142F2E9FBB4085D4605F8E87712990DFA1A4EBAD292C3161173ED44CC733C389E6C005E0
    // 3045022025983db40ad924adc10b7800811b501e0b75be940539cad75c00e6074dad5fc7022100d8351f91b4fd8e4180cdd1eca20ad591f18d3c583fa3d5158cda7b254d9a5d6b

    slice signature = slice_alloc(72);
    slice message = string_to_slice("aaa");
    signature = ecdsa_secp256r1_sha256(signature, private_key, message);
}