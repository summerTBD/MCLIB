#include "Deque.h"
#include "List.h"
#include "Rc.h"
#include "String.h"
#include "Vec.h"
#include <stdio.h>

DEFINE_VEC(int)
DEFINE_DEQUE(double)
DEFINE_LIST(char)
DEFINE_RC(int)

int main(void) {
    /* Vec */
    Vec_int v;
    vec_int_init(&v);
    vec_int_push(&v, 42);
    printf("Vec: %d\n", vec_int_get(&v, 0));
    vec_int_destroy(&v);

    /* Deque */
    Deque_double d;
    deque_double_init(&d, 4);
    deque_double_push_back(&d, 3.14);
    printf("Deque: %f\n", deque_double_front(&d));
    deque_double_destroy(&d);

    /* List */
    List_char l;
    list_char_init(&l);
    list_char_push(&l, 'X');
    printf("List: %c\n", list_char_get(&l, 0));
    list_char_destroy(&l);

    /* Rc */
    Rc_int r1;
    rc_int_init(&r1, 100);
    Rc_int r2 = rc_int_clone(&r1); // refcount = 2

    int* p1 = rc_int_deref(&r1); // 返回指针, 可读可写
    int* p2 = rc_int_deref(&r2); // 指向同一块内存
    printf("Rc: r1=%d r2=%d\n", *p1, *p2);
    *p1 = 999;                             // 通过 Rc 修改原值!
    printf("Rc: r1=%d r2=%d\n", *p1, *p2); // 两个都变成 999

    rc_int_drop(&r1); // refcount = 1
    rc_int_drop(&r2); // refcount = 0, 释放

    /* Rc init_move —— 指针传参，避免多余拷贝 */
    int val = 77;
    Rc_int r3;
    rc_int_init_move(&r3, &val);
    printf("Rc init_move: %d\n", *rc_int_deref(&r3));
    rc_int_drop(&r3);

    /* String */
    String s;
    string_init(&s);
    string_push(&s, "hello X-Macro");
    printf("String: %s\n", string_c_str(&s));
    string_destroy(&s);

    printf("All tests passed!\n");
    return 0;
}
