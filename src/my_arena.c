#include "my_arena.h"
#include <stdlib.h> // malloc, free
#include <string.h> // 可选，如果后续用到 memset 等

// 向上对齐到 2 的幂次（宏定义）
#define ALIGN_UP_POW2(x, align) (((x) + (align) - 1) & ~((align) - 1))

// 创建 Arena
my_arena* arena_create(size_t buffer_size) {
    size_t total_size = sizeof(my_arena) + buffer_size;
    my_arena* arena = (my_arena*)malloc(total_size);
    if (arena == NULL) {
        return NULL;
    }

    arena->pos = 0;
    arena->capacity = buffer_size;
    return arena;
}

// 销毁 Arena
void arena_destroy(my_arena* arena) {
    free(arena);
}

// 核心分配函数
void* arena_alloc_aligned(my_arena* arena, size_t size, size_t align) {
    if (arena == NULL || size == 0 || align == 0)
        return NULL;

    // 为了安全，如果传入的对齐数不是 2 的幂，我们自动修正为 1（不报错）
    // 或者保证至少是 1
    if ((align & (align - 1)) != 0)
        align = 1;

    size_t start = ALIGN_UP_POW2(arena->pos, align);
    size_t new_pos = start + size;

    if (new_pos > arena->capacity)
        return NULL;

    arena->pos = new_pos;
    return (void*)&arena->data[start];
}

void* arena_alloc(my_arena* arena, size_t size) {
    return arena_alloc_aligned(arena, size, ARENA_ALIGN);
}

// 回退最近一次分配（size 不能超过当前 pos）
void arena_pop(my_arena* arena, size_t size) {
    if (arena == NULL) {
        return;
    }
    size = (size < arena->pos) ? size : arena->pos;
    arena->pos -= size;
}

// 回退到指定的游标位置
void arena_pop_to(my_arena* arena, size_t pos) {
    if (arena == NULL) {
        return;
    }
    size_t size = (arena->pos > pos) ? (arena->pos - pos) : 0;
    arena_pop(arena, size);
}

// 清空 Arena
void arena_clear(my_arena* arena) {
    arena_pop_to(arena, 0);
}