#ifndef MY_ARENA_H
#define MY_ARENA_H

#include <stddef.h> // size_t

// 默认对齐大小（16字节，在64位系统下为 2 * sizeof(size_t)）
#ifndef ARENA_ALIGN
#define ARENA_ALIGN (2 * sizeof(size_t))
#endif

// Arena 结构体（柔性数组）
typedef struct {
    size_t pos;      // 当前已使用的偏移量
    size_t capacity; // 总容量
    char data[];     // 柔性数组，数据紧跟在结构体后面
} my_arena;

// 创建 Arena（buffer_size 为数据区大小）
my_arena* arena_create(size_t buffer_size);

// 销毁 Arena（一次性释放）
void arena_destroy(my_arena* arena);

// 分配内存+指定对齐大小（返回对齐后的指针，失败返回 NULL）
void* arena_alloc_align(my_arena* arena, size_t size, size_t align);

// 分配内存（返回对齐后的指针，失败返回 NULL）
void* arena_alloc(my_arena* arena, size_t size);

// 回退最近一次分配（size 为要回退的字节数）
void arena_pop(my_arena* arena, size_t size);

// 回退到指定的游标位置
void arena_pop_to(my_arena* arena, size_t pos);

// 清空 Arena（重置游标到 0）
void arena_clear(my_arena* arena);

#endif // MY_ARENA_H