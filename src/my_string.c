#include "my_string.h"
#include <stdlib.h>
#include <string.h>

struct my_string {
    size_t length;   // 当前长度（不含'\0'）
    size_t capacity; // 已分配容量（不含'\0'）
    char str[];      // 柔性数组，实际分配 capacity+1 字节
};

// ----- 创建与销毁 -----
my_string* my_string_new(size_t capacity) {
    size_t total = sizeof(my_string) + capacity + 1;
    my_string* ms = (my_string*)malloc(total);
    if (!ms)
        return NULL;
    ms->length = 0;
    ms->capacity = capacity;
    ms->str[0] = '\0';
    return ms;
}

my_string* my_string_from_cstr(const char* s) {
    if (!s)
        return NULL;
    size_t l = strlen(s);
    my_string* ms = my_string_new(l);
    if (!ms)
        return NULL;
    memcpy(ms->str, s, l + 1);
    ms->length = l;
    return ms;
}

my_string* my_string_dup(const my_string* ms) {
    if (!ms)
        return NULL;
    my_string* new_ms = my_string_new(ms->capacity);
    if (!new_ms)
        return NULL;
    memcpy(new_ms->str, ms->str, ms->length + 1);
    new_ms->length = ms->length;
    return new_ms;
}

void my_string_drop(my_string* ms) {
    free(ms);
}

// ----- 信息查询 -----
size_t my_string_len(const my_string* ms) {
    return ms ? ms->length : 0;
}

size_t my_string_capacity(const my_string* ms) {
    return ms ? ms->capacity : 0;
}

const char* my_string_cstr(const my_string* ms) {
    return ms ? ms->str : NULL;
}

int my_string_is_empty(const my_string* ms) {
    return ms ? (ms->length == 0) : 1;
}

// ----- 容量管理 -----
my_string* my_string_reserve(my_string* ms, size_t new_capacity) {
    if (!ms)
        return NULL;
    if (new_capacity <= ms->capacity)
        return ms;
    size_t total = sizeof(my_string) + new_capacity + 1;
    my_string* new_ms = (my_string*)realloc(ms, total);
    if (!new_ms)
        return NULL;
    new_ms->capacity = new_capacity;
    return new_ms;
}

my_string* my_string_shrink_to_fit(my_string* ms) {
    if (!ms)
        return NULL;
    if (ms->length == ms->capacity)
        return ms;
    size_t new_cap = ms->length;
    size_t total = sizeof(my_string) + new_cap + 1;
    my_string* new_ms = (my_string*)realloc(ms, total);
    if (!new_ms)
        return NULL;
    new_ms->capacity = new_cap;
    return new_ms;
}

// ----- 修改操作 -----
my_string* my_string_append_cstr(my_string* ms, const char* s) {
    if (!ms || !s)
        return ms;
    size_t add_len = strlen(s);
    if (add_len == 0)
        return ms;
    size_t new_len = ms->length + add_len;
    if (new_len >= ms->capacity) {
        size_t new_cap = ms->capacity * 2;
        if (new_cap < new_len)
            new_cap = new_len;
        my_string* new_ms = my_string_reserve(ms, new_cap);
        if (!new_ms)
            return NULL;
        ms = new_ms;
    }
    memcpy(ms->str + ms->length, s, add_len + 1); // 拷贝 '\0'
    ms->length = new_len;
    return ms;
}

my_string* my_string_append_char(my_string* ms, char c) {
    if (!ms)
        return NULL;
    if (ms->length + 1 >= ms->capacity) {
        size_t new_cap = ms->capacity ? ms->capacity * 2 : 1;
        my_string* new_ms = my_string_reserve(ms, new_cap);
        if (!new_ms)
            return NULL;
        ms = new_ms;
    }
    ms->str[ms->length] = c;
    ms->length++;
    ms->str[ms->length] = '\0';
    return ms;
}

void my_string_clear(my_string* ms) {
    if (!ms)
        return;
    ms->length = 0;
    ms->str[0] = '\0';
}

my_string* my_string_truncate(my_string* ms, size_t new_len) {
    if (!ms)
        return NULL;
    if (new_len == ms->length)
        return ms;
    if (new_len < ms->length) {
        ms->length = new_len;
        ms->str[new_len] = '\0';
        return ms;
    }
    // 需要扩展
    if (new_len >= ms->capacity) {
        size_t new_cap = ms->capacity * 2;
        if (new_cap < new_len)
            new_cap = new_len;
        my_string* new_ms = my_string_reserve(ms, new_cap);
        if (!new_ms)
            return NULL;
        ms = new_ms;
    }
    // 填充新增部分为 '\0'（保证安全）
    memset(ms->str + ms->length, 0, new_len - ms->length + 1);
    ms->length = new_len;
    return ms;
}