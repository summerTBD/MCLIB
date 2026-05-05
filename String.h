#ifndef STRING_H
#define STRING_H

#include <assert.h>
#include <stdlib.h>
#include <string.h> /* memcpy, strlen */

/* 初始容量 */
#ifndef STRING_DEFAULT_CAPACITY
#define STRING_DEFAULT_CAPACITY 16
#endif

/* ---------- 前向声明 ---------- */
typedef struct String String;
typedef struct StringOps StringOps;

/* ---------- 操作表 ---------- */
struct StringOps {
	void (*init)(String *self);
	void (*destroy)(String *self);
	void (*push)(String *self, const char *str);
	void (*pop_back)(String *self);
	void (*clear)(String *self);
	size_t (*size)(String *self);
	size_t (*capacity)(String *self);
	const char *(*c_str)(String *self);
	void (*assign)(String *self, const char *str);
	char *(*front)(String *self);
	char *(*back)(String *self);
};

/* ---------- 字符串主结构 ---------- */
struct String {
	char *data;
	size_t size; /* 不含终止符的长度 */
	size_t capacity;
	const StringOps *ops;
};

/* ====== 所有 static 函数前向声明 ====== */
static void string_init(String *self);
static void string_destroy(String *self);
static void string_push(String *self, const char *str);
static void string_pop_back(String *self);
static void string_clear(String *self);
static size_t string_size(String *self);
static size_t string_capacity(String *self);
static const char *string_c_str(String *self);
static void string_assign(String *self, const char *str);
static char *string_front(String *self);
static char *string_back(String *self);

/* ---------- 默认操作表（提前到这里，因为声明已可见） ---------- */
static const StringOps string_default_ops = {.init = string_init,
											 .destroy = string_destroy,
											 .push = string_push,
											 .pop_back = string_pop_back,
											 .clear = string_clear,
											 .size = string_size,
											 .capacity = string_capacity,
											 .c_str = string_c_str,
											 .assign = string_assign,
											 .front = string_front,
											 .back = string_back};

/* ---------- 函数实现 ---------- */
static void string_init(String *self) {
	self->data = NULL;
	self->size = 0;
	self->capacity = 0;
	self->ops = &string_default_ops;
}

static void string_destroy(String *self) {
	free(self->data);
	self->data = NULL;
	self->size = 0;
	self->capacity = 0;
}

static void string_push(String *self, const char *str) {
	assert(str != NULL);

	size_t add_len = strlen(str);

	/* 检查溢出 */
	if (add_len > (size_t)-1 - self->size) {
		string_destroy(self);
		assert(0 && "String size overflow");
	}

	size_t needed = self->size + add_len + 1; /* +1 for '\0' */

	if (needed > self->capacity) {
		size_t new_cap =
			self->capacity == 0 ? STRING_DEFAULT_CAPACITY : self->capacity * 2;
		while (new_cap < needed) {
			new_cap *= 2;
		}
		char *tmp = (char *)realloc(self->data, new_cap);
		assert(tmp != NULL);
		self->data = tmp;
		self->capacity = new_cap;
	}

	memcpy(self->data + self->size, str, add_len);
	self->size += add_len;
	self->data[self->size] = '\0';
}

static void string_pop_back(String *self) {
	assert(self->size > 0);
	self->size--;
	self->data[self->size] = '\0';
}

static void string_clear(String *self) {
	self->size = 0;
	if (self->data) {
		self->data[0] = '\0';
	}
}

static size_t string_size(String *self) { return self->size; }

static size_t string_capacity(String *self) { return self->capacity; }

static const char *string_c_str(String *self) { return self->data; }

static void string_assign(String *self, const char *str) {
	string_clear(self);
	string_push(self, str);
}

static char *string_front(String *self) {
	assert(self->data != NULL);
	return self->data;
}

static char *string_back(String *self) {
	assert(self->size > 0);
	return self->data + self->size - 1;
}

#endif /* STRING_H */