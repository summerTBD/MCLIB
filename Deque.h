#ifndef DEQUE_H
#define DEQUE_H

#include <assert.h>
#include <stdlib.h>

/* 默认初始容量（可在包含前重定义） */
#ifndef DEQUE_DEFAULT_CAPACITY
#define DEQUE_DEFAULT_CAPACITY 4
#endif

/**
 * 宏 DEFINE_DEQUE(TYPE, NAME) —— 生成针对 TYPE 类型的双端队列
 *
 * 队列类型名 ： deque_##NAME
 * 操作表类型 ： deque_ops_##NAME
 * 操作表实例 ： deque_##NAME##_ops
 *
 * 调用方式：
 *   // 直接调用
 *   deque_int_push_back(&dq, 42);
 *   // 通过 ops 表调用
 *   dq.ops->push_back(&dq, 42);
 *
 * 注意：生成的函数和 ops 表均为 static，可以在头文件中安全使用。
 */
#define DEFINE_DEQUE(TYPE, NAME)                                               \
	/* ---------- ops 表前向声明 ---------- */                                 \
	typedef struct deque_ops_##NAME deque_ops_##NAME;                          \
                                                                               \
	/* ---------- 双端队列结构体 ---------- */                                 \
	typedef struct {                                                           \
		TYPE *data;                                                            \
		size_t capacity;                                                       \
		size_t head;                                                           \
		size_t tail;                                                           \
		size_t size;                                                           \
		const deque_ops_##NAME *ops; /* 指向操作表的常量指针 */                \
	} deque_##NAME;                                                            \
                                                                               \
	/* ---------- 内部扩容 ---------- */                                       \
	static void deque_##NAME##_grow(deque_##NAME *dq) {                        \
		size_t new_cap =                                                       \
			dq->capacity == 0 ? DEQUE_DEFAULT_CAPACITY : dq->capacity * 2;     \
		TYPE *new_data = (TYPE *)malloc(new_cap * sizeof(TYPE));               \
		assert(new_data != NULL);                                              \
		for (size_t i = 0; i < dq->size; ++i) {                                \
			new_data[i] = dq->data[(dq->head + i) % dq->capacity];             \
		}                                                                      \
		free(dq->data);                                                        \
		dq->data = new_data;                                                   \
		dq->head = 0;                                                          \
		dq->tail = dq->size;                                                   \
		dq->capacity = new_cap;                                                \
	}                                                                          \
                                                                               \
	/* ---------- 初始化 ---------- */                                         \
	static void deque_##NAME##_init(deque_##NAME *dq, size_t init_cap) {       \
		if (init_cap == 0)                                                     \
			init_cap = DEQUE_DEFAULT_CAPACITY;                                 \
		dq->data = (TYPE *)malloc(init_cap * sizeof(TYPE));                    \
		assert(dq->data != NULL);                                              \
		dq->capacity = init_cap;                                               \
		dq->head = 0;                                                          \
		dq->tail = 0;                                                          \
		dq->size = 0;                                                          \
		dq->ops = &deque_##NAME##_ops; /* 绑定 ops 表 */                       \
	}                                                                          \
                                                                               \
	/* ---------- 销毁 ---------- */                                           \
	static void deque_##NAME##_destroy(deque_##NAME *dq) {                     \
		free(dq->data);                                                        \
		dq->data = NULL;                                                       \
		dq->capacity = dq->head = dq->tail = dq->size = 0;                     \
	}                                                                          \
                                                                               \
	/* ---------- 尾部插入 ---------- */                                       \
	static void deque_##NAME##_push_back(deque_##NAME *dq, TYPE val) {         \
		if (dq->size == dq->capacity)                                          \
			deque_##NAME##_grow(dq);                                           \
		dq->data[dq->tail] = val;                                              \
		dq->tail = (dq->tail + 1) % dq->capacity;                              \
		dq->size++;                                                            \
	}                                                                          \
                                                                               \
	/* ---------- 头部插入 ---------- */                                       \
	static void deque_##NAME##_push_front(deque_##NAME *dq, TYPE val) {        \
		if (dq->size == dq->capacity)                                          \
			deque_##NAME##_grow(dq);                                           \
		dq->head = (dq->head - 1 + dq->capacity) % dq->capacity;               \
		dq->data[dq->head] = val;                                              \
		dq->size++;                                                            \
	}                                                                          \
                                                                               \
	/* ---------- 尾部弹出 ---------- */                                       \
	static TYPE deque_##NAME##_pop_back(deque_##NAME *dq) {                    \
		assert(dq->size > 0);                                                  \
		dq->tail = (dq->tail - 1 + dq->capacity) % dq->capacity;               \
		TYPE ret = dq->data[dq->tail];                                         \
		dq->size--;                                                            \
		return ret;                                                            \
	}                                                                          \
                                                                               \
	/* ---------- 头部弹出 ---------- */                                       \
	static TYPE deque_##NAME##_pop_front(deque_##NAME *dq) {                   \
		assert(dq->size > 0);                                                  \
		TYPE ret = dq->data[dq->head];                                         \
		dq->head = (dq->head + 1) % dq->capacity;                              \
		dq->size--;                                                            \
		return ret;                                                            \
	}                                                                          \
                                                                               \
	/* ---------- 访问头部 ---------- */                                       \
	static TYPE deque_##NAME##_front(const deque_##NAME *dq) {                 \
		assert(dq->size > 0);                                                  \
		return dq->data[dq->head];                                             \
	}                                                                          \
                                                                               \
	/* ---------- 访问尾部 ---------- */                                       \
	static TYPE deque_##NAME##_back(const deque_##NAME *dq) {                  \
		assert(dq->size > 0);                                                  \
		return dq->data[(dq->tail - 1 + dq->capacity) % dq->capacity];         \
	}                                                                          \
                                                                               \
	/* ---------- 判空 ---------- */                                           \
	static int deque_##NAME##_empty(const deque_##NAME *dq) {                  \
		return dq->size == 0;                                                  \
	}                                                                          \
                                                                               \
	/* ---------- 大小 ---------- */                                           \
	static size_t deque_##NAME##_size(const deque_##NAME *dq) {                \
		return dq->size;                                                       \
	}                                                                          \
                                                                               \
	/* ---------- 清空 ---------- */                                           \
	static void deque_##NAME##_clear(deque_##NAME *dq) {                       \
		dq->head = 0;                                                          \
		dq->tail = 0;                                                          \
		dq->size = 0;                                                          \
	}                                                                          \
                                                                               \
	/* ---------- ops 表完整定义 ---------- */                                 \
	struct deque_ops_##NAME {                                                  \
		void (*push_back)(deque_##NAME *, TYPE);                               \
		void (*push_front)(deque_##NAME *, TYPE);                              \
		TYPE (*pop_back)(deque_##NAME *);                                      \
		TYPE (*pop_front)(deque_##NAME *);                                     \
		TYPE (*front)(const deque_##NAME *);                                   \
		TYPE (*back)(const deque_##NAME *);                                    \
		int (*empty)(const deque_##NAME *);                                    \
		size_t (*size)(const deque_##NAME *);                                  \
		void (*clear)(deque_##NAME *);                                         \
		void (*destroy)(deque_##NAME *);                                       \
	};                                                                         \
                                                                               \
	/* ---------- 全局 ops 表实例（使用指定初始化器） ---------- */            \
	static const deque_ops_##NAME deque_##NAME##_ops = {                       \
		.push_back = deque_##NAME##_push_back,                                 \
		.push_front = deque_##NAME##_push_front,                               \
		.pop_back = deque_##NAME##_pop_back,                                   \
		.pop_front = deque_##NAME##_pop_front,                                 \
		.front = deque_##NAME##_front,                                         \
		.back = deque_##NAME##_back,                                           \
		.empty = deque_##NAME##_empty,                                         \
		.size = deque_##NAME##_size,                                           \
		.clear = deque_##NAME##_clear,                                         \
		.destroy = deque_##NAME##_destroy};

#endif /* DEQUE_H */