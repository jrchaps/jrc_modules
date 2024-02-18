@echo off
cls
echo Building...

clang ^
sha256_test.c -o sha256_test.exe ^
-march=native ^
-O0 ^
-g -Xlinker /INCREMENTAL:NO ^
-D slice_bounds_checking ^
--config ../clang_build_flags.cfg