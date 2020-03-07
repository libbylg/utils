#ifndef test_H
#define test_H


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
enum TEST_FLAG {
    TEST_FLAG_GROUP = 0x0001,   //! 测试组
    TEST_FLAG_DYNAMIC = 0x8000, //! 动态资源
};


//! The message type for testing.
enum TEST_MESSAGE {
    TEST_MESSAGE_PASS = 0,
    TEST_MESSAGE_SETUP_FAIL = 1,
    TEST_MESSAGE_TEARDOWN_FAIL = 2,
    TEST_MESSAGE_ASSERT_FAIL = 3,
    TEST_MESSAGE_EXPECT_FAIL = 4,
    TEST_MESSAGE_CATCH_FAULT = 5,
    TEST_MESSAGE_MEMORY_LEAK = 6,
    TEST_MESSAGE_INTERNAL_ERROR = 7,
};


//  The result of the testing
enum TEST_RESULT {
    TEST_RESULT_PASS,    //! Successfuly and do not have any exceptional
    TEST_RESULT_WARNING, //! Succes, but have some warning
    TEST_RESULT_FAIL,    //! Not pass the test
    TEST_RESULT_FAULT,   //! Hit some fault
};


//! What is the testing freamewoek doing now.
enum TEST_ACTION {
    TEST_ACTION_SETUP = 0,
    TEST_ACTION_TESTING = 1,
    TEST_ACTION_TEARDOWN = 2,
};


struct test_message_t;
struct test_result_t;
struct test_t;

typedef uintptr_t test_ctx_t;

typedef uint32_t test_flags_t;

typedef void (*test_event_t)();

typedef void (*test_proc_t)();

typedef void (*test_output_t)(struct test_message_t* message);

// clang-format off
struct test_message_t {
    struct test_message_t*  next;
    struct test_message_t*  prev;
    const char*             message;
    const char*             file;
    int                     line;
    uint16_t                type;
    uint16_t                len;
    char*                   text;
};
// clang-format on

// clang-format off
struct test_result_t {
    struct test_t*          test;
    int                     result;
    struct test_message_t*  cause;
};
// clang-format on


// clang-format off
struct test_t {
    struct test_t*          next;
    struct test_t*          prev;
    struct test_t*          parent;
    struct test_t*          childs;
    struct test_result_t*   result;
    uint32_t                flags;
    const char*             name;
    test_ctx_t              context;
    test_event_t            event;
    test_proc_t             proc;
};
// clang-format on


//----------------------------------------------------------------------------------------------------------------------
// clang-format off
//! `test_register` 函数用于将 `test` 对象注册到名称为 `group` 的组下面.
//! \param `test` 可以为一个"测试",也可以为一个"测试组". 
//! \param `group` 如果指定为"", 表示注册到全局的测试组下面
EXPORT_API struct test_t*   test_init(struct test_t* t, const char* name, test_proc_t proc, uint32_t flags);
EXPORT_API int              test_register(struct test_t* t, const char* group);
EXPORT_API struct test_t*   test_find(const char* name);
EXPORT_API struct test_t*   test_self();
EXPORT_API test_ctx_t       test_ctx();
EXPORT_API void             test_ctx_set(test_ctx_t ctx);
EXPORT_API enum TEST_ACTION test_action();
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
#define _TEST_EVENT_DEFINE1(EVENTNAME)                                                                                 \
    static void EVENTNAME();                                                                                           \
    _TEST_INITIALIZER(EVENTNAME##_event_init)                                                                          \
    {                                                                                                                  \
        struct test_t* group = test_find(__FILE__);                                                                    \
        test_event_set((EVENTNAME), group);                                                                            \
    }                                                                                                                  \
    static void EVENTNAME()

#define _TEST_EVENT_DEFINE2(TARGET, EVENTNAME)                                                                         \
    static void EVENTNAME();                                                                                           \
    _TEST_INITIALIZER(EVENTNAME##_event_init)                                                                          \
    {                                                                                                                  \
        test_event_set(&(TARGET##_target), (EVENTNAME));                                                               \
    }                                                                                                                  \
    static void EVENTNAME()


//----------------------------------------------------------------------------------------------------------------------
#define _TEST_DEFINE1(TESTNAME)                                                                                        \
    static void TESTNAME();                                                                                            \
    static struct test_t TESTNAME##_target;                                                                            \
    _TEST_INITIALIZER(TESTNAME##_target_init)                                                                          \
    {                                                                                                                  \
        test_register(test_init(&(TESTNAME##_target), #TESTNAME, (TESTNAME), 0), __FILE__);                            \
    }                                                                                                                  \
    static void TESTNAME()


#define _TEST_DEFINE2(GROUP, TESTNAME)                                                                                 \
    static void TESTNAME();                                                                                            \
    static struct test_t TESTNAME##_target;                                                                            \
    _TEST_INITIALIZER(TESTNAME##_target_init)                                                                          \
    {                                                                                                                  \
        test_register(test_init(&(TESTNAME##_target), #TESTNAME, (TESTNAME), 0), ((GROUP##_target).name));             \
    }                                                                                                                  \
    static void TESTNAME()


//----------------------------------------------------------------------------------------------------------------------
#define _TEST_GROUP_DEFINE1(GROUPNAME)                                                                                 \
    static struct test_t GROUPNAME##_target;                                                                           \
    _TEST_INITIALIZER(GROUPNAME##_target_init)                                                                         \
    {                                                                                                                  \
        test_register(test_init(&(GROUPNAME##_target), __FILE__, NULL, TEST_FLAG_GROUP), "");                          \
    }


#define _TEST_GROUP_DEFINE2(GROUPNAME, GROUPSTR)                                                                       \
    static struct test_t GROUPNAME##_target;                                                                           \
    _TEST_INITIALIZER(GROUPNAME##_target_init)                                                                         \
    {                                                                                                                  \
        test_register(test_init(&(GROUPNAME##_target), (GROUPSTR), NULL, TEST_FLAG_GROUP), "");                        \
    }


//----------------------------------------------------------------------------------------------------------------------
#define _TEST_ASSERT2(EXPR, MESSAGE)                                                                                   \
    ({                                                                                                                 \
        if (!(EXPR)) {                                                                                                 \
            test_raise(__FILE__, __LINE__, TEST_MESSAGE_ASSERT_FAIL, (MESSAGE), "%s", #EXPR);                          \
        }                                                                                                              \
    })
#define _TEST_ASSERT1(EXPR)                                                                                            \
    ({                                                                                                                 \
        if (!(EXPR)) {                                                                                                 \
            test_raise(__FILE__, __LINE__, TEST_MESSAGE_ASSERT_FAIL, "", "%s", #EXPR);                                 \
        }                                                                                                              \
    })

#define _TEST_EXPECT2(EXPR, MESSAGE)                                                                                   \
    ({                                                                                                                 \
        if (!(EXPR)) {                                                                                                 \
            test_message(__FILE__, __LINE__, TEST_RESULT_EXPECT_FAIL, (MESSAGE), "%s", #EXPR);                         \
        }                                                                                                              \
    })
#define _TEST_EXPECT1(EXPR)                                                                                            \
    ({                                                                                                                 \
        if (!(EXPR)) {                                                                                                 \
            test_message(__FILE__, __LINE__, TEST_RESULT_EXPECT_FAIL, "", "%s", #EXPR);                                \
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
// TEST([group-object], test-object)
// TEST_GROUP([group-object], group-string)
// TEST_EVENT([test-object|group-object], event-object)
#define TEST(...)       _TEST_MACRO_MAP2(__VA_ARGS__, _TEST_DEFINE2, _TEST_DEFINE1)(__VA_ARGS__)
#define TEST_GROUP(...) _TEST_MACRO_MAP2(__VA_ARGS__, _TEST_GROUP_DEFINE2, _TEST_GROUP_DEFINE1)(__VA_ARGS__)
#define TEST_EVENT(...) _TEST_MACRO_MAP2(__VA_ARGS__, _TEST_EVENT_DEFINE2, _TEST_EVENT_DEFINE1)(__VA_ARGS__)
#define TEST_RUN(...)   _TEST_MACRO_MAP3(__VA_ARGS__, _TEST_RUN2, _TEST_RUN1, _TEST_RUN0)(__VA_ARGS__)
#define ASSERT(...)     _TEST_MACRO_MAP2(__VA_ARGS__, _TEST_ASSERT2, _TEST_ASSERT1)(__VA_ARGS__)
#define EXPECT(...)     _TEST_MACRO_MAP2(__VA_ARGS__, _TEST_EXPECT2, _TEST_EXPECT1)(__VA_ARGS__)
// clang-format on


//======================================================================================================================
#endif  // test_H
