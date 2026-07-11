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

/* ===================================================================
 * X-Macro: 操作列表
 * 格式: X(name, ret_type, full_params)
 * full_params 是包含 String *self 在内的完整参数列表（带括号）
 * =================================================================== */
#define STRING_OPS                          \
	X(init,     void,        (String *self))                \
	X(destroy,  void,        (String *self))                \
	X(push,     void,        (String *self, const char *str)) \
	X(pop_back, void,        (String *self))                \
	X(clear,    void,        (String *self))                \
	X(size,     size_t,      (String *self))                \
	X(capacity, size_t,      (String *self))                \
	X(c_str,    const char *, (String *self))                \
	X(assign,   void,        (String *self, const char *str)) \
	X(front,    char *,      (String *self))                \
	X(back,     char *,      (String *self))

/* ---------- 操作表（由 X-Macro 生成） ---------- */
struct StringOps {
#define X(name, ret, params) ret (*name)params;
	STRING_OPS
#undef X
};

/* ---------- 字符串主结构 ---------- */
struct String {
	char *data;
	size_t size; /* 不含终止符的长度 */
	size_t capacity;
	const StringOps *ops;
};

/* ====== static 函数前向声明（由 X-Macro 生成） ====== */
#define X(name, ret, params) static ret string_##name params;
STRING_OPS
#undef X

/* ---------- 默认操作表（由 X-Macro 生成） ---------- */
static const StringOps string_default_ops = {
#define X(name, ret, params) .name = string_##name,
	STRING_OPS
#undef X
};

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