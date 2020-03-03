#ifndef test_H
#define test_H


//#include <assert.h>


//  `STATIC_ASSERT` is the compile time assertion.
#ifndef STATIC_ASSERT
#if defined(__cplusplus)
#define STATIC_ASSERT(const_expr, ...) _Static_assert(const_expr, __VA_ARGS__);
#else
#define STATIC_ASSERT(const_expr, ...) _Static_assert(const_expr, __VA_ARGS__);
#endif  //__cplusplus
#endif  // STATIC_ASSERT


//! `EXPORT_API` is used to export the symbols.
#ifndef EXPORT_API
#if defined(__cplusplus)
#define EXPORT_API extern "C"
#else
#define EXPORT_API extern
#endif  //__cplusplus
#endif  // EXPORT_API


//! `NULL` is a definition for null-value.
#ifndef NULL
#ifndef __cplusplus
#define NULL ((void*)0)
#else
#define NULL nullptr
#endif  //__cplusplus
#endif  // NULL


//! `uintptr_t` is a type who's size allway same with pointer.
#if !defined(__intptr_t_defined)
typedef unsigned long uintptr_t;  //  TODO need to check 32bit/64bit
#define __intptr_t_defined
STATIC_ASSERT(sizeof(uintptr_t) == sizeof(void*), "check the size")
#endif  //__intptr_t_defined


//! `uint32_t` is a type who's size is 4-bytes on different platform.
#if !defined(__uint32_t_defined)
typedef unsigned int uint32_t;  //  TODO need to check 32bit/64bit
#define __uint32_t_defined
STATIC_ASSERT(sizeof(uint32_t) == 4, "check the size")
#endif  //__uint32_t_defined


//! `uint32_t` is a type who's size is 4-bytes on different platform.
#if !defined(__uint16_t_defined)
typedef unsigned short uint16_t;  //  TODO need to check 32bit/64bit
#define __uint16_t_defined
STATIC_ASSERT(sizeof(uint16_t) == 2, "check the size")
#endif  //__uint16_t_defined

//! Some flags for `test_t`.
enum TEST_FLAGS {
    TEST_FLAG_GROUP = 0x0001,
    TEST_FLAG_DYNAMIC = 0x8000,
};


enum TEST_MESSAGE_TYPE {
    TYPE_PASS = 0,
    TYPE_SETUP_FAIL = 1,
    TYPE_TEARDOWN_FAIL = 2,
    TYPE_ASSERT_FAIL = 3,
    TYPE_EXPECT_FAIL = 4,
    TYPE_CATCH_FAULT = 5,
    TYPE_MEMORY_LEAK = 6,

    TYPE_INTERNAL_ERROR,
};


enum {
    TEST_EVENT_SETUP = 0,
    TEST_EVENT_TEARDOWN = 1,
};


typedef uintptr_t test_ctx_t;

typedef uint32_t test_flags_t;

typedef void (*test_event_t)(test_ctx_t* ctx, int event, void* param);

typedef void (*test_proc_t)();

// clang-format off
struct test_t {
    struct test_t*  next;
    struct test_t*  prev;
    struct test_t*  parent;
    struct test_t*  childs;
    uint32_t        flags;
    const char*     name;
    test_ctx_t      context;
    test_event_t    event;
    test_proc_t     proc;
};
// clang-format on

// clang-format off
struct test_message_t {
    struct test_message_t*  next;
    struct test_message_t*  prev;
    struct test_t*          test;
    const char*             message;
    const char*             file;
    int                     line;
    uint16_t                type;
    uint16_t                len;
    char                    text[0];
};
// clang-format on

typedef void (*test_output_t)(struct test_message_t* message);


//----------------------------------------------------------------------------------------------------------------------
// clang-format off
//! `test_register` 函数用于将 `test` 对象 注册到名称为 `group` 的组下面.
//! \param `test` 可以为一个"测试",也可以为一个"测试组". 
//! \param `group` 如果指定为"", 表示注册到全局的测试组下面
EXPORT_API int              test_register(struct test_t* t, const char* group);
EXPORT_API struct test_t*   test_find(const char* name);
EXPORT_API struct test_t*   test_self();
EXPORT_API test_ctx_t       test_ctx_get();
EXPORT_API void             test_ctx_set(test_ctx_t ctx);
EXPORT_API test_event_t     test_event_get(struct test_t* t);
EXPORT_API void             test_event_set(struct test_t* t, test_event_t event);
EXPORT_API void             test_messagev(const char* file, int line, int type, const char* msg, const char* format, void* va);
EXPORT_API void             test_message(const char* file, int line, int type, const char* msg, const char* format, ...);
EXPORT_API void             test_raisev(const char* file, int line, int type, const char* msg, const char* format, void* va);
EXPORT_API void             test_raise(const char* file, int line, int type, const char* msg, const char* format, ...);
EXPORT_API void             test_access(int (*callback)(void* ctx, struct test_t* test), void* ctx);
EXPORT_API int              test_run(const char* group_pattern, const char* case_pattern);
// clang-format on


//----------------------------------------------------------------------------------------------------------------------
#ifdef _MSC_VER
#define _TEST_INITIALIZER(f)                                                                \
    static void f();                                                                        \
    static int __##f##_wrapper()                                                            \
    {                                                                                       \
        f();                                                                                \
        return 0;                                                                           \
    }                                                                                       \
    static __pragma(data_seg(".CRT$XIU")) int (*__##f##2##__COUNTER__)() = __##f##_wrapper; \
    static __pragma(data_seg()) void f()
#else
#define _TEST_INITIALIZER(f)                      \
    static void f() __attribute__((constructor)); \
    static void f()
#endif  //_MSC_VER


//----------------------------------------------------------------------------------------------------------------------
#define _TEST_MACRO_MAP1(_1, NAME, ...) NAME
#define _TEST_MACRO_MAP2(_1, _2, NAME, ...) NAME
#define _TEST_MACRO_MAP3(_1, _2, _3, NAME, ...) NAME
#define _TEST_MACRO_MAP4(_1, _2, _3, _4, NAME, ...) NAME
#define _TEST_MACRO_MAP5(_1, _2, _3, _4, _5, NAME, ...) NAME


//----------------------------------------------------------------------------------------------------------------------
#define _TEST_EVENT_DEFINE1(EVENTNAME)                              \
    static void EVENTNAME(test_ctx_t* ctx, int event, void* param); \
    _TEST_INITIALIZER(EVENTNAME##_event_init)                       \
    {                                                               \
        test_event_set((EVENTNAME), __FILE__);                      \
    }                                                               \
    static void name(test_ctx_t* ctx, int event, void* param)

#define _TEST_EVENT_DEFINE2(EVENTNAME, TARGET)                      \
    static void EVENTNAME(test_ctx_t* ctx, int event, void* param); \
    _TEST_INITIALIZER(EVENTNAME##_event_init)                       \
    {                                                               \
        test_event_set((EVENTNAME), (TARGET##_target).name);        \
    }                                                               \
    static void name(test_ctx_t* ctx, int event, void* param)


//----------------------------------------------------------------------------------------------------------------------
#define _TEST_DEFINE1(TESTNAME)                                                                                        \
    static void TESTNAME();                                                                                            \
    static struct test_t TESTNAME##_target;                                                                            \
    _TEST_INITIALIZER(TESTNAME##_target_init)                                                                          \
    {                                                                                                                  \
        TESTNAME##_target.next = &(TESTNAME##_target);                                                                 \
        TESTNAME##_target.prev = &(TESTNAME##_target);                                                                 \
        TESTNAME##_target.parent = NULL;                                                                               \
        TESTNAME##_target.childs = NULL;                                                                               \
        TESTNAME##_target.flags = 0;                                                                                   \
        TESTNAME##_target.name = #TESTNAME;                                                                            \
        TESTNAME##_target.context = 0;                                                                                 \
        TESTNAME##_target.event = NULL;                                                                                \
        TESTNAME##_target.proc = (TESTNAME);                                                                           \
        test_register(&(TESTNAME##_target), __FILE__);                                                                 \
    }                                                                                                                  \
    static void TESTNAME()


#define _TEST_DEFINE2(TESTNAME, GROUP)                                                                                 \
    static void TESTNAME();                                                                                            \
    static struct test_t TESTNAME##_target;                                                                            \
    _TEST_INITIALIZER(TESTNAME##_target_init)                                                                          \
    {                                                                                                                  \
        TESTNAME##_target.next = &(TESTNAME##_target);                                                                 \
        TESTNAME##_target.prev = &(TESTNAME##_target);                                                                 \
        TESTNAME##_target.parent = NULL;                                                                               \
        TESTNAME##_target.childs = NULL;                                                                               \
        TESTNAME##_target.flags = 0;                                                                                   \
        TESTNAME##_target.name = #TESTNAME;                                                                            \
        TESTNAME##_target.context = 0;                                                                                 \
        TESTNAME##_target.event = NULL;                                                                                \
        TESTNAME##_target.proc = (TESTNAME);                                                                           \
        test_register(&(TESTNAME##_target), ((GROUP##_target).name));                                                  \
    }                                                                                                                  \
    static void TESTNAME()


//----------------------------------------------------------------------------------------------------------------------
#define _TEST_GROUP_DEFINE1(GROUPNAME)                                                                                 \
    static struct test_t GROUPNAME##_target;                                                                           \
    _TEST_INITIALIZER(GROUPNAME##_target_init)                                                                         \
    {                                                                                                                  \
        GROUPNAME##_target.next = &(GROUPNAME##_target);                                                               \
        GROUPNAME##_target.prev = &(GROUPNAME##_target);                                                               \
        GROUPNAME##_target.parent = NULL;                                                                              \
        GROUPNAME##_target.childs = NULL;                                                                              \
        GROUPNAME##_target.flags = TEST_FLAG_GROUP;                                                                    \
        GROUPNAME##_target.name = __FILE__;                                                                            \
        GROUPNAME##_target.context = 0;                                                                                \
        GROUPNAME##_target.event = NULL;                                                                               \
        GROUPNAME##_target.proc = NULL;                                                                                \
        test_register(&(GROUPNAME##_target), "");                                                                      \
    }


#define _TEST_GROUP_DEFINE2(GROUPNAME, GROUPSTR)                                                                       \
    static struct test_t GROUPNAME##_target;                                                                           \
    _TEST_INITIALIZER(GROUPNAME##_target_init)                                                                         \
    {                                                                                                                  \
        GROUPNAME##_target.next = &(GROUPNAME##_target);                                                               \
        GROUPNAME##_target.prev = &(GROUPNAME##_target);                                                               \
        GROUPNAME##_target.parent = NULL;                                                                              \
        GROUPNAME##_target.childs = NULL;                                                                              \
        GROUPNAME##_target.flags = TEST_FLAG_GROUP;                                                                    \
        GROUPNAME##_target.name = (GROUPSTR);                                                                          \
        GROUPNAME##_target.context = 0;                                                                                \
        GROUPNAME##_target.event = NULL;                                                                               \
        GROUPNAME##_target.proc = NULL;                                                                                \
        test_register(&(GROUPNAME##_target), "");                                                                      \
    }


//----------------------------------------------------------------------------------------------------------------------
#define _TEST_ASSERT2(EXPR, MESSAGE)                                                                                   \
    ({                                                                                                                 \
        if (!(EXPR)) {                                                                                                 \
            test_raise(__FILE__, __LINE__, TYPE_ASSERT_FAIL, (MESSAGE), "%s", #EXPR);                                  \
        }                                                                                                              \
    })
#define _TEST_ASSERT1(EXPR)                                                                                            \
    ({                                                                                                                 \
        if (!(EXPR)) {                                                                                                 \
            test_raise(__FILE__, __LINE__, TYPE_ASSERT_FAIL, "", "%s", #EXPR);                                         \
        }                                                                                                              \
    })

#define _TEST_EXPECT2(EXPR, MESSAGE)                                                                                   \
    ({                                                                                                                 \
        if (!(EXPR)) {                                                                                                 \
            test_message(__FILE__, __LINE__, TYPE_EXPECT_FAIL, (MESSAGE), "%s", #EXPR);                                \
        }                                                                                                              \
    })
#define _TEST_EXPECT1(EXPR)                                                                                            \
    ({                                                                                                                 \
        if (!(EXPR)) {                                                                                                 \
            test_message(__FILE__, __LINE__, TYPE_EXPECT_FAIL, "", "%s", #EXPR);                                       \
        }                                                                                                              \
    })

//----------------------------------------------------------------------------------------------------------------------
#define _TEST_RUN2(pgroup, ptest) test_run(pgroup, ptest)
#define _TEST_RUN1(pgroup) test_run(pgroup, "*")
#define _TEST_RUN0() test_run("*", "*")


//----------------------------------------------------------------------------------------------------------------------
//! We need to redefine the `ASSERT`
#if defined(ASSERT)
#undef ASSERT
#endif //ASSERT


//----------------------------------------------------------------------------------------------------------------------
// clang-format off
// TEST(test-object, group-object)
// TEST_GROUP(group-object, group-string)
// TEST_EVENT(event-object, group-object)
#define TEST(...)       _TEST_MACRO_MAP2(__VA_ARGS__, _TEST_DEFINE2, _TEST_DEFINE1)(__VA_ARGS__)
#define TEST_GROUP(...) _TEST_MACRO_MAP2(__VA_ARGS__, _TEST_GROUP_DEFINE2, _TEST_GROUP_DEFINE1)(__VA_ARGS__)
#define TEST_EVENT(...) _TEST_MACRO_MAP2(__VA_ARGS__, _TEST_EVENT_DEFINE2, _TEST_EVENT_DEFINE1)(__VA_ARGS__)
#define TEST_RUN(...)   _TEST_MACRO_MAP3(__VA_ARGS__, _TEST_RUN2, _TEST_RUN1, _TEST_RUN0)(__VA_ARGS__)
#define ASSERT(...) _TEST_MACRO_MAP2(__VA_ARGS__, _TEST_ASSERT2, _TEST_ASSERT1)(__VA_ARGS__)
#define EXPECT(...) _TEST_MACRO_MAP2(__VA_ARGS__, _TEST_EXPECT2, _TEST_EXPECT1)(__VA_ARGS__)
// clang-format on


//======================================================================================================================
#endif  // test_H
