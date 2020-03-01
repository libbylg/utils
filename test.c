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
    jmp_buf         trap;
};
// clang-format on


static struct test_control_t global_test_control = {{0}, 0, {0}};


_TEST_INITIALIZER(global_test_group_init)
{
    global_test_control.root.next = &(global_test_control.root);
    global_test_control.root.prev = &(global_test_control.root);
    global_test_control.root.parent = NULL;
    global_test_control.root.childs = NULL;
    global_test_control.root.flags = TEST_FLAG_GROUP | TEST_FLAG_DYNAMIC;
    global_test_control.root.name = "";
    global_test_control.root.context = 0;
    global_test_control.root.event = NULL;
    global_test_control.root.proc = NULL;

    global_test_control.current = NULL;
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
        }
        else if ((*wild == *str) || (*wild == '?')) {
            wild++;
            str++;
        }
        else {
            wild = mp;
            str = cp++;
        }
    }

    while (*wild == '*') {
        wild++;
    }
    return (*wild ? -1 : 0);
}


EXPORT_API void test_access(int (*callback)(void* ctx, struct test_t* test), void* ctx)
{
    struct test_t* curr = &(global_test_control.root);
    while (curr) {
        int ret = callback(ctx, curr);
        if (0 != ret) {
            return;
        }

        //  如果有子节点,就优先进入子节点
        if (NULL != curr->childs) {
            curr = curr->childs;
            continue;
        }

        //  如果兄弟节点没有了就回退到父节点
        if (NULL == curr->parent) {
            return;
        }
        if (curr->next == curr->parent->childs) {
            curr = curr->parent;
        }

        //  如果兄弟节点还有,就找到兄弟节点
        if (NULL != curr) {
            curr = curr->next;
        }
    }
}


EXPORT_API int test_register(struct test_t* test, const char* groupName)
{
    struct test_t* tgroup = test_find(groupName);
    if (NULL == tgroup) {
        tgroup = malloc(sizeof(struct test_t));
        //TODO tgroup null-check
        tgroup->next = NULL;
        tgroup->prev = NULL;
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
        test_insert(test, tgroup->childs->prev, tgroup->childs->next);
    }
    else {
        tgroup->childs = test;
    }

    test->parent = tgroup;

    return 0;
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
        return -1; //  we found
    }

    return 0;
}


EXPORT_API struct test_t* test_find(const char* name)
{
    struct test_find_callback_context ctx = {name, NULL};
    test_access(test_find_callback, &ctx);
    return ctx.result;
}


EXPORT_API test_ctx_t test_ctx_get()
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


EXPORT_API test_event_t test_event_get(struct test_t* t)
{
    if (NULL == t) {
        return NULL;
    }

    if (NULL == t->event) {
        return test_event_get(t->parent);
    }

    return t->event;
}


EXPORT_API void test_event_set(struct test_t* t, test_event_t event)
{
    t->event = event;
}


static int test_event_wrap(struct test_t* t, int eventid, void* param)
{
    test_event_t event = test_event_get(t);
    if (NULL == event) {
        return 0;
    }

    int ret = setjmp(global_test_control.trap);
    if (0 != ret) {
        //TODO 遇到异常了;
        return -1;
    }

    (*event)(&(t->context), eventid, param);
    return 0;
}

static int test_proc_wrap(struct test_t* t)
{
    int ret = setjmp(global_test_control.trap);
    if (0 != ret) {
        //TODO 遇到异常了;
        return -1;
    }

    (*(t->proc))();
    return 0;
}

static void test_run_test(struct test_t* t)
{
    global_test_control.current = t;

    int ret = test_event_wrap(t, TEST_EVENT_SETUP, NULL);
    if (0 != ret) {
        //TODO record failed of `test_event_wrap` at `TEST_EVENT_SETUP`
        return;
    }

    ret = test_proc_wrap(t);
    if (0 != ret) {
        //TODO record failed of `test_proc_wrap`
    }

    ret = test_event_wrap(t, TEST_EVENT_TEARDOWN, NULL);
    if (0 != ret) {
        //TODO record failed of `test_event_wrap` at `TEST_EVENT_TEARDOWN`
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

    test_access(test_run_callback, &ctx);
    return 0;
}


#if defined(TEST_AUTO_RUN)
int main(int argc, char* argv[])
{
    TEST_RUN();
    return 0;
}
#endif
