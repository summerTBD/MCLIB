#ifndef BOX_H
#define BOX_H

#include <assert.h>
#include <stdlib.h>

/* ===================================================================
 * X-Macro: Box 操作总表 —— 单一数据源
 * 格式: M(TYPE, 操作名, 返回类型, (参数列表))
 *
 * 只用 TYPE, 不用 NAME:
 *   DEFINE_BOX(int) → Box_int    (只有一份, 不会重复)
 *   DEFINE_BOX(int) → Box_int    (再调也是同一个类型, 不会生成两份)
 *
 *   Box<T> 由 T 唯一确定, 不需要额外起名.
 *   复杂类型先 typedef: typedef struct Foo Foo; DEFINE_BOX(Foo)
 * =================================================================== */
#define BOX_OPS(M, TYPE)                               \
    M(TYPE, init, void, (Box_##TYPE * self, TYPE val)) \
    M(TYPE, deref, TYPE, (const Box_##TYPE* self))     \
    M(TYPE, destroy, void, (Box_##TYPE * self))

/* X-Macro helper —— M 的第一个参数永远是 TYPE, 后面三个是 op/ret/params */
#define BOX_OPS_FIELD(TYPE, op, ret, params) ret(*op) params;
#define BOX_FWD(TYPE, op, ret, params) static ret box_##TYPE##_##op params;
#define BOX_TABLE(TYPE, op, ret, params) .op = box_##TYPE##_##op,

#define DEFINE_BOX(TYPE)                                        \
    /* ---------- 前向声明 ---------- */                        \
    typedef struct Box_##TYPE Box_##TYPE;                       \
    typedef struct BoxOps_##TYPE BoxOps_##TYPE;                 \
                                                                \
    /* ---------- 操作表结构 ---------- */                      \
    struct BoxOps_##TYPE {                                      \
        BOX_OPS(BOX_OPS_FIELD, TYPE)                            \
    };                                                          \
                                                                \
    /* ---------- Box 结构体 ---------- */                      \
    struct Box_##TYPE {                                         \
        const BoxOps_##TYPE* ops;                               \
        TYPE* ptr;                                              \
    };                                                          \
                                                                \
    /* ====== 所有 static 函数前向声明 ====== */                \
    BOX_OPS(BOX_FWD, TYPE)                                      \
                                                                \
    /* ---------- 默认操作表实例 ---------- */                  \
    static const BoxOps_##TYPE box_##TYPE##_ops = {             \
        BOX_OPS(BOX_TABLE, TYPE)};                              \
                                                                \
    /* ---------- 函数实现 ---------- */                        \
    static void box_##TYPE##_init(Box_##TYPE* self, TYPE val) { \
        self->ptr = (TYPE*)malloc(sizeof(TYPE));                \
        assert(self->ptr != NULL);                              \
        *self->ptr = val;                                       \
        self->ops = &box_##TYPE##_ops;                          \
    }                                                           \
                                                                \
    static TYPE box_##TYPE##_deref(const Box_##TYPE* self) {    \
        assert(self->ptr != NULL);                              \
        return *self->ptr;                                      \
    }                                                           \
                                                                \
    static void box_##TYPE##_destroy(Box_##TYPE* self) {        \
        free(self->ptr);                                        \
        self->ptr = NULL;                                       \
    }

#endif