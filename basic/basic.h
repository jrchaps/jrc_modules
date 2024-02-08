#include <stdint.h>
#include <stddef.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef float f32;
typedef double f64;

#define u8_max UINT8_MAX
#define u16_max UINT16_MAX
#define u32_max UINT32_MAX
#define u64_max UINT64_MAX
#define s8_max INT8_MAX
#define s16_max INT16_MAX
#define s32_max INT32_MAX
#define s64_max INT64_MAX

#define true 1
#define false 0 

#define array_length(array) (sizeof(array) / sizeof(array[0]))

#ifdef slice_bounds_checking
    #ifndef assert
        #error "You must define assert if using slices with bounds checking."
    #endif
#endif

typedef struct slice slice;

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
#define array_to_slice(array) (slice) slice_make(array_length(array), array_length(array), array)