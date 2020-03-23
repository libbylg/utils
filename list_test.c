#include "test.h"

#include "list.h"

TEST(Test_list1)
{
    struct list_head list;
    list_init(&list);
    ASSERT(list.prev == &list);
    ASSERT(list.next == &list);
}
