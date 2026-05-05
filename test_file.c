#include "Deque.h"
#include <stdio.h>
#include <stdlib.h>

DEFINE_DEQUE(int, dq)

int main() {
	deque_dq a;
	deque_dq_init(&a, 1);
	a.ops->push_back(&a, 1);
	printf("%d\n", a.ops->back(&a));
	a.ops->destroy(&a);
	return 0;
}