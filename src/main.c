#include "mystring.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    char ss[100];
    scanf("%s", ss);
    // 新建一个mystring
    // 别忘了销毁
    mystring* ms = mystring_from_cstr(ss);
    printf("%s", mystring_cstr(ms));
    MYSTRING_DROP(ms);
    return 0;
}