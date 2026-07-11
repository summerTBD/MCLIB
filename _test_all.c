#include "Deque.h"
#include "List.h"
#include "String.h"
#include "Vec.h"
#include <stdio.h>

DEFINE_VEC(int)
DEFINE_DEQUE(double)
DEFINE_LIST(char)

int main(void) {
    /* Vec */
    Vec_int v;
    vec_int_init(&v);
    v.ops->push(&v, 42);
    printf("Vec: %d\n", v.ops->get(&v, 0));
    v.ops->destroy(&v);

    /* Deque */
    Deque_double d;
    deque_double_init(&d, 4);
    d.ops->push_back(&d, 3.14);
    printf("Deque: %f\n", d.ops->front(&d));
    d.ops->destroy(&d);

    /* List */
    List_char l;
    list_char_init(&l);
    l.ops->push(&l, 'X');
    printf("List: %c\n", l.ops->get(&l, 0));
    l.ops->destroy(&l);

    /* String */
    String s;
    string_init(&s);
    s.ops->push(&s, "hello X-Macro");
    printf("String: %s\n", s.ops->c_str(&s));
    s.ops->destroy(&s);

    printf("All tests passed!\n");
    return 0;
}
