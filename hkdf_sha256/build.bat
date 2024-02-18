@echo off
cls
echo Building...

clang ^
hkdf_sha256_test.c -o hkdf_sha256_test.exe ^
-march=native ^
-O0 ^
-g -Xlinker /INCREMENTAL:NO ^
-D slice_bounds_checking ^
--config ../clang_build_flags.cfg