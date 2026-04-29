#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stddef.h>

#define ElemType int

/* 前向声明 */
typedef struct ListNode ListNode;
typedef struct List List;
typedef struct ListOps ListOps;

/* 链表节点 */
struct ListNode {
	ElemType data;
	struct ListNode *prev;
	struct ListNode *next;
};

/* 操作表：函数指针集合 */
struct ListOps {
	void (*init)(List *self);
	void (*destroy)(List *self);
	bool (*is_empty)(List *self);
	ElemType (*get)(List *self, size_t index);
	void (*set)(List *self, size_t index, ElemType value);
	void (*push)(List *self, ElemType value);
	void (*insert)(List *self, size_t index, ElemType value);
	ElemType (*pop)(List *self);
	ElemType (*remove)(List *self, size_t index);
	size_t (*size)(List *self);
	void (*clear)(List *self);
};

/* 链表核心结构 */
struct List {
	ListNode *head;
	ListNode *tail;
	size_t size;
	const ListOps *ops;
};

/* 全局默认操作表声明 */
extern const ListOps default_list_ops;

#endif /* LIST_H */