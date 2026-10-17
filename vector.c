#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "vector.h"

int init_vector(vec_t* vec) {
    if (vec == NULL) {
        fprintf(stderr, "fatal: Failed to initalize vector.");
        return 1;
    }

    vec->items = NULL;
    vec->count = 0;
    vec->capacity = 0;
    vec->max_index = 0;
    return 0;
}

int add_vector_index(vec_t* vec, void* item, size_t idx) {
    if (vec == NULL || item == NULL) {
        fprintf(stderr, "fatal: Failed to add item at specified index.\n");
        return 1;
    }

    if (idx >= SIZE_MAX / sizeof(*vec->items)) {
        fprintf(stderr, "fatal: Failed to add item at specified index.\n");
        return 1;
    }

    if (sizeof(*vec->items) * (idx + 1) > vec->capacity) {
        void** tmp = realloc(vec->items, sizeof(*vec->items) * (idx + 1));
         
        if (tmp == NULL) {
            fprintf(stderr, "fatal: Failed to add item at specified index\n");
            return 1;
        }
        
        //printf("%d\n", idx);
        vec->items = tmp;
        vec->capacity = sizeof(*vec->items) * (idx + 1);

        for (size_t i = vec->max_index; i <= idx; i++)
            vec->items[i] = NULL;

    }
    vec->max_index = vec->max_index > idx + 1 ? vec->max_index : idx + 1;

    if (vec->items[idx] == item)
        return 0;

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
    if (vec == NULL) {
        fprintf(stderr, "fatal: Failed to remove item at specified index.\n");
        return 1;
    }

    if (idx >= vec->max_index) {
        fprintf(stderr, "fatal: Failed to remove item at specified index\n");
        return 1;
    }

    if (vec->items[idx] == NULL)
        return 0;
    
    free(vec->items[idx]);
    vec->items[idx] = NULL;
    vec->count--;
    return 0;
}

int free_vector(vec_t* vec) {
    if (vec == NULL) {
        fprintf(stderr, "fatal: Failed to free allocated vector\n");
        return 1;
    }

    for (size_t i = 0; i < vec->max_index; i++)
        if (vec->items[i] != NULL)
            free(vec->items[i]);

    free(vec->items);
    vec->items = NULL;
    vec->count = 0;
    vec->capacity = 0;
    vec->max_index = 0;
    return 0;
}
