#ifndef arena_h
#define arena_h

#include "arena.h"

arena arena_make(void* base, size_t size) {
    arena out;
    out.head = (char*) base;
    #ifdef arena_bounds_checking
        out.head_max = out.head + size;
    #endif
    return out;
}

void* arena_allocate(arena* in, size_t size) {
    char* temp = in->head;
    in->head += size;
    //assert(in->head <= in->head_max && in->head >= temp, ""); // Out of bounds/Overflow
    return temp;
}

#endif