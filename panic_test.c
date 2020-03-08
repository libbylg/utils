#include "panic.h"
#include "test.h"

static int bar_counter = 0;
static int foo_counter = 0;
static void bar()
{
    defer(bar_defer, { bar_counter++; });
    panic("123");
}

static void foo()
{
    defer(bar_defer, { foo_counter++; });
    defer(foo_defer, { foo_counter++; });
    bar();
}


TEST(Test_defer_1)
{
    foo();
    ASSERT(1 == bar_counter);
    ASSERT(2 == foo_counter);
}
