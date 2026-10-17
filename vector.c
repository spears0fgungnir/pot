#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

int init_vector(vec_t* vec) {
    if (vec == NULL)
        return 1;

    vec->items = NULL;
    vec->count = 0;
    vec->capacity = 0;
    vec->max_index = 0;
    return 0;
}

int add_vector(vec_t* vec, void* item) {
    if (vec == NULL)
        return 1;

    if (sizeof(item) * (vec->max_index + 1) > vec->capacity) {
        void** tmp = realloc(vec->items, vec->capacity + sizeof(item));

        if (tmp == NULL)
            return 1;

        vec->items = tmp;
        vec->capacity += sizeof(item);
    }

    vec->items[vec->max_index] = item;
    vec->count++;
    vec->max_index++;
    return 0;
}

int add_vector_index(vec_t* vec, void* item, size_t idx) {
    if (vec == NULL)
        return 1;

    if (sizeof(item) * (idx + 1) > vec->capacity) {
        void** tmp = realloc(vec->items, sizeof(item) * (idx + 1));

        if (tmp == NULL)
            return 1;

        vec->items = tmp;
        vec->capacity = sizeof(item) * (idx + 1);


        for (int i = vec->max_index; i <= idx; i++)
            vec->items[i] = NULL;

    }
    vec->max_index = vec->max_index > idx + 1 ? vec->max_index : idx + 1;

    if (vec->items[idx] != NULL) {
        free(vec->items[idx]);
        vec->items[idx] = item;
        return 0;
    }

    vec->items[idx] = item;
    vec->count++;
    return 0;
}

int remove_vector_index(vec_t* vec, size_t idx) {
    if (vec == NULL)
        return 1;

    if (idx >= vec->max_index)
        return 1;

    if (vec->items[idx] == NULL)
        return 0;
    
    free(vec->items[idx]);
    vec->items[idx] = NULL;
    vec->count--;
    return 0;
}

int free_vector(vec_t* vec) {
    if (vec == NULL)
        return 1;

    for (int i = 0; i < vec->max_index; i++)
        if (vec->items[i] != NULL)
            free(vec->items[i]);

    free(vec->items);
    vec->items = NULL;
    vec->count = 0;
    vec->capacity = 0;
    vec->max_index = 0;
    return 0;
}
