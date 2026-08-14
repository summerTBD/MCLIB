#include "my_arena.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    my_arena* arena = arena_create(1024 * 1024);
    int* a = arena_alloc(arena, sizeof(int));
    char* b = arena_alloc(arena, sizeof(char));
    *a = 10;
    *b = 'a';
    printf("a is %d: \n", *a);
    printf("b is %c: \n", *b);

    arena_destroy(arena);
    return 0;
}