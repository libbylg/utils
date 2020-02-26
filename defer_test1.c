#include "panic.h"

static void bar()
{
    printf("bar enter\n");
    defer(defer_bar, { printf("bar defer\n"); });
    printf("bar leave\n");
}

static void foo()
{
    printf("foo enter\n");
    defer(defer_bar, { printf("foo defer\n"); });
    bar();
    printf("foo leave\n");
}

int main(int argc, char* argv[])
{
    int ret = 0;
    if (0 == ret) {
        printf("before foo\n");
        foo();
        printf("after  foo\n");
    } else {
        printf("catch  jump here\n");
    }

    return 0;
}
