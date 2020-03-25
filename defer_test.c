#include "panic.h"
#include "test.h"

static int counter = 0;

static void bar()
{
    defer(defer_bar, {
        printf("defer_bar\n");
        counter++;
    });
}

static void foo()
{
    defer(defer_foo, {
        printf("defer_foo\n");
        counter++;
    });
    bar();
}

TEST_GROUP(hello2, "a.b.c");

TEST_EVENT(hello2, Event_Sample)
{
    enum TEST_ACTION a = test_action();
    if (a == TEST_ACTION_SETUP) {
        counter = 0;
    }
}

TEST(hello2, Test_Sample2)
{
    printf("begin Test_Sample2\n");
    foo();
    ASSERT(counter == 2);
}

TEST(Test_Sample)
{
    printf("begin Test_Sample\n");
    foo();
    ASSERT(counter == 2);
}
