@echo off
cls
echo Building...

clang ^
ecdsa_secp256r1_sha256_test.c -o ecdsa_secp256r1_sha256_test.exe ^
-I tinycrypt/lib/include ^
-march=native ^
-O0 ^
-g -Xlinker /INCREMENTAL:NO ^
-D runtime_checks ^
--config ../clang_build_flags.cfg