#ifndef MY_STRING_H
#define MY_STRING_H

#include <stddef.h> // size_t

typedef struct my_string my_string; // 不透明类型

// ----- 创建与销毁 -----
my_string* my_string_new(size_t capacity);
my_string* my_string_from_cstr(const char* s);
my_string* my_string_dup(const my_string* ms);
void my_string_drop(my_string* ms);
#define MY_STRING_DROP(p)  \
    do {                   \
        my_string_drop(p); \
        (p) = NULL;        \
    } while (0)

// ----- 信息查询 -----
size_t my_string_len(const my_string* ms);
size_t my_string_capacity(const my_string* ms);
const char* my_string_cstr(const my_string* ms);
int my_string_is_empty(const my_string* ms);

// ----- 容量管理 -----
my_string* my_string_reserve(my_string* ms, size_t new_capacity);
my_string* my_string_shrink_to_fit(my_string* ms);

// ----- 修改操作 -----
my_string* my_string_append_cstr(my_string* ms, const char* s);
my_string* my_string_append_char(my_string* ms, char c);
void my_string_clear(my_string* ms);
my_string* my_string_truncate(my_string* ms, size_t new_len);

#endif