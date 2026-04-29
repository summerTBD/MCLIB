#ifndef VEC_H
#define VEC_H

#include <stdbool.h>
#include <stddef.h>

#define ElemType int

#define VEC_MAX(a, b) ((a) > (b) ? (a) : (b))
#define VEC_MIN(a, b) ((a) < (b) ? (a) : (b))

/* 前向声明 */
typedef struct Vec Vec;
typedef struct VecOps VecOps;

/* 操作表：函数指针集合 */
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

/* Vec 核心结构 */
struct Vec {
	const struct VecOps *ops;
	ElemType *data;
	size_t size;
	size_t capacity;
};

/* 全局默认操作表声明 */
extern const VecOps vec_default_ops;

#endif /* VEC_H */