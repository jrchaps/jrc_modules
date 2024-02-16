@echo off
cls
echo Building...

if "%1"=="dev" (
    clang ^
    aes128_gcm_test.c -o aes128_gcm_test.exe ^
    -march=native ^
    -O0 ^
    -g -Xlinker /INCREMENTAL:NO ^
    -D slice_bounds_checking ^
    -D %2 ^
    --config ../clang_build_flags.cfg
) 

if "%1"=="release" (
    clang ^
    aes128_gcm_test.c -o aes128_gcm_test.exe ^
    -march=native ^
    -O3 ^
    -g -Xlinker /INCREMENTAL:NO ^
    -D %2 ^
    --config ../clang_build_flags.cfg
)