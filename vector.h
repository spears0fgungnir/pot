#pragma once
#include <stddef.h>

typedef struct Vector {
    void** items;
    size_t count;
    size_t max_index;
    size_t capacity;
} vec_t;

int init_vector(vec_t* vec);

int add_vector_index(vec_t* vec, void* item, size_t idx);

int remove_vector_index(vec_t* vec, size_t idx);

int free_vector(vec_t* vec);
