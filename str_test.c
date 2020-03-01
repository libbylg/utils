#include "str.h"


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
