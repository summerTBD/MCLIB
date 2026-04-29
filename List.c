#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define ElemType int

/* ----- 节点与链表的基础结构 ----- */
typedef struct ListNode {
	ElemType data;
	struct ListNode *prev;
	struct ListNode *next;
} ListNode;

/* 前向声明，因为 ListOps 里要用到 List* */
typedef struct List List;

/* ----- 操作函数表（虚表） ----- */
typedef struct ListOps {
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
} ListOps;

/* ----- 链表主结构 ----- */
struct List {
	ListNode *head;
	ListNode *tail;
	size_t size;
	const ListOps *ops; // 指向操作表，完成多态调用
};

/* ----- 函数前向声明（供 ops 表使用） ----- */
static void list_init_impl(List *self);
static void list_destroy_impl(List *self);
static bool list_is_empty_impl(List *self);
static ElemType list_get_impl(List *self, size_t index);
static void list_set_impl(List *self, size_t index, ElemType value);
static void list_push_impl(List *self, ElemType value);
static void list_insert_impl(List *self, size_t index, ElemType value);
static ElemType list_pop_impl(List *self);
static ElemType list_remove_impl(List *self, size_t index);
static size_t list_size_impl(List *self);
static void list_clear_impl(List *self);

/* ----- 内部辅助函数（不暴露在 ops 中） ----- */
static ListNode *list_node_ptr(List *self, size_t index);

/* ----- 默认操作表实例 ----- */
static const ListOps default_list_ops = {.init = list_init_impl,
										 .destroy = list_destroy_impl,
										 .is_empty = list_is_empty_impl,
										 .get = list_get_impl,
										 .set = list_set_impl,
										 .push = list_push_impl,
										 .insert = list_insert_impl,
										 .pop = list_pop_impl,
										 .remove = list_remove_impl,
										 .size = list_size_impl,
										 .clear = list_clear_impl};

/* ================================================================
 * 操作的实现
 * 各函数内部通过 self->ops 调用其他操作（类似虚函数调用），
 * 以便后续可整体替换操作表。
 * ================================================================ */

static void list_init_impl(List *self) {
	self->head = NULL;
	self->tail = NULL;
	self->size = 0;
	self->ops = &default_list_ops; // 绑定默认操作表
}

static void list_destroy_impl(List *self) {
	ListNode *current = self->head;
	while (current) {
		ListNode *next = current->next;
		free(current);
		current = next;
	}
	self->head = NULL;
	self->tail = NULL;
	self->size = 0;
}

static bool list_is_empty_impl(List *self) { return self->size == 0; }

static ElemType list_get_impl(List *self, size_t index) {
	if (index >= self->size) {
		fprintf(stderr, "Index out of bounds\n");
		self->ops->destroy(self);
		exit(EXIT_FAILURE);
	}
	ListNode *current = list_node_ptr(self, index);
	return current->data;
}

static void list_set_impl(List *self, size_t index, ElemType value) {
	if (index >= self->size) {
		fprintf(stderr, "Index out of bounds\n");
		self->ops->destroy(self);
		exit(EXIT_FAILURE);
	}
	ListNode *current = list_node_ptr(self, index);
	current->data = value;
}

static void list_push_impl(List *self, ElemType value) {
	ListNode *new_node = (ListNode *)malloc(sizeof(ListNode));
	if (new_node == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		self->ops->destroy(self);
		exit(EXIT_FAILURE);
	}
	new_node->data = value;
	new_node->next = NULL;

	if (self->ops->is_empty(self)) {
		new_node->prev = NULL;
		self->head = new_node;
		self->tail = new_node;
	} else {
		new_node->prev = self->tail;
		self->tail->next = new_node;
		self->tail = new_node;
	}
	self->size += 1;
}

static void list_insert_impl(List *self, size_t index, ElemType value) {
	if (index > self->size) {
		fprintf(stderr, "Index out of bounds\n");
		self->ops->destroy(self);
		exit(EXIT_FAILURE);
	}

	if (index == self->size) {
		self->ops->push(self, value); // 通过 ops 调用 push
		return;
	}

	ListNode *current = list_node_ptr(self, index);
	ListNode *new_node = (ListNode *)malloc(sizeof(ListNode));
	if (new_node == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		self->ops->destroy(self);
		exit(EXIT_FAILURE);
	}
	new_node->data = value;
	new_node->next = current;
	new_node->prev = current->prev;
	current->prev = new_node;
	if (new_node->prev) {
		new_node->prev->next = new_node;
	} else {
		self->head = new_node;
	}
	self->size += 1;
}

static ElemType list_pop_impl(List *self) {
	if (self->ops->is_empty(self)) {
		fprintf(stderr, "List is empty\n");
		self->ops->destroy(self);
		exit(EXIT_FAILURE);
	}
	ListNode *last_node = self->tail;
	self->tail = last_node->prev;
	ElemType pop_value = last_node->data;
	free(last_node);
	if (self->tail) {
		self->tail->next = NULL;
	} else {
		self->head = NULL;
	}
	self->size -= 1;
	return pop_value;
}

static ElemType list_remove_impl(List *self, size_t index) {
	if (index >= self->size) {
		fprintf(stderr, "Index out of bounds\n");
		self->ops->destroy(self);
		exit(EXIT_FAILURE);
	}

	ListNode *current = list_node_ptr(self, index);

	if (current->prev) {
		current->prev->next = current->next;
	} else {
		self->head = current->next;
	}
	if (current->next) {
		current->next->prev = current->prev;
	} else {
		self->tail = current->prev;
	}

	ElemType remove_value = current->data;
	free(current);
	self->size -= 1;
	return remove_value;
}

static size_t list_size_impl(List *self) { return self->size; }

static void list_clear_impl(List *self) {
	ListNode *current = self->head;
	while (current) {
		ListNode *next = current->next;
		free(current);
		current = next;
	}
	self->head = NULL;
	self->tail = NULL;
	self->size = 0;
}

/* ----- 内部的定位函数（不暴露给 ops） ----- */
static ListNode *list_node_ptr(List *self, size_t index) {
	assert(index < self->size);

	ListNode *current;
	if (index < self->size / 2) {
		current = self->head;
		for (size_t i = 0; i < index; ++i) {
			current = current->next;
		}
	} else {
		current = self->tail;
		for (size_t i = self->size - 1; i > index; --i) {
			current = current->prev;
		}
	}
	return current;
}
