#ifndef MYSTRING_H
#define MYSTRING_H

#include <stddef.h> // size_t

typedef struct mystring mystring; // 不透明类型

// ----- 创建与销毁 -----
mystring* mystring_new(size_t capacity);
mystring* mystring_from_cstr(const char* s);
mystring* mystring_dup(const mystring* ms);
void mystring_drop(mystring* ms);
#define MYSTRING_DROP(p)  \
    do {                  \
        mystring_drop(p); \
        (p) = NULL;       \
    } while (0)

// ----- 信息查询 -----
size_t mystring_len(const mystring* ms);
size_t mystring_capacity(const mystring* ms);
const char* mystring_cstr(const mystring* ms);
int mystring_is_empty(const mystring* ms);

// ----- 容量管理 -----
mystring* mystring_reserve(mystring* ms, size_t new_capacity);
mystring* mystring_shrink_to_fit(mystring* ms);

// ----- 修改操作 -----
mystring* mystring_append_cstr(mystring* ms, const char* s);
mystring* mystring_append_char(mystring* ms, char c);
void mystring_clear(mystring* ms);
mystring* mystring_truncate(mystring* ms, size_t new_len);

#endif