#include <stdint.h>

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

u32 ror(u32, u32);
u32 be_to_u32(u8*);