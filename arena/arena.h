typedef struct arena arena;

//#define arena_bounds_checking

struct arena {
    char* head;
    #ifdef arena_bounds_checking
        char* head_max;
    #endif
};