# 1 "defer_test.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 360 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "defer_test.c" 2
# 1 "./test.h" 1


# 1 "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/assert.h" 1 3 4
# 42 "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/assert.h" 3 4
# 1 "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/sys/cdefs.h" 1 3 4
# 608 "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/sys/cdefs.h" 3 4
# 1 "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/sys/_symbol_aliasing.h" 1 3 4
# 609 "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/sys/cdefs.h" 2 3 4
# 674 "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/sys/cdefs.h" 3 4
# 1 "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/sys/_posix_availability.h" 1 3 4
# 675 "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/sys/cdefs.h" 2 3 4
# 43 "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/assert.h" 2 3 4
# 76 "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/assert.h" 3 4
void __assert_rtn(const char*, const char*, int, const char*) __attribute__((noreturn))
__attribute__((__disable_tail_calls__));

void __eprintf(const char*, const char*, unsigned, const char*) __attribute__((noreturn));
# 6 "./test.h" 2
# 31 "./test.h"
typedef unsigned long uintptr_t;

_Static_assert(sizeof(uintptr_t) == sizeof(void*), "check the size");


typedef unsigned int uint32_t;

_Static_assert(sizeof(uint32_t) == 4, "check the size");


enum {
    TEST_FLAG_TEST = 0x0001,
    TEST_FLAG_GROUP = 0x0002,
};

struct test_group_t;

typedef uintptr_t test_ctx_t;

typedef uint32_t test_flags_t;

typedef void (*test_event_t)(test_ctx_t* ctx, int event, void* param);

typedef void (*test_proc_t)();

struct test_node_t {
    struct test_node_t* next;
    struct test_node_t* prev;
    struct test_node_t* parent;
    struct test_node_t* childs;
};

struct test_t {
    struct test_node_t node;
    uint32_t flags;
    const char* name;
    test_event_t context;
    test_event_t event;
    test_proc_t proc;
};


extern struct test_t global_test_group;
extern struct test_t* global_test;


extern struct test_t* test_register(struct test_t* test, const char* group);
extern struct test_t* test_find(const char* name);
extern struct test_t* test_flags_set(struct test_t* t, uint32_t flags);
extern uint32_t test_flags_get(struct test_t* t, uint32_t flags);
extern test_ctx_t test_ctx_get();
extern void test_ctx_set(test_ctx_t ctx);
extern test_event_t test_event_get(struct test_t* t);
extern void test_event_set(struct test_t* t, test_event_t event);
extern int test_run(const char* group_pattern, const char* case_pattern);
# 122 "./test.h"
extern char _TEST_FLAGS_TEMPLATE[8];
# 2 "defer_test.c" 2
# 17 "defer_test.c"
static struct test_t hello_group;
static void hello_group_init() __attribute__((constructor))
{
    hello_group.node.next = &(hello_group.node);
    hello_group.node.prev = &(hello_group.node);
    hello_group.node.parent = &(hello_group.node);
    hello_group.node.childs = &(hello_group.node);
    hello_group.flags = TEST_FLAG_GROUP;
    hello_group.name = GROUPSTR;
    hello_group.context = (0);
    hello_group.event = (0);
    hello_group.proc = (0);
    test_register(&(hello_group), "");
};

static void Test_Sample();
static struct test_t Test_Sample_test;
static void Test_Sample_test_init() __attribute__((constructor))
{
    Test_Sample_test.node.next = &(Test_Sample_test);
    Test_Sample_test.node.prev = &(Test_Sample_test);
    Test_Sample_test.node.parent = &(Test_Sample_test);
    Test_Sample_test.node.childs = &(Test_Sample_test);
    Test_Sample_test.flags = TEST_FLAG_TEST;
    Test_Sample_test.name = "Test_Sample";
    Test_Sample_test.context = (0);
    Test_Sample_test.event = (0);
    Test_Sample_test.proc = Test_Sample;
test_register(&(Test_Sample_test), "defer_test.c"));
}
static void Test_Sample()
{
    foo();
    assert_true(counter == 2);
}


int main(int argc, char* argv[])
{
    TEST_RUN();

    return 0;
}
