#ifndef DEQUE_H
#define DEQUE_H

#include <assert.h>
#include <stdlib.h>

#ifndef DEQUE_DEFAULT_CAPACITY
#define DEQUE_DEFAULT_CAPACITY 4
#endif

#define DEFINE_DEQUE(TYPE, NAME)                                               \
	/* ---------- 前向声明 ---------- */                                       \
	typedef struct Deque_##NAME Deque_##NAME;                                  \
	typedef struct DequeOps_##NAME DequeOps_##NAME;                            \
                                                                               \
	/* ---------- 操作表结构 ---------- */                                     \
	struct DequeOps_##NAME {                                                   \
		void (*push_back)(Deque_##NAME * self, TYPE val);                      \
		void (*push_front)(Deque_##NAME * self, TYPE val);                     \
		TYPE (*pop_back)(Deque_##NAME * self);                                 \
		TYPE (*pop_front)(Deque_##NAME * self);                                \
		TYPE (*front)(const Deque_##NAME *self);                               \
		TYPE (*back)(const Deque_##NAME *self);                                \
		int (*empty)(const Deque_##NAME *self);                                \
		size_t (*size)(const Deque_##NAME *self);                              \
		void (*clear)(Deque_##NAME * self);                                    \
		void (*destroy)(Deque_##NAME * self);                                  \
	};                                                                         \
                                                                               \
	/* ---------- Deque 结构体 ---------- */                                   \
	struct Deque_##NAME {                                                      \
		const DequeOps_##NAME *ops;                                            \
		TYPE *data;                                                            \
		size_t capacity;                                                       \
		size_t head;                                                           \
		size_t tail;                                                           \
		size_t size;                                                           \
	};                                                                         \
                                                                               \
	/* ====== 所有 static 函数前向声明 ====== */                               \
	static void deque_##NAME##_push_back(Deque_##NAME *self, TYPE val);        \
	static void deque_##NAME##_push_front(Deque_##NAME *self, TYPE val);       \
	static TYPE deque_##NAME##_pop_back(Deque_##NAME *self);                   \
	static TYPE deque_##NAME##_pop_front(Deque_##NAME *self);                  \
	static TYPE deque_##NAME##_front(const Deque_##NAME *self);                \
	static TYPE deque_##NAME##_back(const Deque_##NAME *self);                 \
	static int deque_##NAME##_empty(const Deque_##NAME *self);                 \
	static size_t deque_##NAME##_size(const Deque_##NAME *self);               \
	static void deque_##NAME##_clear(Deque_##NAME *self);                      \
	static void deque_##NAME##_destroy(Deque_##NAME *self);                    \
	static void deque_##NAME##_grow(Deque_##NAME *self);                       \
	static void deque_##NAME##_init(Deque_##NAME *self, size_t init_cap);      \
                                                                               \
	/* ---------- 默认操作表实例 ---------- */                                 \
	static const DequeOps_##NAME deque_##NAME##_ops = {                        \
		.push_back = deque_##NAME##_push_back,                                 \
		.push_front = deque_##NAME##_push_front,                               \
		.pop_back = deque_##NAME##_pop_back,                                   \
		.pop_front = deque_##NAME##_pop_front,                                 \
		.front = deque_##NAME##_front,                                         \
		.back = deque_##NAME##_back,                                           \
		.empty = deque_##NAME##_empty,                                         \
		.size = deque_##NAME##_size,                                           \
		.clear = deque_##NAME##_clear,                                         \
		.destroy = deque_##NAME##_destroy};                                    \
                                                                               \
	/* ---------- 函数实现 ---------- */                                       \
	static void deque_##NAME##_grow(Deque_##NAME *self) {                      \
		size_t new_cap =                                                       \
			self->capacity == 0 ? DEQUE_DEFAULT_CAPACITY : self->capacity * 2; \
		TYPE *new_data = (TYPE *)malloc(new_cap * sizeof(TYPE));               \
		assert(new_data != NULL);                                              \
		for (size_t i = 0; i < self->size; ++i)                                \
			new_data[i] = self->data[(self->head + i) % self->capacity];       \
		free(self->data);                                                      \
		self->data = new_data;                                                 \
		self->head = 0;                                                        \
		self->tail = self->size;                                               \
		self->capacity = new_cap;                                              \
	}                                                                          \
                                                                               \
	static void deque_##NAME##_init(Deque_##NAME *self, size_t init_cap) {     \
		if (init_cap == 0)                                                     \
			init_cap = DEQUE_DEFAULT_CAPACITY;                                 \
		self->data = (TYPE *)malloc(init_cap * sizeof(TYPE));                  \
		assert(self->data != NULL);                                            \
		self->capacity = init_cap;                                             \
		self->head = 0;                                                        \
		self->tail = 0;                                                        \
		self->size = 0;                                                        \
		self->ops = &deque_##NAME##_ops;                                       \
	}                                                                          \
                                                                               \
	static void deque_##NAME##_destroy(Deque_##NAME *self) {                   \
		free(self->data);                                                      \
		self->data = NULL;                                                     \
		self->capacity = self->head = self->tail = self->size = 0;             \
	}                                                                          \
                                                                               \
	static void deque_##NAME##_push_back(Deque_##NAME *self, TYPE val) {       \
		if (self->size == self->capacity)                                      \
			deque_##NAME##_grow(self);                                         \
		self->data[self->tail] = val;                                          \
		self->tail = (self->tail + 1) % self->capacity;                        \
		self->size++;                                                          \
	}                                                                          \
                                                                               \
	static void deque_##NAME##_push_front(Deque_##NAME *self, TYPE val) {      \
		if (self->size == self->capacity)                                      \
			deque_##NAME##_grow(self);                                         \
		self->head = (self->head - 1 + self->capacity) % self->capacity;       \
		self->data[self->head] = val;                                          \
		self->size++;                                                          \
	}                                                                          \
                                                                               \
	static TYPE deque_##NAME##_pop_back(Deque_##NAME *self) {                  \
		assert(self->size > 0);                                                \
		self->tail = (self->tail - 1 + self->capacity) % self->capacity;       \
		TYPE ret = self->data[self->tail];                                     \
		self->size--;                                                          \
		return ret;                                                            \
	}                                                                          \
                                                                               \
	static TYPE deque_##NAME##_pop_front(Deque_##NAME *self) {                 \
		assert(self->size > 0);                                                \
		TYPE ret = self->data[self->head];                                     \
		self->head = (self->head + 1) % self->capacity;                        \
		self->size--;                                                          \
		return ret;                                                            \
	}                                                                          \
                                                                               \
	static TYPE deque_##NAME##_front(const Deque_##NAME *self) {               \
		assert(self->size > 0);                                                \
		return self->data[self->head];                                         \
	}                                                                          \
                                                                               \
	static TYPE deque_##NAME##_back(const Deque_##NAME *self) {                \
		assert(self->size > 0);                                                \
		return self->data[(self->tail - 1 + self->capacity) % self->capacity]; \
	}                                                                          \
                                                                               \
	static int deque_##NAME##_empty(const Deque_##NAME *self) {                \
		return self->size == 0;                                                \
	}                                                                          \
                                                                               \
	static size_t deque_##NAME##_size(const Deque_##NAME *self) {              \
		return self->size;                                                     \
	}                                                                          \
                                                                               \
	static void deque_##NAME##_clear(Deque_##NAME *self) {                     \
		self->head = 0;                                                        \
		self->tail = 0;                                                        \
		self->size = 0;                                                        \
	}

#endif /* DEQUE_H */