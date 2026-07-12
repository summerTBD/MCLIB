#ifndef LIST_H
#define LIST_H

#include <assert.h>
#include <stdlib.h>

/* =================================================================
 * X-Macro: 操作列表
 *
 * 所有操作在此统一定义，一处修改即可同步更新:
 *   1. ListOps 结构体中的函数指针字段
 *   2. static 函数前向声明
 *   3. 默认操作表实例 (list_##TYPE##_ops)
 *
 * 格式: X(TYPE, 返回类型, 函数名, (参数列表))
 * ================================================================= */
#define LIST_OPS(X, TYPE)                                               \
    X(TYPE, void, init, (List_##TYPE * self))                           \
    X(TYPE, void, destroy, (List_##TYPE * self))                        \
    X(TYPE, int, empty, (List_##TYPE * self))                           \
    X(TYPE, TYPE, get, (List_##TYPE * self, size_t index))              \
    X(TYPE, void, set, (List_##TYPE * self, size_t index, TYPE val))    \
    X(TYPE, void, push, (List_##TYPE * self, TYPE val))                 \
    X(TYPE, void, insert, (List_##TYPE * self, size_t index, TYPE val)) \
    X(TYPE, TYPE, pop, (List_##TYPE * self))                            \
    X(TYPE, TYPE, remove_at, (List_##TYPE * self, size_t index))        \
    X(TYPE, size_t, size, (List_##TYPE * self))                         \
    X(TYPE, void, clear, (List_##TYPE * self))

/* X-Macro 展开器: 生成 static 函数前向声明 */
#define LIST_OPS_FWD_DECL(TYPE, ret, name, params) \
    static ret list_##TYPE##_##name params;

#define DEFINE_LIST(TYPE)                                                      \
    /* ---------- 节点结构 ---------- */                                       \
    typedef struct ListNode_##TYPE {                                           \
        TYPE data;                                                             \
        struct ListNode_##TYPE* prev;                                          \
        struct ListNode_##TYPE* next;                                          \
    } ListNode_##TYPE;                                                         \
                                                                               \
    /* 前向声明 */                                                             \
    typedef struct List_##TYPE List_##TYPE;                                    \
                                                                               \
    /* ---------- 链表主结构 ---------- */                                     \
    struct List_##TYPE {                                                       \
        ListNode_##TYPE* head;                                                 \
        ListNode_##TYPE* tail;                                                 \
        size_t count;                                                          \
    };                                                                         \
                                                                               \
    /* ====== 所有 static 函数前向声明 ====== */                               \
    static ListNode_##TYPE* list_##TYPE##_node_ptr(List_##TYPE* self,          \
                                                   size_t index);              \
    LIST_OPS(LIST_OPS_FWD_DECL, TYPE)                                          \
                                                                               \
    /* ---------- 内部定位辅助函数 ---------- */                               \
    static ListNode_##TYPE* list_##TYPE##_node_ptr(List_##TYPE* self,          \
                                                   size_t index) {             \
        assert(index < self->count);                                           \
        ListNode_##TYPE* cur;                                                  \
        if (index < self->count / 2) {                                         \
            cur = self->head;                                                  \
            for (size_t i = 0; i < index; ++i)                                 \
                cur = cur->next;                                               \
        } else {                                                               \
            cur = self->tail;                                                  \
            for (size_t i = self->count - 1; i > index; --i)                   \
                cur = cur->prev;                                               \
        }                                                                      \
        return cur;                                                            \
    }                                                                          \
                                                                               \
    /* ---------- 其他函数实现 ---------- */                                   \
    static void list_##TYPE##_init(List_##TYPE* self) {                        \
        self->head = NULL;                                                     \
        self->tail = NULL;                                                     \
        self->count = 0;                                                       \
        }                                                                          \
                                                                               \
    static void list_##TYPE##_destroy(List_##TYPE* self) {                     \
        ListNode_##TYPE* cur = self->head;                                     \
        while (cur) {                                                          \
            ListNode_##TYPE* next = cur->next;                                 \
            free(cur);                                                         \
            cur = next;                                                        \
        }                                                                      \
        self->head = NULL;                                                     \
        self->tail = NULL;                                                     \
        self->count = 0;                                                       \
    }                                                                          \
                                                                               \
    static int list_##TYPE##_empty(List_##TYPE* self) {                        \
        return self->count == 0;                                               \
    }                                                                          \
                                                                               \
    static TYPE list_##TYPE##_get(List_##TYPE* self, size_t index) {           \
        assert(index < self->count);                                           \
        ListNode_##TYPE* cur = list_##TYPE##_node_ptr(self, index);            \
        return cur->data;                                                      \
    }                                                                          \
                                                                               \
    static void list_##TYPE##_set(List_##TYPE* self, size_t index, TYPE val) { \
        assert(index < self->count);                                           \
        ListNode_##TYPE* cur = list_##TYPE##_node_ptr(self, index);            \
        cur->data = val;                                                       \
    }                                                                          \
                                                                               \
    static void list_##TYPE##_push(List_##TYPE* self, TYPE val) {              \
        ListNode_##TYPE* node =                                                \
            (ListNode_##TYPE*)malloc(sizeof(ListNode_##TYPE));                 \
        assert(node != NULL);                                                  \
        node->data = val;                                                      \
        node->next = NULL;                                                     \
        if (self->count == 0) {                                                \
            node->prev = NULL;                                                 \
            self->head = node;                                                 \
            self->tail = node;                                                 \
        } else {                                                               \
            node->prev = self->tail;                                           \
            self->tail->next = node;                                           \
            self->tail = node;                                                 \
        }                                                                      \
        self->count += 1;                                                      \
    }                                                                          \
                                                                               \
    static void list_##TYPE##_insert(List_##TYPE* self, size_t index,          \
                                     TYPE val) {                               \
        assert(index <= self->count);                                          \
        if (index == self->count) {                                            \
            list_##TYPE##_push(self, val);                                     \
            return;                                                            \
        }                                                                      \
        ListNode_##TYPE* cur = list_##TYPE##_node_ptr(self, index);            \
        ListNode_##TYPE* node =                                                \
            (ListNode_##TYPE*)malloc(sizeof(ListNode_##TYPE));                 \
        assert(node != NULL);                                                  \
        node->data = val;                                                      \
        node->next = cur;                                                      \
        node->prev = cur->prev;                                                \
        cur->prev = node;                                                      \
        if (node->prev) {                                                      \
            node->prev->next = node;                                           \
        } else {                                                               \
            self->head = node;                                                 \
        }                                                                      \
        self->count += 1;                                                      \
    }                                                                          \
                                                                               \
    static TYPE list_##TYPE##_pop(List_##TYPE* self) {                         \
        assert(self->count > 0);                                               \
        ListNode_##TYPE* last = self->tail;                                    \
        TYPE val = last->data;                                                 \
        self->tail = last->prev;                                               \
        free(last);                                                            \
        if (self->tail) {                                                      \
            self->tail->next = NULL;                                           \
        } else {                                                               \
            self->head = NULL;                                                 \
        }                                                                      \
        self->count -= 1;                                                      \
        return val;                                                            \
    }                                                                          \
                                                                               \
    static TYPE list_##TYPE##_remove_at(List_##TYPE* self, size_t index) {     \
        assert(index < self->count);                                           \
        ListNode_##TYPE* cur = list_##TYPE##_node_ptr(self, index);            \
        TYPE val = cur->data;                                                  \
        if (cur->prev) {                                                       \
            cur->prev->next = cur->next;                                       \
        } else {                                                               \
            self->head = cur->next;                                            \
        }                                                                      \
        if (cur->next) {                                                       \
            cur->next->prev = cur->prev;                                       \
        } else {                                                               \
            self->tail = cur->prev;                                            \
        }                                                                      \
        free(cur);                                                             \
        self->count -= 1;                                                      \
        return val;                                                            \
    }                                                                          \
                                                                               \
    static size_t list_##TYPE##_size(List_##TYPE* self) {                      \
        return self->count;                                                    \
    }                                                                          \
                                                                               \
    static void list_##TYPE##_clear(List_##TYPE* self) {                       \
        ListNode_##TYPE* cur = self->head;                                     \
        while (cur) {                                                          \
            ListNode_##TYPE* next = cur->next;                                 \
            free(cur);                                                         \
            cur = next;                                                        \
        }                                                                      \
        self->head = NULL;                                                     \
        self->tail = NULL;                                                     \
        self->count = 0;                                                       \
    }

#endif /* LIST_H */