#ifndef PANIC_H
#define PANIC_H

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>


struct defer_list {
    struct defer_list* parent;
    jmp_buf jbuf;
    void* error;
    void (^block)(void);
};

static struct defer_list top_defer;
__thread static struct defer_list* last_defer = &top_defer;


static void defer_init() __attribute__((constructor));
static void defer_init()
{
    top_defer.parent = NULL;
    top_defer.error = &top_defer;
    top_defer.block = ^{};
    int ret = setjmp(top_defer.jbuf);
    if (0 != ret) {
        //printf("defer_init - abort before\n");
        //abort();
        //printf("defer_init - abort after\n");
    }
}


static void panic(void* error) __attribute__((noreturn))
{
    last_defer->error = error; //  将 error 存放到最后的 defer 处,然后触发jump
    longjmp(last_defer->jbuf, 1);
}


static void* recover()
{
    void* error = last_defer->error;

    //  将 error 设置为 error 自身的地址,以便于 defer_call_normal 能够识别处当前是
    //  否处于 panic 处理流程中, 且 error 已经被用户通过 recover 收走了
    last_defer->error = &(last_defer->error);

    return error;
}


static void defer_call_normal(struct defer_list* this_defer)
{
    //printf("defer_call_normal - enter: last_defer=%p, this_defer=%p\n", last_defer, this_defer);

    //  如果 error 与 error 自己的地址相等, 说明已经已经被恢复过, 此时 block 已经在 panic 之后被调用过了, 直接返回即可
    if (this_defer->error == &(this_defer->error)) {
        //printf("defer_call_normal - leave(1)\n");
        return;
    }

    //  调用 block 触发用户 defer 时注册的代码逻辑
    this_defer->block();

    //  如果 error 与 this_defer 相等, 说明没有抛出异常, 属于正常处理流程
    if (this_defer->error == this_defer) {
        //printf("defer_call_normal - leave(2)\n");
        return;
    }

    //  如果 error 不等于 this_defer,说明当前正好处于 panic 处理流程中, 用户的 defer 时注册的代码也没有通过 recover 将 error 收取掉
    //  此时 error 中存储的, 仍然还是用户通过 panic 设置的异常信息
    //  此时,意味着 panic 还需要忘上层传播
    if (this_defer->error != this_defer) {
        last_defer = last_defer->parent; //  将 last_defer 更新为 parent
        //printf("defer_call_normal - begin re-panic\n");
        panic(this_defer->error);
    }
}


#define defer(name, BLOCK)                                                                                             \
    struct defer_list name __attribute__((__unused__ __cleanup__(defer_call_normal)));                                 \
    (name).error = &(name); /* 初始时设置成 this_defer,应对大多数场景不会触发异常的场景 */       \
    (name).parent = last_defer;                                                                                        \
    (name).block = ^BLOCK;                                                                                             \
    last_defer = &(name);                                                                                              \
    do {                                                                                                               \
        if (0 != setjmp((name).jbuf)) {                                                                                \
            defer_call_normal(&(name));                                                                                \
        }                                                                                                              \
    } while (0)


#endif // PANIC_H
