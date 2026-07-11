#ifndef VEC_H
#define VEC_H

#include <assert.h>
#include <stdlib.h>

#ifndef VEC_DEFAULT_CAPACITY
#define VEC_DEFAULT_CAPACITY 4
#endif


/* ===================================================================
 * X-Macro: Vec 操作总表 —— 单一数据源
 * 格式: M(容器名, 操作名, 返回类型, (参数列表))
 * =================================================================== */
#define VEC_OPS(M, TYPE)                                                  \
    M(TYPE, init,          void,   (Vec_##TYPE* self))                          \
    M(TYPE, init_fill,     void,   (Vec_##TYPE* self, size_t n, TYPE val))      \
    M(TYPE, init_copy,     void,   (Vec_##TYPE* self, const Vec_##TYPE* other,  \
                                    size_t start, size_t end))                  \
    M(TYPE, destroy,       void,   (Vec_##TYPE* self))                          \
    M(TYPE, reserve,       void,   (Vec_##TYPE* self, size_t new_cap))          \
    M(TYPE, resize,        void,   (Vec_##TYPE* self, size_t new_size))         \
    M(TYPE, get,           TYPE,   (Vec_##TYPE* self, size_t index))            \
    M(TYPE, set,           void,   (Vec_##TYPE* self, size_t index, TYPE val))  \
    M(TYPE, push,          void,   (Vec_##TYPE* self, TYPE val))                \
    M(TYPE, pop,           TYPE,   (Vec_##TYPE* self))                          \
    M(TYPE, size,          size_t, (Vec_##TYPE* self))                          \
    M(TYPE, capacity,      size_t, (Vec_##TYPE* self))                          \
    M(TYPE, empty,         int,    (Vec_##TYPE* self))                          \
    M(TYPE, front,         TYPE,   (Vec_##TYPE* self))                          \
    M(TYPE, back,          TYPE,   (Vec_##TYPE* self))                          \
    M(TYPE, begin,         TYPE*,  (Vec_##TYPE* self))                          \
    M(TYPE, end,           TYPE*,  (Vec_##TYPE* self))                          \
    M(TYPE, clear,         void,   (Vec_##TYPE* self))                          \
    M(TYPE, swap,          void,   (Vec_##TYPE* self, Vec_##TYPE* other))       \
    M(TYPE, shrink_to_fit, void,   (Vec_##TYPE* self))

/* X-Macro helper —— 文件作用域，供 DEFINE_VEC 复用 */
#define VEC_OPS_FIELD(TYPE, op, ret, params) ret (*op) params;
#define VEC_FWD(TYPE, op, ret, params)        static ret vec_##TYPE##_##op params;
#define VEC_TABLE(TYPE, op, ret, params)      .op = vec_##TYPE##_##op,

#define DEFINE_VEC(TYPE)                                                 \
	/* ---------- 前向声明 ---------- */                                       \
	typedef struct Vec_##TYPE Vec_##TYPE;                                      \
	typedef struct VecOps_##TYPE VecOps_##TYPE;                                \
                                                                               \
	/* ---------- 操作表结构 ---------- */                                     \
	struct VecOps_##TYPE {                                                     \
		VEC_OPS(VEC_OPS_FIELD, TYPE)                                     \
	};                                                                         \
                                                                               \
	/* ---------- Vec 结构体 ---------- */                                     \
	struct Vec_##TYPE {                                                        \
		const VecOps_##TYPE *ops;                                              \
		TYPE *data;                                                            \
		size_t size;                                                           \
		size_t capacity;                                                       \
	};                                                                         \
                                                                               \
	/* ====== 所有 static 函数前向声明 ====== */                                \
	VEC_OPS(VEC_FWD, TYPE)                  \
                                                                               \
	/* ---------- 默认操作表实例 ---------- */                                 \
	static const VecOps_##TYPE vec_##TYPE##_ops = {                            \
		VEC_OPS(VEC_TABLE, TYPE)                                         \
	};                          \
                                                                               \
	/* ---------- 函数实现（顺序随意） ---------- */                           \
	static void vec_##TYPE##_init(Vec_##TYPE *self) {                          \
		self->ops = &vec_##TYPE##_ops;                                         \
		self->data = NULL;                                                     \
		self->size = 0;                                                        \
		self->capacity = 0;                                                    \
	}                                                                          \
                                                                               \
	static void vec_##TYPE##_init_fill(Vec_##TYPE *self, size_t n, TYPE val) { \
		vec_##TYPE##_init(self);                                               \
		vec_##TYPE##_resize(self, n);                                          \
		for (size_t i = 0; i < n; ++i)                                         \
			self->data[i] = val;                                               \
	}                                                                          \
                                                                               \
	static void vec_##TYPE##_init_copy(                                        \
		Vec_##TYPE *self, const Vec_##TYPE *other, size_t start, size_t end) { \
		vec_##TYPE##_init(self);                                               \
		assert(start <= end && end <= other->size);                            \
		size_t len = end - start;                                              \
		vec_##TYPE##_resize(self, len);                                        \
		for (size_t i = 0; i < len; ++i)                                       \
			self->data[i] = other->data[start + i];                            \
	}                                                                          \
                                                                               \
	static void vec_##TYPE##_destroy(Vec_##TYPE *self) {                       \
		free(self->data);                                                      \
		self->data = NULL;                                                     \
		self->size = 0;                                                        \
		self->capacity = 0;                                                    \
	}                                                                          \
                                                                               \
	static void vec_##TYPE##_reserve(Vec_##TYPE *self, size_t new_cap) {       \
		if (new_cap > self->capacity) {                                        \
			TYPE *tmp = (TYPE *)realloc(self->data, new_cap * sizeof(TYPE));   \
			assert(tmp != NULL);                                               \
			self->data = tmp;                                                  \
			self->capacity = new_cap;                                          \
		}                                                                      \
	}                                                                          \
                                                                               \
	static void vec_##TYPE##_resize(Vec_##TYPE *self, size_t new_size) {       \
		if (new_size > self->capacity) {                                       \
			size_t new_cap =                                                   \
				new_size > self->capacity * 2 ? new_size : self->capacity * 2; \
			if (new_cap == 0)                                                  \
				new_cap = VEC_DEFAULT_CAPACITY;                                \
			vec_##TYPE##_reserve(self, new_cap);                               \
		}                                                                      \
		self->size = new_size;                                                 \
	}                                                                          \
                                                                               \
	static TYPE vec_##TYPE##_get(Vec_##TYPE *self, size_t index) {             \
		assert(index < self->size);                                            \
		return self->data[index];                                              \
	}                                                                          \
                                                                               \
	static void vec_##TYPE##_set(Vec_##TYPE *self, size_t index, TYPE val) {   \
		assert(index < self->size);                                            \
		self->data[index] = val;                                               \
	}                                                                          \
                                                                               \
	static void vec_##TYPE##_push(Vec_##TYPE *self, TYPE val) {                \
		if (self->size == self->capacity) {                                    \
			size_t new_cap = self->capacity == 0 ? VEC_DEFAULT_CAPACITY        \
												 : self->capacity * 2;         \
			vec_##TYPE##_reserve(self, new_cap);                               \
		}                                                                      \
		self->data[self->size++] = val;                                        \
	}                                                                          \
                                                                               \
	static TYPE vec_##TYPE##_pop(Vec_##TYPE *self) {                           \
		assert(self->size > 0);                                                \
		return self->data[--self->size];                                       \
	}                                                                          \
                                                                               \
	static size_t vec_##TYPE##_size(Vec_##TYPE *self) { return self->size; }   \
	static size_t vec_##TYPE##_capacity(Vec_##TYPE *self) {                    \
		return self->capacity;                                                 \
	}                                                                          \
	static int vec_##TYPE##_empty(Vec_##TYPE *self) {                          \
		return self->size == 0;                                                \
	}                                                                          \
                                                                               \
	static TYPE vec_##TYPE##_front(Vec_##TYPE *self) {                         \
		assert(self->size > 0);                                                \
		return self->data[0];                                                  \
	}                                                                          \
                                                                               \
	static TYPE vec_##TYPE##_back(Vec_##TYPE *self) {                          \
		assert(self->size > 0);                                                \
		return self->data[self->size - 1];                                     \
	}                                                                          \
                                                                               \
	static TYPE *vec_##TYPE##_begin(Vec_##TYPE *self) { return self->data; }   \
	static TYPE *vec_##TYPE##_end(Vec_##TYPE *self) {                          \
		return self->data + self->size;                                        \
	}                                                                          \
                                                                               \
	static void vec_##TYPE##_clear(Vec_##TYPE *self) { self->size = 0; }       \
                                                                               \
	static void vec_##TYPE##_swap(Vec_##TYPE *self, Vec_##TYPE *other) {       \
		Vec_##TYPE tmp = *self;                                                \
		*self = *other;                                                        \
		*other = tmp;                                                          \
	}                                                                          \
                                                                               \
	static void vec_##TYPE##_shrink_to_fit(Vec_##TYPE *self) {                 \
		if (self->size < self->capacity) {                                     \
			TYPE *tmp =                                                        \
				(TYPE *)realloc(self->data, self->size * sizeof(TYPE));        \
			assert(tmp != NULL);                                               \
			self->data = tmp;                                                  \
			self->capacity = self->size;                                       \
		}                                                                      \
	}

#endif /* VEC_H */