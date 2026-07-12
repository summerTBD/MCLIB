#ifndef RC_H
#define RC_H

#include <stdio.h>
#include <stdlib.h>

/* ===================================================================
 * X-Macro: Rc 操作列表 —— 单一数据源
 * 格式: M(TYPE, 操作名, 返回类型, (参数列表))
 * =================================================================== */
#define RC_OPS(M, TYPE)                                           \
    M(TYPE, init, void, (Rc_##TYPE * self, TYPE val))             \
    M(TYPE, init_move, void, (Rc_##TYPE * self, const TYPE* src)) \
    M(TYPE, clone, Rc_##TYPE, (const Rc_##TYPE* self))            \
    M(TYPE, deref, TYPE*, (const Rc_##TYPE* self))                \
    M(TYPE, drop, void, (Rc_##TYPE * self))

/* X-Macro: 只为生成前向声明 */
#define RC_FWD(TYPE, op, ret, params) static ret rc_##TYPE##_##op params;

#define DEFINE_RC(TYPE)                                         \
    /* ---------- 内部堆结构 ---------- */                      \
    typedef struct {                                            \
        int refcount;                                           \
        TYPE data;                                              \
    } RcInner_##TYPE;                                           \
                                                                \
    /* ---------- Rc 句柄 ---------- */                         \
    typedef struct {                                            \
        RcInner_##TYPE* inner;                                  \
    } Rc_##TYPE;                                                \
                                                                \
    /* ====== 前向声明 ====== */                                \
    RC_OPS(RC_FWD, TYPE)                                        \
                                                                \
    /* ---------- 函数实现 ---------- */                        \
    static void rc_##TYPE##_init(Rc_##TYPE* self, TYPE val) {   \
        RcInner_##TYPE* inner =                                 \
            (RcInner_##TYPE*)malloc(sizeof(RcInner_##TYPE));    \
        if (!inner) {                                           \
            fprintf(stderr, "Rc_init: OOM\n");                  \
            abort();                                            \
        }                                                       \
        inner->refcount = 1;                                    \
        inner->data = val;                                      \
        self->inner = inner;                                    \
    }                                                           \
                                                                \
    static void rc_##TYPE##_init_move(Rc_##TYPE* self,          \
                                      const TYPE* src) {        \
        RcInner_##TYPE* inner =                                 \
            (RcInner_##TYPE*)malloc(sizeof(RcInner_##TYPE));    \
        if (!inner) {                                           \
            fprintf(stderr, "Rc_init_move: OOM\n");             \
            abort();                                            \
        }                                                       \
        if (!src) {                                             \
            fprintf(stderr, "Rc_init_move: NULL src\n");        \
            abort();                                            \
        }                                                       \
        inner->refcount = 1;                                    \
        inner->data = *src;                                     \
        self->inner = inner;                                    \
    }                                                           \
                                                                \
    static Rc_##TYPE rc_##TYPE##_clone(const Rc_##TYPE* self) { \
        if (!self->inner) {                                     \
            fprintf(stderr, "Rc_clone: use-after-drop\n");      \
            abort();                                            \
        }                                                       \
        self->inner->refcount++;                                \
        return *self;                                           \
    }                                                           \
                                                                \
    static TYPE* rc_##TYPE##_deref(const Rc_##TYPE* self) {     \
        if (!self->inner) {                                     \
            fprintf(stderr, "Rc_deref: use-after-drop\n");      \
            abort();                                            \
        }                                                       \
        return &self->inner->data;                              \
    }                                                           \
                                                                \
    static void rc_##TYPE##_drop(Rc_##TYPE* self) {             \
        if (!self->inner) {                                     \
            return; /* 已经 drop 过了, 幂等 */                  \
        }                                                       \
        if (--self->inner->refcount == 0) {                     \
            free(self->inner);                                  \
        }                                                       \
        self->inner = NULL;                                     \
    }

#endif
