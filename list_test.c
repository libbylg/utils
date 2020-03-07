#include "test.h"

#include <stdio.h>

void Test_proc()
{
}

struct test_t g1;
struct test_t g2;
struct test_t g3;
struct test_t g4;
struct test_t g5;
struct test_t g6;

struct test_t t1;
struct test_t t2;
struct test_t t3;
struct test_t t4;
struct test_t t5;
struct test_t t6;
struct test_t t7;
struct test_t t8;

int order;


int callback_Test_list_init(void* ctx, struct test_t* test)
{
    printf("[%s]\n", test->name);
    return 0;
}


void Test_test_register()
{
    //  Biuld the structure like below:
    //  root("")
    //      g1
    //          t1
    //          t2
    //      g2
    //          g6
    //      g3
    //          t3
    //          g4
    //              t4
    //              t5
    //              t6
    //          g5
    //              t7
    //              t8
    test_register(test_init(&g1, "g1", NULL, TEST_FLAG_GROUP), "");
    test_register(test_init(&t1, "t1", Test_proc, 0), "g1");
    test_register(test_init(&t2, "t2", Test_proc, 0), "g1");

    test_register(test_init(&g2, "g2", NULL, TEST_FLAG_GROUP), "");
    test_register(test_init(&g6, "g6", NULL, TEST_FLAG_GROUP), "g2");

    test_register(test_init(&g3, "g3", NULL, TEST_FLAG_GROUP), "");
    test_register(test_init(&t3, "t3", Test_proc, 0), "g3");
    test_register(test_init(&g4, "g4", NULL, TEST_FLAG_GROUP), "g3");
    test_register(test_init(&t4, "t4", Test_proc, 0), "g4");
    test_register(test_init(&t5, "t5", Test_proc, 0), "g4");
    test_register(test_init(&t6, "t6", Test_proc, 0), "g4");
    test_register(test_init(&g5, "g5", NULL, TEST_FLAG_GROUP), "g3");
    test_register(test_init(&t7, "t7", Test_proc, 0), "g5");
    test_register(test_init(&t8, "t8", Test_proc, 0), "g5");

    //(void* ctx, struct test_t* test), void* ctx
    test_access(callback_Test_list_init, NULL);
}


int main(int argc, char* atgv[])
{
    Test_test_register();

    return 0;
}
