#include "test.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// clang-format off
struct test_control_t {
    struct test_t   root;
    struct test_t*  current;
    int             action;
    test_output_t   output;
    jmp_buf         trap;
};
// clang-format on


static const char* global_test_type_names[] = {
    "Testing pass",              //   TEST_MESSAGE_TESTING_PASS
    "Testing fail",              //   TEST_MESSAGE_TESTING_FAIL
    "Testing pass with warning", //   TEST_MESSAGE_TESTING_WARN
    "Setup failed",              //   TEST_MESSAGE_SETUP_FAIL
    "Teardown failed",           //   TEST_MESSAGE_TEARDOWN_FAIL
    "Assert failed",             //   TEST_MESSAGE_ASSERT_FAIL
    "Expect failed",             //   TEST_MESSAGE_EXPECT_FAIL
    "Catch fault",               //   TEST_MESSAGE_CATCH_FAULT
    "Memory leak",               //   TEST_MESSAGE_MEMORY_LEAK
    "Internal error",            //   TEST_MESSAGE_INTERNAL_ERROR
};

static struct test_control_t global_test_control = {{0}};


static void test_output_default(struct test_message_t* message);
_TEST_INITIALIZER(global_test_group_init)
{
    test_init(&(global_test_control.root), "", NULL, TEST_FLAG_GROUP);
    global_test_control.current = NULL;
    global_test_control.output = test_output_default;
}

static inline struct test_t* test_insert(struct test_t* n, struct test_t* prev, struct test_t* next)
{
    n->prev = prev;
    n->next = next;
    prev->next = n;
    next->prev = n;
    return n;
}


static int test_wildcmp(const char* wild, const char* str)
{
    // Written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
    const char* cp = NULL;
    const char* mp = NULL;

    while ((*str) && (*wild != '*')) {
        if ((*wild != *str) && (*wild != '?')) {
            return -1;
        }
        wild++;
        str++;
    }

    while (*str) {
        if (*wild == '*') {
            if (!*++wild) {
                return 0;
            }
            mp = wild;
            cp = str + 1;
        } else if ((*wild == *str) || (*wild == '?')) {
            wild++;
            str++;
        } else {
            wild = mp;
            str = cp++;
        }
    }

    while (*wild == '*') {
        wild++;
    }
    return (*wild ? -1 : 0);
}


EXPORT_API struct test_t* test_init(struct test_t* t, const char* name, test_proc_t proc, uint32_t flags)
{
    t->next = t;
    t->prev = t;
    t->parent = NULL;
    t->childs = NULL;
    t->result = NULL;
    t->flags = flags;
    t->name = name;
    t->context = 0;
    t->event = NULL;
    t->proc = proc;

    return t;
}


EXPORT_API void test_accessX(int (*callback)(void* ctx, struct test_t* test), void* ctx)
{
    struct test_t* curr = &(global_test_control.root);

    while (curr != NULL) {
        int ret = callback(ctx, curr);
        if (0 != ret) {
            return;
        }

        //  如果有子节点,就优先进入子节点
        if (NULL != curr->childs) {
            curr = curr->childs;
            continue;
        }


        //! 如果当前层级没有下一个节点了,那么就回到上层继续看上层是否有下一个节点
        while (curr && (NULL == curr->next)) {
            curr = curr->parent;
        }

        //! 已经回溯到最顶层了
        if (NULL == curr) {
            break;
        }

        //! 回溯之后继续找下一个兄弟节点
        curr = curr->next;
    }
}


EXPORT_API void test_access(int (*callback)(void* ctx, struct test_t* test), void* ctx)
{
    struct test_t* curr = &(global_test_control.root);
    struct test_t* endt = curr;

    while (1) {
        //        printf("//////%s\n", curr->name);
        int ret = callback(ctx, curr);
        if (0 != ret) {
            return;
        }

        //  如果有子节点,就优先进入子节点
        if (NULL != curr->childs) {
            curr = curr->childs;
            endt = curr->prev;
            continue;
        }

        // 如果当前层级没有下一个节点了,那么就回到上层继续看上层是否有下一个节点
        while (endt == curr) {
            //  回到父节点
            curr = curr->parent;

            // 如果已经回溯到最顶层了(1)
            if (NULL == curr) {
                return;
            }

            // 如果已经回溯到最顶层了(2)
            if (NULL == curr->parent) {
                return;
            }

            // 重新确定结束位置
            endt = curr->parent->childs->prev;
        }

        //! 继续找下一个兄弟节点
        curr = curr->next;
    }
}


EXPORT_API int test_register(struct test_t* test, const char* groupName)
{
    struct test_t* tgroup = test_find(groupName);
    if (NULL == tgroup) {
        tgroup = malloc(sizeof(struct test_t));
        // TODO tgroup null-check
        tgroup->next = tgroup;
        tgroup->prev = tgroup;
        tgroup->parent = NULL;
        tgroup->childs = NULL;
        tgroup->flags = TEST_FLAG_GROUP | TEST_FLAG_DYNAMIC;
        tgroup->name = groupName;
        tgroup->context = 0;
        tgroup->event = NULL;
        tgroup->proc = NULL;
        test_register(tgroup, "");
    }

    if (NULL != tgroup->childs) {
        test_insert(test, tgroup->childs->prev, tgroup->childs);
    } else {
        tgroup->childs = test;
    }

    test->parent = tgroup;

    return 0;
}


EXPORT_API struct test_t* test_self()
{
    return global_test_control.current;
}


struct test_find_callback_context {
    const char* name;
    struct test_t* result;
};

static int test_find_callback(void* c, struct test_t* t)
{
    struct test_find_callback_context* ctx = (struct test_find_callback_context*)c;
    if (0 == strcmp(ctx->name, t->name)) {
        ctx->result = t;
        return -1;  //  we found
    }

    return 0;
}


EXPORT_API struct test_t* test_find(const char* name)
{
    //    printf("test_find: [%s]\n", name);
    struct test_find_callback_context ctx = {name, NULL};
    test_access(test_find_callback, &ctx);
    return ctx.result;
}


EXPORT_API test_ctx_t test_ctx()
{
    if (0 != global_test_control.current->context) {
        return global_test_control.current->context;
    }
    return global_test_control.current->parent->context;
}


EXPORT_API void test_ctx_set(test_ctx_t ctx)
{
    global_test_control.current->context = ctx;
}


EXPORT_API enum TEST_ACTION test_action()
{
    return global_test_control.action;
}


static test_event_t test_event_get(struct test_t* t)
{
    if (NULL != t->event) {
        return t->event;
    }

    if (NULL != t->parent) {
        return t->parent->event;
    }

    return NULL;
}

EXPORT_API void test_event_set(struct test_t* t, test_event_t event)
{
    t->event = event;
}


static int test_event_wrap(struct test_t* t, int action)
{
    test_event_t event = test_event_get(t);
    if (NULL == event) {
        return 0;
    }

    global_test_control.action = action;

    int ret = setjmp(global_test_control.trap);
    if (0 != ret) {
        return -1;
    }

    (*event)();
    return 0;
}


static int test_proc_wrap(struct test_t* t)
{
    global_test_control.action = TEST_ACTION_TESTING;

    int ret = setjmp(global_test_control.trap);
    if (0 != ret) {
        test_message("", -1, TEST_MESSAGE_TESTING_FAIL, "", "");
        return -1;
    }

    (*(t->proc))();
    return 0;
}


static void test_run_test(struct test_t* t)
{
    global_test_control.current = t;

    int setup_result = test_event_wrap(t, TEST_ACTION_SETUP);
    if (0 != setup_result) {
        test_message("", -1, TEST_MESSAGE_SETUP_FAIL, "", "");
        return;
    }

    int proc_result = test_proc_wrap(t);
    if (0 != proc_result) {
        test_message("", -1, TEST_MESSAGE_TEARDOWN_FAIL, "", "");
    }

    int teardown_result = test_event_wrap(t, TEST_ACTION_TEARDOWN);
    if (0 != teardown_result) {
        test_message("", -1, TEST_MESSAGE_TEARDOWN_FAIL, "", "");
        return;
    }

    if ((0 == setup_result) && (0 == proc_result) && (0 == teardown_result)) {
        test_message("", -1, TEST_MESSAGE_TESTING_PASS, "", "");
    }
}

struct test_run_callback_context {
    const char* group_pattern;
    const char* test_pattern;
};

static int test_run_callback(void* c, struct test_t* t)
{
    struct test_run_callback_context* ctx = (struct test_run_callback_context*)c;
    if ((0 == (t->flags & TEST_FLAG_GROUP)) && (0 == test_wildcmp(ctx->group_pattern, t->parent->name))) {
        if (0 == test_wildcmp(ctx->group_pattern, t->name)) {
            //            printf("running: %s\n", t->name);
            test_run_test(t);
            return 0;
        }
    }

    return 0;
}


EXPORT_API int test_run(const char* group_pattern, const char* test_pattern)
{
    struct test_run_callback_context ctx = {
        group_pattern,
        test_pattern,
    };

    //    printf("test_run: [%s/%s]\n", group_pattern, test_pattern);
    test_access(test_run_callback, &ctx);
    return 0;
}


static void test_output_default(struct test_message_t* message)
{
    printf("[%s]\n%s(%d): [%s] %s\n", message->test->name, message->file, message->line,
           global_test_type_names[message->type], message->text);
}


EXPORT_API void test_messagev(const char* file, int line, int type, const char* msg, const char* format, void* va)
{
    char desc[1024] = {0};
    int len = vsnprintf(desc, sizeof(desc), format, va);
    if (len <= 0) {
        desc[0] = '\0';
        len = 0;
    }

    struct test_message_t* message = (struct test_message_t*)malloc(sizeof(struct test_message_t) + (len + 1));
    message->next = message;
    message->prev = message;
    message->test = global_test_control.current;
    message->file = file;
    message->message = msg;
    message->line = line;
    message->type = type;
    message->len = len;
    memcpy(message->text, desc, len + 1);

    global_test_control.output(message);
}


EXPORT_API void test_message(const char* file, int line, int type, const char* msg, const char* format, ...)
{
    va_list va;
    va_start(va, format);
    test_messagev(file, line, type, msg, format, va);
    va_end(va);
}


EXPORT_API void test_raisev(const char* file, int line, int type, const char* msg, const char* format, void* va)
{
    //! Save the test result
    test_messagev(file, line, type, msg, format, va);

    //! Jump to the trap to run continue
    longjmp(global_test_control.trap, 1);
}


EXPORT_API void test_raise(const char* file, int line, int type, const char* msg, const char* format, ...)
{
    va_list va;
    va_start(va, format);
    test_raisev(file, line, type, msg, format, va);
    va_end(va);
}


#if defined(TEST_AUTO_RUN)
int main(int argc, char* argv[])
{
    TEST_RUN();
    return 0;
}
#endif
