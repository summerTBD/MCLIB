#include "mystring.h"
#include <stdlib.h>
#include <string.h>

struct mystring {
    size_t length;   // 当前长度（不含'\0'）
    size_t capacity; // 已分配容量（不含'\0'）
    char str[];      // 柔性数组，实际分配 capacity+1 字节
};

// ----- 创建与销毁 -----
mystring* mystring_new(size_t capacity) {
    size_t total = sizeof(mystring) + capacity + 1;
    mystring* ms = (mystring*)malloc(total);
    if (!ms)
        return NULL;
    ms->length = 0;
    ms->capacity = capacity;
    ms->str[0] = '\0';
    return ms;
}

mystring* mystring_from_cstr(const char* s) {
    if (!s)
        return NULL;
    size_t l = strlen(s);
    mystring* ms = mystring_new(l);
    if (!ms)
        return NULL;
    memcpy(ms->str, s, l + 1);
    ms->length = l;
    return ms;
}

mystring* mystring_dup(const mystring* ms) {
    if (!ms)
        return NULL;
    mystring* new_ms = mystring_new(ms->capacity);
    if (!new_ms)
        return NULL;
    memcpy(new_ms->str, ms->str, ms->length + 1);
    new_ms->length = ms->length;
    return new_ms;
}

void mystring_drop(mystring* ms) {
    free(ms);
}

// ----- 信息查询 -----
size_t mystring_len(const mystring* ms) {
    return ms ? ms->length : 0;
}

size_t mystring_capacity(const mystring* ms) {
    return ms ? ms->capacity : 0;
}

const char* mystring_cstr(const mystring* ms) {
    return ms ? ms->str : NULL;
}

int mystring_is_empty(const mystring* ms) {
    return ms ? (ms->length == 0) : 1;
}

// ----- 容量管理 -----
mystring* mystring_reserve(mystring* ms, size_t new_capacity) {
    if (!ms)
        return NULL;
    if (new_capacity <= ms->capacity)
        return ms;
    size_t total = sizeof(mystring) + new_capacity + 1;
    mystring* new_ms = (mystring*)realloc(ms, total);
    if (!new_ms)
        return NULL;
    new_ms->capacity = new_capacity;
    return new_ms;
}

mystring* mystring_shrink_to_fit(mystring* ms) {
    if (!ms)
        return NULL;
    if (ms->length == ms->capacity)
        return ms;
    size_t new_cap = ms->length;
    size_t total = sizeof(mystring) + new_cap + 1;
    mystring* new_ms = (mystring*)realloc(ms, total);
    if (!new_ms)
        return NULL;
    new_ms->capacity = new_cap;
    return new_ms;
}

// ----- 修改操作 -----
mystring* mystring_append_cstr(mystring* ms, const char* s) {
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
        mystring* new_ms = mystring_reserve(ms, new_cap);
        if (!new_ms)
            return NULL;
        ms = new_ms;
    }
    memcpy(ms->str + ms->length, s, add_len + 1); // 拷贝 '\0'
    ms->length = new_len;
    return ms;
}

mystring* mystring_append_char(mystring* ms, char c) {
    if (!ms)
        return NULL;
    if (ms->length + 1 >= ms->capacity) {
        size_t new_cap = ms->capacity ? ms->capacity * 2 : 1;
        mystring* new_ms = mystring_reserve(ms, new_cap);
        if (!new_ms)
            return NULL;
        ms = new_ms;
    }
    ms->str[ms->length] = c;
    ms->length++;
    ms->str[ms->length] = '\0';
    return ms;
}

void mystring_clear(mystring* ms) {
    if (!ms)
        return;
    ms->length = 0;
    ms->str[0] = '\0';
}

mystring* mystring_truncate(mystring* ms, size_t new_len) {
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
        mystring* new_ms = mystring_reserve(ms, new_cap);
        if (!new_ms)
            return NULL;
        ms = new_ms;
    }
    // 填充新增部分为 '\0'（保证安全）
    memset(ms->str + ms->length, 0, new_len - ms->length + 1);
    ms->length = new_len;
    return ms;
}