#ifndef BOX_H
#define BOX_H

#include <stdio.h>
#include <stdlib.h>

/* ===================================================================
 * X-Macro: Box 操作列表 —— 单一数据源
 * 格式: M(TYPE, 操作名, 返回类型, (参数列表))
 * =================================================================== */
#define BOX_OPS(M, TYPE)                               \
    M(TYPE, init, void, (Box_##TYPE * self, TYPE val)) \
    M(TYPE, deref, TYPE*, (const Box_##TYPE* self))    \
    M(TYPE, destroy, void, (Box_##TYPE * self))

/* X-Macro: 只为生成前向声明 */
#define BOX_FWD(TYPE, op, ret, params) static ret box_##TYPE##_##op params;

#define DEFINE_BOX(TYPE)                                        \
    /* ---------- Box 结构体 ---------- */                      \
    typedef struct {                                            \
        TYPE* ptr;                                              \
    } Box_##TYPE;                                               \
                                                                \
    /* ====== 前向声明 ====== */                                \
    BOX_OPS(BOX_FWD, TYPE)                                      \
                                                                \
    /* ---------- 函数实现 ---------- */                        \
    static void box_##TYPE##_init(Box_##TYPE* self, TYPE val) { \
        self->ptr = (TYPE*)malloc(sizeof(TYPE));                \
        if (!self->ptr) {                                       \
            fprintf(stderr, "Box_init: OOM\n");                 \
            abort();                                            \
        }                                                       \
        *self->ptr = val;                                       \
    }                                                           \
                                                                \
    static TYPE* box_##TYPE##_deref(const Box_##TYPE* self) {   \
        if (!self->ptr) {                                       \
            fprintf(stderr, "Box_deref: use-after-destroy\n");  \
            abort();                                            \
        }                                                       \
        return self->ptr;                                       \
    }                                                           \
                                                                \
    static void box_##TYPE##_destroy(Box_##TYPE* self) {        \
        free(self->ptr);                                        \
        self->ptr = NULL;                                       \
    }

#endif