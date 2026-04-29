#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define ElemType int

#define VEC_MAX(a, b) ((a) > (b) ? (a) : (b))
#define VEC_MIN(a, b) ((a) < (b) ? (a) : (b))

/* 前向声明 */
typedef struct Vec Vec;
struct VecOps;

/* Vec 对象，通过 ops 指针绑定方法 */
struct Vec {
	const struct VecOps *ops;
	ElemType *data;
	size_t size;
	size_t capacity;
};

/* 操作表：每一成员都是一个函数指针 */
struct VecOps {
	void (*init)(Vec *self);
	void (*init_fill)(Vec *self, size_t size, ElemType value);
	void (*init_copy)(Vec *self, const Vec *other, size_t start, size_t end);
	void (*destroy)(Vec *self);
	void (*reserve)(Vec *self, size_t new_capacity);
	void (*resize)(Vec *self, size_t new_size);
	ElemType (*get)(Vec *self, size_t index);
	void (*set)(Vec *self, size_t index, ElemType value);
	void (*push)(Vec *self, ElemType value);
	ElemType (*pop)(Vec *self);
	size_t (*size)(Vec *self);
	size_t (*capacity)(Vec *self);
	bool (*is_empty)(Vec *self);
	ElemType (*front)(Vec *self);
	ElemType (*back)(Vec *self);
	ElemType *(*begin)(Vec *self);
	ElemType *(*end)(Vec *self);
	void (*clear)(Vec *self);
	void (*swap)(Vec *self, Vec *other);
	void (*shrink_to_fit)(Vec *self);
};

/* ---------- 所有功能函数声明（静态，仅本文件可见） ---------- */
static void vec_init(Vec *self);
static void vec_init_fill(Vec *self, size_t size, ElemType value);
static void vec_init_copy(Vec *self, const Vec *other, size_t start,
						  size_t end);
static void vec_destroy(Vec *self);
static void vec_reserve(Vec *self, size_t new_capacity);
static void vec_resize(Vec *self, size_t new_size);
static ElemType vec_get(Vec *self, size_t index);
static void vec_set(Vec *self, size_t index, ElemType value);
static void vec_push(Vec *self, ElemType value);
static ElemType vec_pop(Vec *self);
static size_t vec_size(Vec *self);
static size_t vec_capacity(Vec *self);
static bool vec_is_empty(Vec *self);
static ElemType vec_front(Vec *self);
static ElemType vec_back(Vec *self);
static ElemType *vec_begin(Vec *self);
static ElemType *vec_end(Vec *self);
static void vec_clear(Vec *self);
static void vec_swap(Vec *self, Vec *other);
static void vec_shrink_to_fit(Vec *self);

/* ---------- 全局默认操作表（所有 Vec 共用） ---------- */
static const struct VecOps vec_default_ops = {.init = vec_init,
											  .init_fill = vec_init_fill,
											  .init_copy = vec_init_copy,
											  .destroy = vec_destroy,
											  .reserve = vec_reserve,
											  .resize = vec_resize,
											  .get = vec_get,
											  .set = vec_set,
											  .push = vec_push,
											  .pop = vec_pop,
											  .size = vec_size,
											  .capacity = vec_capacity,
											  .is_empty = vec_is_empty,
											  .front = vec_front,
											  .back = vec_back,
											  .begin = vec_begin,
											  .end = vec_end,
											  .clear = vec_clear,
											  .swap = vec_swap,
											  .shrink_to_fit =
												  vec_shrink_to_fit};

/* ---------- 函数实现 ---------- */

static void vec_init(Vec *self) {
	self->ops = &vec_default_ops; /* 绑定操作表 */
	self->data = NULL;
	self->size = 0;
	self->capacity = 0;
}

static void vec_init_fill(Vec *self, size_t size, ElemType value) {
	vec_init(self);
	vec_resize(self, size);
	for (size_t i = 0; i < size; ++i) {
		self->data[i] = value;
	}
}

static void vec_init_copy(Vec *self, const Vec *other, size_t start,
						  size_t end) {
	vec_init(self);
	if (start > end || end > other->size) {
		fprintf(stderr, "Invalid range for copy\n");
		vec_destroy(self);
		exit(EXIT_FAILURE);
	}
	size_t new_size = end - start;
	vec_resize(self, new_size);
	for (size_t i = 0; i < new_size; ++i) {
		self->data[i] = other->data[start + i];
	}
}

static void vec_destroy(Vec *self) {
	free(self->data);
	self->data = NULL;
	self->size = 0;
	self->capacity = 0;
}

static void vec_reserve(Vec *self, size_t new_capacity) {
	if (new_capacity > self->capacity) {
		ElemType *new_data =
			(ElemType *)realloc(self->data, new_capacity * sizeof(ElemType));
		if (new_data == NULL) {
			fprintf(stderr, "Failed to allocate memory\n");
			vec_destroy(self);
			exit(EXIT_FAILURE);
		}
		self->data = new_data;
		self->capacity = new_capacity;
	}
}

static void vec_resize(Vec *self, size_t new_size) {
	if (new_size > self->capacity) {
		size_t new_capacity = VEC_MAX(new_size, self->capacity * 2);
		vec_reserve(self, new_capacity);
	}
	self->size = new_size;
}

static ElemType vec_get(Vec *self, size_t index) {
	if (index >= self->size) {
		fprintf(stderr, "Index out of bounds\n");
		vec_destroy(self);
		exit(EXIT_FAILURE);
	}
	return self->data[index];
}

static void vec_set(Vec *self, size_t index, ElemType value) {
	if (index >= self->size) {
		fprintf(stderr, "Index out of bounds\n");
		vec_destroy(self);
		exit(EXIT_FAILURE);
	}
	self->data[index] = value;
}

static void vec_push(Vec *self, ElemType value) {
	if (self->size == self->capacity) {
		size_t new_capacity = VEC_MAX(2, self->capacity * 2);
		vec_reserve(self, new_capacity);
	}
	self->data[self->size] = value;
	self->size += 1;
}

static ElemType vec_pop(Vec *self) {
	if (self->size == 0) {
		fprintf(stderr, "Vector is empty\n");
		vec_destroy(self);
		exit(EXIT_FAILURE);
	}
	ElemType back_value = self->data[self->size - 1];
	self->size -= 1;
	return back_value;
}

static size_t vec_size(Vec *self) { return self->size; }
static size_t vec_capacity(Vec *self) { return self->capacity; }
static bool vec_is_empty(Vec *self) { return self->size == 0; }

static ElemType vec_front(Vec *self) {
	if (vec_is_empty(self)) {
		fprintf(stderr, "Vector is empty\n");
		vec_destroy(self);
		exit(EXIT_FAILURE);
	}
	return self->data[0];
}

static ElemType vec_back(Vec *self) {
	if (vec_is_empty(self)) {
		fprintf(stderr, "Vector is empty\n");
		vec_destroy(self);
		exit(EXIT_FAILURE);
	}
	return self->data[self->size - 1];
}

static ElemType *vec_begin(Vec *self) { return self->data; }
static ElemType *vec_end(Vec *self) { return self->data + self->size; }
static void vec_clear(Vec *self) { self->size = 0; }

static void vec_swap(Vec *self, Vec *other) {
	Vec temp = *self;
	*self = *other;
	*other = temp;
}

static void vec_shrink_to_fit(Vec *self) {
	if (self->size < self->capacity) {
		ElemType *new_data =
			(ElemType *)realloc(self->data, self->size * sizeof(ElemType));
		if (new_data == NULL) {
			fprintf(stderr, "Failed to allocate memory\n");
			vec_destroy(self);
			exit(EXIT_FAILURE);
		}
		self->data = new_data;
		self->capacity = self->size;
	}
}
