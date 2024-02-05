#include "../basic.h"
#include "../string/string.c"

typedef struct slice slice;

#ifdef slice_bounds_checking
    #ifndef assert
        #error "You must define assert if using slices with bounds checking."
    #endif
#else
    #ifndef assert
        #define assert(...)
    #endif
#endif

struct slice {
    #ifdef slice_bounds_checking
        size_t capacity;
    #endif
    size_t length;
    u8* items;
};

#ifdef slice_bounds_checking
    #define slice_make(capacity, length, buffer) { capacity, length, buffer }
#else
    #define slice_make(capacity, length, buffer) { length, buffer }
#endif

#define slice_alloc(capacity) slice_make(capacity, 0, (u8[capacity]) { 0 })
#define slice_literal(...) slice_make(sizeof((u8[]) { __VA_ARGS__ }), sizeof((u8[]) { __VA_ARGS__ }), ((u8[]) { __VA_ARGS__ }))