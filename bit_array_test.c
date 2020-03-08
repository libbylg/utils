#include "bit_array.h"

#include "macros.h"
#include "test.h"

TEST(Test_t1)
{
    ASSERT(sizeof(struct bit_array_t) + 0 == bit_array_memsize(0));
    ASSERT(sizeof(struct bit_array_t) + 8 == bit_array_memsize(1));
    ASSERT(sizeof(struct bit_array_t) + 8 == bit_array_memsize(8));
    ASSERT(sizeof(struct bit_array_t) + 128 == bit_array_memsize(123));
}


TEST(Test_t2)
{
    struct bit_array_t* array = bit_array_alloca(123);
    ASSERT(array->len == 128 / 64);
}
