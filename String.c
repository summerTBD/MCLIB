#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct String;
typedef struct StringOps;

typedef struct String {
	char *data;
	size_t size; // 不包括结尾 '\0' 的长度
	size_t capacity;
	const struct StringOps *ops;
} String;

struct StringOps {
	void (*init)(String *self);
	void (*destroy)(String *self);
	void (*push)(String *self, const char *str);
	void (*clear)(String *self);
	size_t (*size)(String *self);
	size_t (*capacity)(String *self);
	const char *(*c_str)(String *self);
	void (*assign)(String *self, const char *str);
	char *(*front)(String *self);
	char *(*back)(String *self);
};

static void string_init(String *self);
static void string_destroy(String *self);
static void string_push(String *self, const char *str);
static void string_clear(String *self);
static size_t string_size(String *self);
static size_t string_capacity(String *self);
static const char *string_c_str(String *self);
static void string_assign(String *self, const char *str);
static char *string_front(String *self);
static char *string_back(String *self);

static const struct StringOps string_default_ops = {.init = string_init,
													.destroy = string_destroy,
													.push = string_push,
													.clear = string_clear,
													.size = string_size,
													.capacity = string_capacity,
													.c_str = string_c_str,
													.assign = string_assign,
													.front = string_front,
													.back = string_back};

static void string_init(String *self) {
	self->data = NULL;
	self->size = 0;
	self->capacity = 0;
	self->ops = &string_default_ops;
}

static void string_destroy(String *self) {
	free(self->data);
	self->data = NULL;
	self->size = 0;
	self->capacity = 0;
}

static void string_push(String *self, const char *str) {
	assert(str != NULL);

	size_t str_len = strlen(str);

	// 防止溢出
	if (str_len > SIZE_MAX - self->size) {
		fprintf(stderr, "String size overflow\n");
		string_destroy(self);
		exit(EXIT_FAILURE);
	}

	size_t new_size = self->size + str_len;
	size_t needed = new_size + 1; // 包括结尾 '\0'

	if (needed > self->capacity) {
		size_t new_capacity = self->capacity == 0 ? 16 : self->capacity * 2;
		while (new_capacity < needed) {
			new_capacity *= 2;
		}
		char *new_data = realloc(self->data, new_capacity);
		if (new_data == NULL) {
			fprintf(stderr, "Failed to allocate memory\n");
			string_destroy(self);
			exit(EXIT_FAILURE);
		}
		self->data = new_data;
		self->capacity = new_capacity;
	}

	memcpy(self->data + self->size, str, str_len);
	self->size = new_size;
	self->data[self->size] = '\0';
}

static void string_clear(String *self) {
	self->size = 0;
	if (self->data) {
		self->data[0] = '\0';
	}
}

static size_t string_size(String *self) { return self->size; }

static size_t string_capacity(String *self) { return self->capacity; }

static const char *string_c_str(String *self) { return self->data; }

static void string_assign(String *self, const char *str) {
	string_clear(self);
	string_push(self, str);
}

static char *string_front(String *self) { return self->data; }

static char *string_back(String *self) { return self->data + self->size - 1; }