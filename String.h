#ifndef STRING_H
#define STRING_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* 前向声明 */
typedef struct String String;
typedef struct StringOps StringOps;

/* 操作表：函数指针集合 */
struct StringOps {
	void (*init)(String *self);
	void (*destroy)(String *self);
	void (*push)(String *self, const char *str);
	void (*clear)(String *self);
	size_t (*size)(String *self);
	size_t (*capacity)(String *self);
	const char *(*c_str)(String *self);
	void (*assign)(String *self, const char *str);
	char *(*front)(String *self);
	char *(*back)(String *self);
};

/* 字符串核心结构 */
struct String {
	char *data;
	size_t size; // 不包括 '\0'
	size_t capacity;
	const StringOps *ops;
};

/* 全局默认操作表声明 */
extern const StringOps string_default_ops;

#endif /* STRING_H */