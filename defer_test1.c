#include "panic.h"


static void bar()
{
    printf("bar enter\n");
    defer({ printf("bar defer\n"); });
    printf("bar leave\n");
}

static void foo()
{
    printf("foo enter\n");
    defer({ printf("foo defer\n"); });
    bar();
    printf("foo leave\n");
}

extern int test1()
{
    int ret = 0;
    if (0 == ret) {
        printf("before foo\n");
        foo();
        printf("after  foo\n");
    }
    else {
        printf("catch  jump here\n");
    }

    return 0;
}
