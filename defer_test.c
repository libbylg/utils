#include "panic.h"
#include "test.h"

static int counter = 0;

static void bar()
{
    defer(defer_bar, { counter++; });
}

static void foo()
{
    defer(defer_foo, { counter++; });
    bar();
}

TEST_GROUP(hello2, "a.b.c");


TEST(hello2, Test_Sample2)
{
    foo();
    ASSERT(counter == 2);
}

TEST(Test_Sample)
{
    foo();
    ASSERT(counter == 2);
}
