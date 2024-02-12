@echo off
cls
echo Building...

if "%1"=="dev" (
    clang ^
    %2.c -o temp/%2.exe ^
    -g -Xlinker /INCREMENTAL:NO ^
    -march=native ^
    -O0 ^
    -D slice_bounds_checking ^
    -Weverything ^
    -Wno-newline-eof ^
    -Wno-unused-variable ^
    -Wno-missing-noreturn ^
    -Wno-unused-macros ^
    -Wno-missing-prototypes ^
    -Wno-disabled-macro-expansion ^
    -Wno-missing-variable-declarations ^
    -Wno-unused-parameter ^
    -Wno-switch ^
    -Wno-bad-function-cast ^
    -Wno-shadow ^
    -Wno-extra-semi ^
    -Wno-extra-semi-stmt ^
    -Wno-conditional-uninitialized ^
    -Wno-declaration-after-statement ^
    -Wno-strict-prototypes ^
    -Wno-unsafe-buffer-usage ^
    -Wno-bitwise-op-parentheses
) 

if "%1"=="release" (
    clang ^
    %2.c -o temp/%2.exe ^
    -g -Xlinker /INCREMENTAL:NO ^
    -march=native ^
    -O3 ^
    -Weverything ^
    -Wno-newline-eof ^
    -Wno-unused-variable ^
    -Wno-missing-noreturn ^
    -Wno-unused-macros ^
    -Wno-missing-prototypes ^
    -Wno-disabled-macro-expansion ^
    -Wno-missing-variable-declarations ^
    -Wno-unused-parameter ^
    -Wno-switch ^
    -Wno-bad-function-cast ^
    -Wno-shadow ^
    -Wno-extra-semi ^
    -Wno-extra-semi-stmt ^
    -Wno-conditional-uninitialized ^
    -Wno-bitwise-op-parentheses
)

if "%1"=="release_no_intrinsics" (
    clang ^
    %2.c -o temp/%2.exe ^
    -g -Xlinker /INCREMENTAL:NO ^
    -march=native ^
    -O3 ^
    -D no_intrinsics ^
    -Weverything ^
    -Wno-newline-eof ^
    -Wno-unused-variable ^
    -Wno-missing-noreturn ^
    -Wno-unused-macros ^
    -Wno-missing-prototypes ^
    -Wno-disabled-macro-expansion ^
    -Wno-missing-variable-declarations ^
    -Wno-unused-parameter ^
    -Wno-switch ^
    -Wno-bad-function-cast ^
    -Wno-shadow ^
    -Wno-extra-semi ^
    -Wno-extra-semi-stmt ^
    -Wno-conditional-uninitialized ^
    -Wno-bitwise-op-parentheses
)

if "%1"=="lint" (
    clang ^
    %2.c -fsyntax-only ^
    -Weverything ^
    -Wno-newline-eof ^
    -Wno-missing-noreturn ^
    -Wno-disabled-macro-expansion ^
    -Wno-missing-variable-declarations ^
    -Wno-switch ^
    -Wno-bad-function-cast ^
    -Wno-shadow ^
    -Wno-extra-semi ^
    -Wno-extra-semi-stmt ^
    -Wno-conditional-uninitialized
)
