#ifndef LIST_H
#define LIST_H

#include <assert.h>
#include <stdlib.h>

/**
 * 宏 DEFINE_LIST(TYPE, NAME) —— 生成针对 TYPE 类型的双向链表
 *
 * 队列类型名   : List_##NAME
 * 操作表类型   : ListOps_##NAME
 * 操作表实例   : list_##NAME##_ops
 *
 * 使用方法：
 *   1. 包含此头文件
 *   2. 在需要的源文件中调用一次 DEFINE_LIST(int, int) 等
 *   3. 声明 List_int lst; 并使用
 *
 * 所有函数为 static，可安全放在头文件中多文件包含。
 */
#define DEFINE_LIST(TYPE, NAME)                                                \
	/* ---------- 节点结构 ---------- */                                       \
	typedef struct ListNode_##NAME {                                           \
		TYPE data;                                                             \
		struct ListNode_##NAME *prev;                                          \
		struct ListNode_##NAME *next;                                          \
	} ListNode_##NAME;                                                         \
                                                                               \
	/* 前向声明 */                                                             \
	typedef struct List_##NAME List_##NAME;                                    \
	typedef struct ListOps_##NAME ListOps_##NAME;                              \
                                                                               \
	/* ---------- 操作表结构 ---------- */                                     \
	struct ListOps_##NAME {                                                    \
		void (*init)(List_##NAME * self);                                      \
		void (*destroy)(List_##NAME * self);                                   \
		int (*empty)(List_##NAME * self);                                      \
		TYPE (*get)(List_##NAME * self, size_t index);                         \
		void (*set)(List_##NAME * self, size_t index, TYPE value);             \
		void (*push)(List_##NAME * self, TYPE value);                          \
		void (*insert)(List_##NAME * self, size_t index, TYPE value);          \
		TYPE (*pop)(List_##NAME * self);                                       \
		TYPE (*remove_at)(List_##NAME * self, size_t index);                   \
		size_t (*size)(List_##NAME * self);                                    \
		void (*clear)(List_##NAME * self);                                     \
	};                                                                         \
                                                                               \
	/* ---------- 链表主结构 ---------- */                                     \
	struct List_##NAME {                                                       \
		ListNode_##NAME *head;                                                 \
		ListNode_##NAME *tail;                                                 \
		size_t count;                                                          \
		const ListOps_##NAME *ops;                                             \
	};                                                                         \
                                                                               \
	/* ---------- 内部定位辅助函数 ---------- */                               \
	static ListNode_##NAME *list_##NAME##_node_ptr(List_##NAME *self,          \
												   size_t index) {             \
		assert(index < self->count);                                           \
		ListNode_##NAME *cur;                                                  \
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
	/* ---------- 实现函数 ---------- */                                       \
	static void list_##NAME##_init(List_##NAME *self) {                        \
		self->head = NULL;                                                     \
		self->tail = NULL;                                                     \
		self->count = 0;                                                       \
		self->ops = &list_##NAME##_ops;                                        \
	}                                                                          \
                                                                               \
	static void list_##NAME##_destroy(List_##NAME *self) {                     \
		ListNode_##NAME *cur = self->head;                                     \
		while (cur) {                                                          \
			ListNode_##NAME *next = cur->next;                                 \
			free(cur);                                                         \
			cur = next;                                                        \
		}                                                                      \
		self->head = NULL;                                                     \
		self->tail = NULL;                                                     \
		self->count = 0;                                                       \
	}                                                                          \
                                                                               \
	static int list_##NAME##_empty(List_##NAME *self) {                        \
		return self->count == 0;                                               \
	}                                                                          \
                                                                               \
	static TYPE list_##NAME##_get(List_##NAME *self, size_t index) {           \
		assert(index < self->count);                                           \
		ListNode_##NAME *cur = list_##NAME##_node_ptr(self, index);            \
		return cur->data;                                                      \
	}                                                                          \
                                                                               \
	static void list_##NAME##_set(List_##NAME *self, size_t index, TYPE val) { \
		assert(index < self->count);                                           \
		ListNode_##NAME *cur = list_##NAME##_node_ptr(self, index);            \
		cur->data = val;                                                       \
	}                                                                          \
                                                                               \
	static void list_##NAME##_push(List_##NAME *self, TYPE val) {              \
		ListNode_##NAME *node =                                                \
			(ListNode_##NAME *)malloc(sizeof(ListNode_##NAME));                \
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
	static void list_##NAME##_insert(List_##NAME *self, size_t index,          \
									 TYPE val) {                               \
		assert(index <= self->count);                                          \
		if (index == self->count) {                                            \
			list_##NAME##_push(self, val);                                     \
			return;                                                            \
		}                                                                      \
		ListNode_##NAME *cur = list_##NAME##_node_ptr(self, index);            \
		ListNode_##NAME *node =                                                \
			(ListNode_##NAME *)malloc(sizeof(ListNode_##NAME));                \
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
	static TYPE list_##NAME##_pop(List_##NAME *self) {                         \
		assert(self->count > 0);                                               \
		ListNode_##NAME *last = self->tail;                                    \
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
	static TYPE list_##NAME##_remove_at(List_##NAME *self, size_t index) {     \
		assert(index < self->count);                                           \
		ListNode_##NAME *cur = list_##NAME##_node_ptr(self, index);            \
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
	static size_t list_##NAME##_size(List_##NAME *self) {                      \
		return self->count;                                                    \
	}                                                                          \
                                                                               \
	static void list_##NAME##_clear(List_##NAME *self) {                       \
		ListNode_##NAME *cur = self->head;                                     \
		while (cur) {                                                          \
			ListNode_##NAME *next = cur->next;                                 \
			free(cur);                                                         \
			cur = next;                                                        \
		}                                                                      \
		self->head = NULL;                                                     \
		self->tail = NULL;                                                     \
		self->count = 0;                                                       \
	}                                                                          \
                                                                               \
	/* ---------- 默认操作表实例 ---------- */                                 \
	static const ListOps_##NAME list_##NAME##_ops = {                          \
		.init = list_##NAME##_init,                                            \
		.destroy = list_##NAME##_destroy,                                      \
		.empty = list_##NAME##_empty,                                          \
		.get = list_##NAME##_get,                                              \
		.set = list_##NAME##_set,                                              \
		.push = list_##NAME##_push,                                            \
		.insert = list_##NAME##_insert,                                        \
		.pop = list_##NAME##_pop,                                              \
		.remove_at = list_##NAME##_remove_at,                                  \
		.size = list_##NAME##_size,                                            \
		.clear = list_##NAME##_clear}

#endif /* LIST_H */