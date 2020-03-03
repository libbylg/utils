#include "str.h"

#define TEST_AUTO_RUN
#include "test.h"


TEST(Test_normal)
{
    EXPECT(0 == wildcmp("*.sh", "test.sh"));
    EXPECT(0 == wildcmp("*.*", "test.sh"));
    EXPECT(0 != wildcmp("*.*", "foobar"));
}


int main(int argc, char* argv[])
{
    TEST_RUN();
    return 0;
}
