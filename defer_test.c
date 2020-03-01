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

TEST_GROUP(hello);

TEST(Test_Sample2, hello)
{
    foo();
    assert_true(counter == 2);
}

TEST(Test_Sample)
{
    foo();
    assert_true(counter == 2);
}


int main(int argc, char* argv[])
{
    TEST_RUN();

    return 0;
}
