#include "str.h"

#define TEST_AUTO_RUN
#include "test.h"


TEST(Test_star)
{
    ASSERT(0 == wildcmp("*.sh", "test.sh"));
    ASSERT(0 == wildcmp("*.*", "test.sh"));
    ASSERT(0 == wildcmp("*", "foobar"));
    ASSERT(0 == wildcmp("*", ""));
    ASSERT(0 == wildcmp("*b", "foob"));
    ASSERT(0 == wildcmp("*b", "b"));
    ASSERT(0 == wildcmp("*b", "bbbb"));
    ASSERT(0 == wildcmp("abcd", "abcd"));
    ASSERT(0 == wildcmp("", ""));
    ASSERT(0 != wildcmp("*.*", "foobar"));
    ASSERT(0 != wildcmp("*b", "foobar"));
    ASSERT(0 != wildcmp("*b", "foobar"));
    ASSERT(0 != wildcmp("", "123"));
}


TEST(Test_support)
{
    ASSERT(0 == wildcmp("?", "a"));
    ASSERT(0 == wildcmp("??", "ab"));
    ASSERT(0 != wildcmp("??", "abc"));
    ASSERT(0 != wildcmp("??", "a"));
    ASSERT(0 != wildcmp("?", ""));
    ASSERT(0 != wildcmp("?", "aa"));
}


TEST(Test_mixin)
{
    ASSERT(0 == wildcmp("?*", "a"));
    ASSERT(0 == wildcmp("?*", "abc"));
    ASSERT(0 != wildcmp("a*?", "a"));
    ASSERT(0 == wildcmp("a*?", "ab"));
    ASSERT(0 == wildcmp("a*?", "abc"));
    ASSERT(0 == wildcmp("?a*b", "mab"));
    ASSERT(0 == wildcmp("?a*b", "maxxxxb"));
    ASSERT(0 != wildcmp("?a*b", "axxxxb"));
}

int main(int argc, char* argv[])
{
    TEST_RUN();
    return 0;
}
