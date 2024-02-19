@echo off
cls
echo Building...

clang ^
chacha20_poly1305_test.c -o chacha20_poly1305_test.exe ^
-march=native ^
-O0 ^
-g -Xlinker /INCREMENTAL:NO ^
-D runtime_checks ^
--config ../clang_build_flags.cfg