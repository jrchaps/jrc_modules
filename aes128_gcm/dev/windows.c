#if 0
// #include "test_aes128_gcm.h"
#include "Windows.h"

LARGE_INTEGER performance_counter_frequency;

s64 profile_begin() {
    LARGE_INTEGER counter;
    BOOL success = QueryPerformanceCounter(&counter);
    //assert(success);
    return (u64) counter.QuadPart;
}

f64 profile_end(s64 counter) {
    LARGE_INTEGER counter_end;
    Bool success = QueryPerformanceCounter(&counter_end);
    // assert(success);
    s64 counter_elapsed = counter_end - counter;
    return (f64) (counter_elapsed / performance_counter_frequency.QuadPart);
}

int wWinMain() {

    LARGE_INTEGER counter_frequency;
    BOOL success = QueryPerformanceFrequency(&counter_frequency);

    // test_aes128_gcm();

}
#endif