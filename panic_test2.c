#include "panic.h"


static void bar()
{
    printf("bar enter\n");
    defer(bar_defer, { printf("bar defer\n"); });
    panic("123");
    printf("bar leave\n");
}

static void foo()
{
    printf("foo enter\n");
    defer(foo_defer, { printf("foo defer\n"); });
    bar();
    printf("foo leave\n");
}

extern int test2()
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
