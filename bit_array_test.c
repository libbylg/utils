#include "bit_array.h"

#include "macros.h"
#include "test.h"

TEST(Test_array1)
{
    ASSERT(sizeof(struct bit_array_t) + 0 == bit_array_bytesof(0));
    ASSERT(sizeof(struct bit_array_t) + 8 == bit_array_bytesof(1));
    ASSERT(sizeof(struct bit_array_t) + 8 == bit_array_bytesof(8));
    ASSERT(sizeof(struct bit_array_t) + (128 / 8) == bit_array_bytesof(123));
}


TEST(Test_array2)
{
    struct bit_array_t* array = bit_array_alloca(123);
    ASSERT(array->len == 128 / 64);

    bit_array_reset(array);

    bit_array_set(array, 0);
    bit_array_set(array, 1);
    bit_array_set(array, 122);
    bit_array_set(array, 123);
    bit_array_set(array, 127);

    ASSERT(5 == bit_array_count_of(array, 1));
}
