#ifndef VEC_H
#define VEC_H

#include <assert.h>
#include <stdlib.h>

#ifndef VEC_DEFAULT_CAPACITY
#define VEC_DEFAULT_CAPACITY 4
#endif

/**
 * 宏 DEFINE_VEC(TYPE, NAME) —— 生成针对 TYPE 类型的动态数组
 *
 * 类型名     : Vec_##NAME
 * 操作表     : VecOps_##NAME
 * 操作表实例 : vec_##NAME##_ops
 *
 * 所有函数为 static，可安全放入头文件并在多个 .c 文件中包含。
 */
#define DEFINE_VEC(TYPE, NAME)                                                 \
	/* ---------- 辅助宏 ---------- */                                         \
	/* 已在外部可用，此处仅作占位 */                                           \
                                                                               \
	/* 前向声明 */                                                             \
	typedef struct Vec_##NAME Vec_##NAME;                                      \
	typedef struct VecOps_##NAME VecOps_##NAME;                                \
                                                                               \
	/* ---------- 操作表结构 ---------- */                                     \
	struct VecOps_##NAME {                                                     \
		void (*init)(Vec_##NAME * self);                                       \
		void (*init_fill)(Vec_##NAME * self, size_t n, TYPE val);              \
		void (*init_copy)(Vec_##NAME * self, const Vec_##NAME *other,          \
						  size_t start, size_t end);                           \
		void (*destroy)(Vec_##NAME * self);                                    \
		void (*reserve)(Vec_##NAME * self, size_t new_cap);                    \
		void (*resize)(Vec_##NAME * self, size_t new_size);                    \
		TYPE (*get)(Vec_##NAME * self, size_t index);                          \
		void (*set)(Vec_##NAME * self, size_t index, TYPE val);                \
		void (*push)(Vec_##NAME * self, TYPE val);                             \
		TYPE (*pop)(Vec_##NAME * self);                                        \
		size_t (*size)(Vec_##NAME * self);                                     \
		size_t (*capacity)(Vec_##NAME * self);                                 \
		int (*empty)(Vec_##NAME * self);                                       \
		TYPE (*front)(Vec_##NAME * self);                                      \
		TYPE (*back)(Vec_##NAME * self);                                       \
		TYPE *(*begin)(Vec_##NAME * self);                                     \
		TYPE *(*end)(Vec_##NAME * self);                                       \
		void (*clear)(Vec_##NAME * self);                                      \
		void (*swap)(Vec_##NAME * self, Vec_##NAME *other);                    \
		void (*shrink_to_fit)(Vec_##NAME * self);                              \
	};                                                                         \
                                                                               \
	/* ---------- Vec 结构体 ---------- */                                     \
	struct Vec_##NAME {                                                        \
		const VecOps_##NAME *ops;                                              \
		TYPE *data;                                                            \
		size_t size;                                                           \
		size_t capacity;                                                       \
	};                                                                         \
                                                                               \
	/* ---------- 函数实现 ---------- */                                       \
	static void vec_##NAME##_init(Vec_##NAME *self) {                          \
		self->ops = &vec_##NAME##_ops;                                         \
		self->data = NULL;                                                     \
		self->size = 0;                                                        \
		self->capacity = 0;                                                    \
	}                                                                          \
                                                                               \
	static void vec_##NAME##_init_fill(Vec_##NAME *self, size_t n, TYPE val) { \
		vec_##NAME##_init(self);                                               \
		vec_##NAME##_resize(self, n);                                          \
		for (size_t i = 0; i < n; ++i)                                         \
			self->data[i] = val;                                               \
	}                                                                          \
                                                                               \
	static void vec_##NAME##_init_copy(                                        \
		Vec_##NAME *self, const Vec_##NAME *other, size_t start, size_t end) { \
		vec_##NAME##_init(self);                                               \
		assert(start <= end && end <= other->size);                            \
		size_t len = end - start;                                              \
		vec_##NAME##_resize(self, len);                                        \
		for (size_t i = 0; i < len; ++i)                                       \
			self->data[i] = other->data[start + i];                            \
	}                                                                          \
                                                                               \
	static void vec_##NAME##_destroy(Vec_##NAME *self) {                       \
		free(self->data);                                                      \
		self->data = NULL;                                                     \
		self->size = 0;                                                        \
		self->capacity = 0;                                                    \
	}                                                                          \
                                                                               \
	static void vec_##NAME##_reserve(Vec_##NAME *self, size_t new_cap) {       \
		if (new_cap > self->capacity) {                                        \
			TYPE *tmp = (TYPE *)realloc(self->data, new_cap * sizeof(TYPE));   \
			assert(tmp != NULL);                                               \
			self->data = tmp;                                                  \
			self->capacity = new_cap;                                          \
		}                                                                      \
	}                                                                          \
                                                                               \
	static void vec_##NAME##_resize(Vec_##NAME *self, size_t new_size) {       \
		if (new_size > self->capacity) {                                       \
			size_t new_cap =                                                   \
				new_size > self->capacity * 2 ? new_size : self->capacity * 2; \
			if (new_cap == 0)                                                  \
				new_cap = VEC_DEFAULT_CAPACITY;                                \
			vec_##NAME##_reserve(self, new_cap);                               \
		}                                                                      \
		self->size = new_size;                                                 \
	}                                                                          \
                                                                               \
	static TYPE vec_##NAME##_get(Vec_##NAME *self, size_t index) {             \
		assert(index < self->size);                                            \
		return self->data[index];                                              \
	}                                                                          \
                                                                               \
	static void vec_##NAME##_set(Vec_##NAME *self, size_t index, TYPE val) {   \
		assert(index < self->size);                                            \
		self->data[index] = val;                                               \
	}                                                                          \
                                                                               \
	static void vec_##NAME##_push(Vec_##NAME *self, TYPE val) {                \
		if (self->size == self->capacity) {                                    \
			size_t new_cap = self->capacity == 0 ? VEC_DEFAULT_CAPACITY        \
												 : self->capacity * 2;         \
			vec_##NAME##_reserve(self, new_cap);                               \
		}                                                                      \
		self->data[self->size++] = val;                                        \
	}                                                                          \
                                                                               \
	static TYPE vec_##NAME##_pop(Vec_##NAME *self) {                           \
		assert(self->size > 0);                                                \
		return self->data[--self->size];                                       \
	}                                                                          \
                                                                               \
	static size_t vec_##NAME##_size(Vec_##NAME *self) { return self->size; }   \
	static size_t vec_##NAME##_capacity(Vec_##NAME *self) {                    \
		return self->capacity;                                                 \
	}                                                                          \
	static int vec_##NAME##_empty(Vec_##NAME *self) {                          \
		return self->size == 0;                                                \
	}                                                                          \
                                                                               \
	static TYPE vec_##NAME##_front(Vec_##NAME *self) {                         \
		assert(self->size > 0);                                                \
		return self->data[0];                                                  \
	}                                                                          \
                                                                               \
	static TYPE vec_##NAME##_back(Vec_##NAME *self) {                          \
		assert(self->size > 0);                                                \
		return self->data[self->size - 1];                                     \
	}                                                                          \
                                                                               \
	static TYPE *vec_##NAME##_begin(Vec_##NAME *self) { return self->data; }   \
	static TYPE *vec_##NAME##_end(Vec_##NAME *self) {                          \
		return self->data + self->size;                                        \
	}                                                                          \
                                                                               \
	static void vec_##NAME##_clear(Vec_##NAME *self) { self->size = 0; }       \
                                                                               \
	static void vec_##NAME##_swap(Vec_##NAME *self, Vec_##NAME *other) {       \
		Vec_##NAME tmp = *self;                                                \
		*self = *other;                                                        \
		*other = tmp;                                                          \
	}                                                                          \
                                                                               \
	static void vec_##NAME##_shrink_to_fit(Vec_##NAME *self) {                 \
		if (self->size < self->capacity) {                                     \
			TYPE *tmp =                                                        \
				(TYPE *)realloc(self->data, self->size * sizeof(TYPE));        \
			assert(tmp != NULL);                                               \
			self->data = tmp;                                                  \
			self->capacity = self->size;                                       \
		}                                                                      \
	}                                                                          \
                                                                               \
	/* ---------- 默认操作表实例 ---------- */                                 \
	static const VecOps_##NAME vec_##NAME##_ops = {                            \
		.init = vec_##NAME##_init,                                             \
		.init_fill = vec_##NAME##_init_fill,                                   \
		.init_copy = vec_##NAME##_init_copy,                                   \
		.destroy = vec_##NAME##_destroy,                                       \
		.reserve = vec_##NAME##_reserve,                                       \
		.resize = vec_##NAME##_resize,                                         \
		.get = vec_##NAME##_get,                                               \
		.set = vec_##NAME##_set,                                               \
		.push = vec_##NAME##_push,                                             \
		.pop = vec_##NAME##_pop,                                               \
		.size = vec_##NAME##_size,                                             \
		.capacity = vec_##NAME##_capacity,                                     \
		.empty = vec_##NAME##_empty,                                           \
		.front = vec_##NAME##_front,                                           \
		.back = vec_##NAME##_back,                                             \
		.begin = vec_##NAME##_begin,                                           \
		.end = vec_##NAME##_end,                                               \
		.clear = vec_##NAME##_clear,                                           \
		.swap = vec_##NAME##_swap,                                             \
		.shrink_to_fit = vec_##NAME##_shrink_to_fit}

#endif /* VEC_H */