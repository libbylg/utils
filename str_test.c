#include "str.h"

#define TEST_AUTO_RUN
#include "test.h"


TEST(Test_normal)
{
    ASSERT(1 == 2);
}


int main(int argc, char* argv[])
{
    TEST_RUN();
    return 0;
}
