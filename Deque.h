#ifndef DEQUE_H
#define DEQUE_H

#include <assert.h>
#include <stdlib.h>

#ifndef DEQUE_DEFAULT_CAPACITY
#define DEQUE_DEFAULT_CAPACITY 4
#endif

/* ===================================================================
 * X-Macro: Deque 操作总表 —— 单一数据源
 * 格式: M(容器名, 操作名, 返回类型, (参数列表))
 * =================================================================== */
#define DEQUE_OPS(M, TYPE)                                     \
    M(TYPE, push_back, void, (Deque_##TYPE * self, TYPE val))  \
    M(TYPE, push_front, void, (Deque_##TYPE * self, TYPE val)) \
    M(TYPE, pop_back, TYPE, (Deque_##TYPE * self))             \
    M(TYPE, pop_front, TYPE, (Deque_##TYPE * self))            \
    M(TYPE, front, TYPE, (const Deque_##TYPE* self))           \
    M(TYPE, back, TYPE, (const Deque_##TYPE* self))            \
    M(TYPE, empty, int, (const Deque_##TYPE* self))            \
    M(TYPE, size, size_t, (const Deque_##TYPE* self))          \
    M(TYPE, clear, void, (Deque_##TYPE * self))                \
    M(TYPE, destroy, void, (Deque_##TYPE * self))

/* X-Macro: 只为生成前向声明 */
#define DEQUE_FWD(TYPE, op, ret, params) static ret deque_##TYPE##_##op params;

#define DEFINE_DEQUE(TYPE)                                                     \
    /* ---------- 前向声明 ---------- */                                       \
    typedef struct Deque_##TYPE Deque_##TYPE;                                  \
                                                                               \
    /* ---------- Deque 结构体 ---------- */                                   \
    struct Deque_##TYPE {                                                      \
        TYPE* data;                                                            \
        size_t capacity;                                                       \
        size_t head;                                                           \
        size_t tail;                                                           \
        size_t size;                                                           \
    };                                                                         \
                                                                               \
    /* ====== 所有 static 函数前向声明 ====== */                               \
    DEQUE_OPS(DEQUE_FWD, TYPE)                                                 \
    static void deque_##TYPE##_grow(Deque_##TYPE* self);                       \
    static void deque_##TYPE##_init(Deque_##TYPE* self, size_t init_cap);      \
                                                                               \
    /* ---------- 函数实现 ---------- */                                       \
    static void deque_##TYPE##_grow(Deque_##TYPE* self) {                      \
        size_t new_cap =                                                       \
            self->capacity == 0 ? DEQUE_DEFAULT_CAPACITY : self->capacity * 2; \
        TYPE* new_data = (TYPE*)malloc(new_cap * sizeof(TYPE));                \
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
    static void deque_##TYPE##_init(Deque_##TYPE* self, size_t init_cap) {     \
        if (init_cap == 0)                                                     \
            init_cap = DEQUE_DEFAULT_CAPACITY;                                 \
        self->data = (TYPE*)malloc(init_cap * sizeof(TYPE));                   \
        assert(self->data != NULL);                                            \
        self->capacity = init_cap;                                             \
        self->head = 0;                                                        \
        self->tail = 0;                                                        \
        self->size = 0;                                                        \
    }                                                                          \
                                                                               \
    static void deque_##TYPE##_destroy(Deque_##TYPE* self) {                   \
        free(self->data);                                                      \
        self->data = NULL;                                                     \
        self->capacity = self->head = self->tail = self->size = 0;             \
    }                                                                          \
                                                                               \
    static void deque_##TYPE##_push_back(Deque_##TYPE* self, TYPE val) {       \
        if (self->size == self->capacity)                                      \
            deque_##TYPE##_grow(self);                                         \
        self->data[self->tail] = val;                                          \
        self->tail = (self->tail + 1) % self->capacity;                        \
        self->size++;                                                          \
    }                                                                          \
                                                                               \
    static void deque_##TYPE##_push_front(Deque_##TYPE* self, TYPE val) {      \
        if (self->size == self->capacity)                                      \
            deque_##TYPE##_grow(self);                                         \
        self->head = (self->head - 1 + self->capacity) % self->capacity;       \
        self->data[self->head] = val;                                          \
        self->size++;                                                          \
    }                                                                          \
                                                                               \
    static TYPE deque_##TYPE##_pop_back(Deque_##TYPE* self) {                  \
        assert(self->size > 0);                                                \
        self->tail = (self->tail - 1 + self->capacity) % self->capacity;       \
        TYPE ret = self->data[self->tail];                                     \
        self->size--;                                                          \
        return ret;                                                            \
    }                                                                          \
                                                                               \
    static TYPE deque_##TYPE##_pop_front(Deque_##TYPE* self) {                 \
        assert(self->size > 0);                                                \
        TYPE ret = self->data[self->head];                                     \
        self->head = (self->head + 1) % self->capacity;                        \
        self->size--;                                                          \
        return ret;                                                            \
    }                                                                          \
                                                                               \
    static TYPE deque_##TYPE##_front(const Deque_##TYPE* self) {               \
        assert(self->size > 0);                                                \
        return self->data[self->head];                                         \
    }                                                                          \
                                                                               \
    static TYPE deque_##TYPE##_back(const Deque_##TYPE* self) {                \
        assert(self->size > 0);                                                \
        return self->data[(self->tail - 1 + self->capacity) % self->capacity]; \
    }                                                                          \
                                                                               \
    static int deque_##TYPE##_empty(const Deque_##TYPE* self) {                \
        return self->size == 0;                                                \
    }                                                                          \
                                                                               \
    static size_t deque_##TYPE##_size(const Deque_##TYPE* self) {              \
        return self->size;                                                     \
    }                                                                          \
                                                                               \
    static void deque_##TYPE##_clear(Deque_##TYPE* self) {                     \
        self->head = 0;                                                        \
        self->tail = 0;                                                        \
        self->size = 0;                                                        \
    }

#endif /* DEQUE_H */